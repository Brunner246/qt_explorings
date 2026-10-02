#pragma once

#include "Post.h"

#include <QMainWindow>

class JsonPlaceholderClient;
class PostListModel;
class QListView;
class QProgressBar;
class QTextBrowser;

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void createActions();
    void connectClient();
    void showSelectedPost();
    void showComments(int postId, const QList<Comment> &comments);
    void showError(const QString &message);
    void openNewPostDialog();
    void onPostCreated(const Post &post);

    JsonPlaceholderClient *m_client = nullptr;
    PostListModel *m_posts = nullptr;
    QListView *m_postList = nullptr;
    QTextBrowser *m_details = nullptr;
    QProgressBar *m_busyIndicator = nullptr;
    int m_currentPostId = 0;
};
