#pragma once

#include "Item.h"

#include <QAbstractTableModel>
#include <QList>

#include <optional>

class ItemTableModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column
    {
        NameColumn,
        CategoryColumn,
        ColumnCount
    };

    explicit ItemTableModel(std::optional<int> capacity = std::nullopt, QObject *parent = nullptr);

    void setItems(QList<Item> items);
    [[nodiscard]] const QList<Item> &items() const { return m_items; }

    [[nodiscard]] std::optional<int> capacity() const { return m_capacity; }
    void setCapacity(std::optional<int> capacity);
    [[nodiscard]] bool isFull() const;

    [[nodiscard]] int rowCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

    [[nodiscard]] Qt::DropActions supportedDragActions() const override;
    [[nodiscard]] Qt::DropActions supportedDropActions() const override;
    [[nodiscard]] QStringList mimeTypes() const override;
    [[nodiscard]] QMimeData *mimeData(const QModelIndexList &indexes) const override;
    [[nodiscard]] bool canDropMimeData(
        const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    bool
    dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent = {}) override;

signals:
    void countChanged();
    void capacityChanged();

private:
    [[nodiscard]] static QList<Item> decode(const QMimeData *data);

    QList<Item> m_items;
    std::optional<int> m_capacity;
};
