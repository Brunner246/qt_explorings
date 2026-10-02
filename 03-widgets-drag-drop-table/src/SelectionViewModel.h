#pragma once

#include "Item.h"

#include <QObject>

class ItemTableModel;

class SelectionViewModel: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int capacity READ capacity WRITE setCapacity NOTIFY capacityChanged)
    Q_PROPERTY(QString summary READ summary NOTIFY summaryChanged)
    Q_PROPERTY(bool isFull READ isFull NOTIFY summaryChanged)

public:
    explicit SelectionViewModel(int capacity, QObject *parent = nullptr);

    [[nodiscard]] ItemTableModel *availableItems() const { return m_available; }
    [[nodiscard]] ItemTableModel *selectedItems() const { return m_selected; }

    [[nodiscard]] int capacity() const;
    void setCapacity(int capacity) const;

    [[nodiscard]] QString summary() const;
    [[nodiscard]] bool isFull() const;
    [[nodiscard]] QList<Item> selection() const;

signals:
    void capacityChanged();
    void summaryChanged();

private:
    ItemTableModel *m_available = nullptr;
    ItemTableModel *m_selected = nullptr;
};
