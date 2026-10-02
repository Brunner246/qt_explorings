#include "MainWindow.h"

#include "ProjectTreeModel.h"
#include "SampleProject.h"

#include <QHeaderView>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QToolBar>
#include <QTreeView>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("05 - Tree View"));

    m_model = new ProjectTreeModel(createSampleProjects(), this);

    m_proxy = new QSortFilterProxyModel(this);
    m_proxy->setSourceModel(m_model);
    m_proxy->setRecursiveFilteringEnabled(true);
    m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxy->setFilterKeyColumn(ProjectTreeModel::NameColumn);

    m_tree = new QTreeView(this);
    m_tree->setModel(m_proxy);
    m_tree->setSortingEnabled(true);
    m_tree->sortByColumn(-1, Qt::AscendingOrder);
    m_tree->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    m_tree->header()->setSectionResizeMode(ProjectTreeModel::NameColumn, QHeaderView::Stretch);
    m_tree->header()->setStretchLastSection(false);
    m_tree->expandToDepth(1);
    setCentralWidget(m_tree);

    createToolBar();

    connect(m_tree->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::updateActions);
    updateActions();
}

void MainWindow::createToolBar()
{
    auto *toolBar = addToolBar(tr("Tree"));
    toolBar->setMovable(false);

    m_addAction = toolBar->addAction(tr("Add Child"), this, &MainWindow::addChild);
    m_removeAction = toolBar->addAction(tr("Remove"), this, &MainWindow::removeSelected);
    m_removeAction->setShortcut(QKeySequence::Delete);
    toolBar->addSeparator();
    toolBar->addAction(tr("Expand All"), m_tree, &QTreeView::expandAll);
    toolBar->addAction(tr("Collapse All"), m_tree, &QTreeView::collapseAll);
    toolBar->addSeparator();

    auto *search = new QLineEdit(toolBar);
    search->setPlaceholderText(tr("Filter by name…"));
    search->setClearButtonEnabled(true);
    search->setMaximumWidth(220);
    toolBar->addWidget(search);
    connect(search, &QLineEdit::textChanged, this, &MainWindow::applyFilter);
}

void MainWindow::addChild()
{
    const auto parent = currentSourceIndex();
    const auto child = m_model->appendChild(parent);
    if (!child.isValid())
        return;

    const auto proxyChild = m_proxy->mapFromSource(child);
    if (!proxyChild.isValid())
        return;

    m_tree->expand(proxyChild.parent());
    m_tree->setCurrentIndex(proxyChild);
    m_tree->edit(proxyChild);
}

void MainWindow::removeSelected()
{
    const auto index = currentSourceIndex();
    if (index.isValid())
        m_model->removeRow(index.row(), index.parent());
}

void MainWindow::applyFilter(const QString &text)
{
    m_proxy->setFilterFixedString(text);
    if (!text.isEmpty())
        m_tree->expandAll();
}

void MainWindow::updateActions()
{
    const auto index = currentSourceIndex();
    m_addAction->setEnabled(m_model->canHaveChildren(index));
    m_removeAction->setEnabled(index.isValid());
}

QModelIndex MainWindow::currentSourceIndex() const
{
    const auto proxyIndex = m_tree->currentIndex().siblingAtColumn(ProjectTreeModel::NameColumn);
    return m_proxy->mapToSource(proxyIndex);
}
