#include "ItemTableModel.h"

ItemTableModel::ItemTableModel(const std::optional<int> capacity, QObject *parent)
    : QAbstractTableModel(parent), m_capacity(capacity)
{
}

void ItemTableModel::setItems(QList<Item> items)
{
    beginResetModel();
    m_items = std::move(items);
    endResetModel();
    emit countChanged();
    emit fullChanged();
}

void ItemTableModel::setCapacity(const std::optional<int> capacity)
{
    if (m_capacity == capacity)
        return;
    m_capacity = capacity;
    emit capacityChanged();
    emit fullChanged();
}

bool ItemTableModel::isFull() const { return m_capacity && m_items.size() >= *m_capacity; }

bool ItemTableModel::moveRowTo(const int row, ItemTableModel *target)
{
    if (!target || target == this || target->isFull() || row < 0 || row >= m_items.size())
        return false;

    target->append(m_items.at(row));
    removeAt(row);
    return true;
}

int ItemTableModel::rowCount(const QModelIndex &parent) const { return parent.isValid() ? 0 : count(); }

int ItemTableModel::columnCount(const QModelIndex &parent) const { return parent.isValid() ? 0 : ColumnCount; }

QVariant ItemTableModel::data(const QModelIndex &index, const int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return {};

    const auto &item = m_items.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return index.column() == NameColumn ? item.name : item.category;
    case NameRole:
        return item.name;
    default:
        return {};
    }
}

QVariant ItemTableModel::headerData(const int section, const Qt::Orientation orientation, const int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QAbstractTableModel::headerData(section, orientation, role);
    return section == NameColumn ? tr("Name") : tr("Category");
}

QHash<int, QByteArray> ItemTableModel::roleNames() const
{
    auto roles = QAbstractTableModel::roleNames();
    roles.insert(NameRole, "name");
    return roles;
}

void ItemTableModel::append(const Item &item)
{
    const int row = count();
    beginInsertRows({}, row, row);
    m_items.append(item);
    endInsertRows();
    emit countChanged();
    emit fullChanged();
}

void ItemTableModel::removeAt(const int row)
{
    beginRemoveRows({}, row, row);
    m_items.removeAt(row);
    endRemoveRows();
    emit countChanged();
    emit fullChanged();
}
