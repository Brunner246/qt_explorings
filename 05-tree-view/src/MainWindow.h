#pragma once

#include <QMainWindow>

class ProjectTreeModel;
class QAction;
class QSortFilterProxyModel;
class QTreeView;

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void createToolBar();
    void addChild();
    void removeSelected();
    void applyFilter(const QString &text);
    void updateActions();
    [[nodiscard]] QModelIndex currentSourceIndex() const;

    ProjectTreeModel *m_model = nullptr;
    QSortFilterProxyModel *m_proxy = nullptr;
    QTreeView *m_tree = nullptr;
    QAction *m_addAction = nullptr;
    QAction *m_removeAction = nullptr;
};
