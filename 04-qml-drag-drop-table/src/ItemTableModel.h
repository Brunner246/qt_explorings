#pragma once

#include "Item.h"

#include <QAbstractTableModel>
#include <QList>
#include <QtQml/qqmlregistration.h>

#include <optional>

class ItemTableModel: public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Provided by SelectionViewModel")
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool isFull READ isFull NOTIFY fullChanged)

public:
    enum Column
    {
        NameColumn,
        CategoryColumn,
        ColumnCount
    };

    enum Role
    {
        NameRole = Qt::UserRole + 1
    };

    explicit ItemTableModel(std::optional<int> capacity = std::nullopt, QObject *parent = nullptr);

    void setItems(QList<Item> items);
    [[nodiscard]] const QList<Item> &items() const { return m_items; }
    [[nodiscard]] int count() const { return static_cast<int>(m_items.size()); }

    [[nodiscard]] std::optional<int> capacity() const { return m_capacity; }
    void setCapacity(std::optional<int> capacity);
    [[nodiscard]] bool isFull() const;

    Q_INVOKABLE bool moveRowTo(int row, ItemTableModel *target);

    [[nodiscard]] int rowCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

signals:
    void countChanged();
    void capacityChanged();
    void fullChanged();

private:
    void append(const Item &item);
    void removeAt(int row);

    QList<Item> m_items;
    std::optional<int> m_capacity;
};
