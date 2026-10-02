#include "ItemTableModel.h"

#include <QIODevice>
#include <QMimeData>

#include <algorithm>
#include <set>

namespace
{
const QString itemMimeType = QStringLiteral("application/x-qtexamples-item");
}

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
}

void ItemTableModel::setCapacity(const std::optional<int> capacity)
{
    if (m_capacity == capacity)
        return;
    m_capacity = capacity;
    emit capacityChanged();
}

bool ItemTableModel::isFull() const { return m_capacity && m_items.size() >= *m_capacity; }

int ItemTableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_items.size());
}

int ItemTableModel::columnCount(const QModelIndex &parent) const { return parent.isValid() ? 0 : ColumnCount; }

QVariant ItemTableModel::data(const QModelIndex &index, const int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid) || role != Qt::DisplayRole)
        return {};

    const auto &item = m_items.at(index.row());
    return index.column() == NameColumn ? item.name : item.category;
}

QVariant ItemTableModel::headerData(const int section, const Qt::Orientation orientation, const int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QAbstractTableModel::headerData(section, orientation, role);
    return section == NameColumn ? tr("Name") : tr("Category");
}

Qt::ItemFlags ItemTableModel::flags(const QModelIndex &index) const
{
    const auto defaultFlags = QAbstractTableModel::flags(index);
    return index.isValid() ? defaultFlags | Qt::ItemIsDragEnabled : defaultFlags | Qt::ItemIsDropEnabled;
}

Qt::DropActions ItemTableModel::supportedDragActions() const { return Qt::MoveAction; }

Qt::DropActions ItemTableModel::supportedDropActions() const { return Qt::MoveAction; }

QStringList ItemTableModel::mimeTypes() const { return {itemMimeType}; }

QMimeData *ItemTableModel::mimeData(const QModelIndexList &indexes) const
{
    std::set<int> rows;
    for (const auto &index : indexes)
        rows.insert(index.row());

    QByteArray encoded;
    QDataStream stream(&encoded, QIODevice::WriteOnly);
    for (int row : rows)
        stream << m_items.at(row);

    auto *mimeData = new QMimeData;
    mimeData->setData(itemMimeType, encoded);
    return mimeData;
}

bool ItemTableModel::canDropMimeData(const QMimeData *data, const Qt::DropAction action, int, int, const QModelIndex &) const
{
    if (action != Qt::MoveAction || !data->hasFormat(itemMimeType))
        return false;
    if (!m_capacity)
        return true;
    return m_items.size() + decode(data).size() <= *m_capacity;
}

bool ItemTableModel::dropMimeData(
    const QMimeData *data, const Qt::DropAction action, int row, const int column, const QModelIndex &parent)
{
    if (!canDropMimeData(data, action, row, column, parent))
        return false;

    // Dropping onto a cell inserts before that cell's row, dropping on empty space appends.
    if (row < 0)
        row = parent.isValid() ? parent.row() : rowCount();

    const auto incoming = decode(data);
    beginInsertRows({}, row, row + static_cast<int>(incoming.size()) - 1);
    for (qsizetype i = 0; i < incoming.size(); ++i)
        m_items.insert(row + i, incoming.at(i));
    endInsertRows();
    emit countChanged();
    return true;
}

bool ItemTableModel::removeRows(const int row, const int count, const QModelIndex &parent)
{
    if (parent.isValid() || row < 0 || count <= 0 || row + count > m_items.size())
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    m_items.remove(row, count);
    endRemoveRows();
    emit countChanged();
    return true;
}

QList<Item> ItemTableModel::decode(const QMimeData *data)
{
    QList<Item> items;
    QDataStream stream(data->data(itemMimeType));
    while (!stream.atEnd()) {
        Item item;
        stream >> item;
        items.append(item);
    }
    return items;
}
