#include "MainWindow.h"

#include "JsonPlaceholderClient.h"
#include "NewPostDialog.h"
#include "PostListModel.h"

#include <QListView>
#include <QMessageBox>
#include <QProgressBar>
#include <QSplitter>
#include <QStatusBar>
#include <QTextBrowser>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_client(new JsonPlaceholderClient(this)), m_posts(new PostListModel(this))
{
    setWindowTitle(tr("07 - Network Access"));

    m_postList = new QListView(this);
    m_postList->setModel(m_posts);
    m_postList->setSelectionMode(QAbstractItemView::SingleSelection);

    m_details = new QTextBrowser(this);
    m_details->setPlaceholderText(tr("Select a post to see its comments."));

    auto *splitter = new QSplitter(this);
    splitter->addWidget(m_postList);
    splitter->addWidget(m_details);
    splitter->setStretchFactor(1, 1);
    setCentralWidget(splitter);

    m_busyIndicator = new QProgressBar(this);
    m_busyIndicator->setRange(0, 0);
    m_busyIndicator->setMaximumWidth(120);
    m_busyIndicator->hide();
    statusBar()->addPermanentWidget(m_busyIndicator);

    createActions();
    connectClient();
    connect(m_postList->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::showSelectedPost);

    m_client->fetchPosts();
}

void MainWindow::createActions()
{
    auto *toolBar = addToolBar(tr("Posts"));
    toolBar->setMovable(false);

    auto *reload = toolBar->addAction(tr("Reload"), m_client, &JsonPlaceholderClient::fetchPosts);
    reload->setShortcut(QKeySequence::Refresh);
    toolBar->addAction(tr("New Post…"), this, &MainWindow::openNewPostDialog);
}

void MainWindow::connectClient()
{
    connect(m_client, &JsonPlaceholderClient::busyChanged, m_busyIndicator, &QWidget::setVisible);
    connect(m_client, &JsonPlaceholderClient::errorOccurred, this, &MainWindow::showError);
    connect(m_client, &JsonPlaceholderClient::commentsReceived, this, &MainWindow::showComments);
    connect(m_client, &JsonPlaceholderClient::postCreated, this, &MainWindow::onPostCreated);
    connect(m_client,
            &JsonPlaceholderClient::postsReceived,
            this,
            [this](const QList<Post> &posts)
            {
                m_posts->setPosts(posts);
                statusBar()->showMessage(tr("Loaded %n post(s)", nullptr, static_cast<int>(posts.size())), 3000);
            });
}

void MainWindow::showSelectedPost()
{
    const auto index = m_postList->currentIndex();
    if (!index.isValid())
        return;

    const auto &post = m_posts->postAt(index.row());
    m_currentPostId = post.id;
    m_details->setHtml(tr("<h2>%1</h2><p>%2</p><p><i>Loading comments…</i></p>")
                           .arg(post.title.toHtmlEscaped(), post.body.toHtmlEscaped()));
    m_client->fetchComments(post.id);
}

void MainWindow::showComments(const int postId, const QList<Comment> &comments)
{
    // Replies can arrive out of order; ignore comments for a post that is no longer selected.
    if (postId != m_currentPostId)
        return;

    const auto &post = m_posts->postAt(m_postList->currentIndex().row());
    QString html = tr("<h2>%1</h2><p>%2</p><h3>Comments (%3)</h3>")
                       .arg(post.title.toHtmlEscaped(), post.body.toHtmlEscaped())
                       .arg(comments.size());
    for (const auto &comment : comments)
        html += QStringLiteral("<p><b>%1</b> &lt;%2&gt;<br>%3</p>")
                    .arg(comment.name.toHtmlEscaped(), comment.email.toHtmlEscaped(), comment.body.toHtmlEscaped());

    m_details->setHtml(html);
}

void MainWindow::showError(const QString &message) { statusBar()->showMessage(tr("Error: %1").arg(message), 8000); }

void MainWindow::openNewPostDialog()
{
    NewPostDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
        m_client->createPost(dialog.post());
}

void MainWindow::onPostCreated(const Post &post)
{
    m_posts->prependPost(post);
    QMessageBox::information(this,
                             tr("Post Created"),
                             tr("The server created the post with id %1.\n\n"
                                "Note: jsonplaceholder does not persist data, so the post will be gone after a reload.")
                                 .arg(post.id));
}
