#include "ProjectTreeModel.h"

namespace
{
QString displayName(const NodeType type)
{
    switch (type) {
    case NodeType::Root:
        return {};
    case NodeType::Project:
        return ProjectTreeModel::tr("Project");
    case NodeType::Building:
        return ProjectTreeModel::tr("Building");
    case NodeType::Floor:
        return ProjectTreeModel::tr("Floor");
    case NodeType::Element:
        return ProjectTreeModel::tr("Element");
    }
    return {};
}
} // namespace

ProjectTreeModel::ProjectTreeModel(std::unique_ptr<TreeNode> root, QObject *parent)
    : QAbstractItemModel(parent), m_root(std::move(root))
{
}

ProjectTreeModel::~ProjectTreeModel() = default;

QModelIndex ProjectTreeModel::index(const int row, const int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};

    // The node pointer travels inside the index, so parent() and data() can find it again in O(1).
    return createIndex(row, column, nodeFor(parent)->child(row));
}

QModelIndex ProjectTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    TreeNode *parentNode = nodeFor(index)->parent();
    if (!parentNode || parentNode == m_root.get())
        return {};

    return createIndex(parentNode->row(), 0, parentNode);
}

int ProjectTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;
    return nodeFor(parent)->childCount();
}

int ProjectTreeModel::columnCount(const QModelIndex &) const { return ColumnCount; }

QVariant ProjectTreeModel::data(const QModelIndex &index, const int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return {};

    const TreeNode *node = nodeFor(index);
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case NameColumn:
            return node->name();
        case TypeColumn:
            return displayName(node->type());
        case ElementCountColumn:
            return node->type() == NodeType::Element ? QVariant() : node->elementCount();
        }
    }
    if (role == Qt::TextAlignmentRole && index.column() == ElementCountColumn)
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);

    return {};
}

QVariant ProjectTreeModel::headerData(const int section, const Qt::Orientation orientation, const int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return {};

    switch (section) {
    case NameColumn:
        return tr("Name");
    case TypeColumn:
        return tr("Type");
    case ElementCountColumn:
        return tr("Elements");
    }
    return {};
}

Qt::ItemFlags ProjectTreeModel::flags(const QModelIndex &index) const
{
    auto flags = QAbstractItemModel::flags(index);
    if (index.isValid() && index.column() == NameColumn)
        flags |= Qt::ItemIsEditable;
    return flags;
}

bool ProjectTreeModel::setData(const QModelIndex &index, const QVariant &value, const int role)
{
    const auto name = value.toString().trimmed();
    if (role != Qt::EditRole || index.column() != NameColumn || name.isEmpty())
        return false;

    nodeFor(index)->setName(name);
    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
    return true;
}

bool ProjectTreeModel::insertRows(const int row, const int count, const QModelIndex &parent)
{
    TreeNode *parentNode = nodeFor(parent);
    const auto childType = parentNode->childType();
    if (!childType || row < 0 || row > parentNode->childCount() || count <= 0)
        return false;

    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        const auto name = tr("New %1").arg(displayName(*childType));
        parentNode->insertChild(row + i, std::make_unique<TreeNode>(name, *childType));
    }
    endInsertRows();

    notifyElementCountChanged(parent);
    return true;
}

bool ProjectTreeModel::removeRows(const int row, const int count, const QModelIndex &parent)
{
    TreeNode *parentNode = nodeFor(parent);
    if (row < 0 || count <= 0 || row + count > parentNode->childCount())
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    parentNode->removeChildren(row, count);
    endRemoveRows();

    notifyElementCountChanged(parent);
    return true;
}

bool ProjectTreeModel::canHaveChildren(const QModelIndex &index) const
{
    return nodeFor(index)->childType().has_value();
}

QModelIndex ProjectTreeModel::appendChild(const QModelIndex &parent)
{
    const int row = rowCount(parent);
    return insertRows(row, 1, parent) ? index(row, NameColumn, parent) : QModelIndex();
}

TreeNode *ProjectTreeModel::nodeFor(const QModelIndex &index) const
{
    return index.isValid() ? static_cast<TreeNode *>(index.internalPointer()) : m_root.get();
}

void ProjectTreeModel::notifyElementCountChanged(const QModelIndex &index)
{
    for (auto current = index; current.isValid(); current = current.parent()) {
        const auto countIndex = current.siblingAtColumn(ElementCountColumn);
        emit dataChanged(countIndex, countIndex, {Qt::DisplayRole});
    }
}
