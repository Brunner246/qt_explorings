#pragma once

#include "TreeNode.h"

#include <QAbstractItemModel>

#include <memory>

class ProjectTreeModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Column
    {
        NameColumn,
        TypeColumn,
        ElementCountColumn,
        ColumnCount
    };

    explicit ProjectTreeModel(std::unique_ptr<TreeNode> root, QObject *parent = nullptr);
    ~ProjectTreeModel() override;

    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
    [[nodiscard]] QModelIndex parent(const QModelIndex &index) const override;
    [[nodiscard]] int rowCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent = {}) override;
    bool removeRows(int row, int count, const QModelIndex &parent = {}) override;

    [[nodiscard]] bool canHaveChildren(const QModelIndex &index) const;
    QModelIndex appendChild(const QModelIndex &parent);

private:
    [[nodiscard]] TreeNode *nodeFor(const QModelIndex &index) const;
    void notifyElementCountChanged(const QModelIndex &index);

    std::unique_ptr<TreeNode> m_root;
};
