#pragma once

#include <QDataStream>
#include <QString>

struct Item
{
    int id = 0;
    QString name;
    QString category;
};

inline QDataStream &operator<<(QDataStream &stream, const Item &item)
{
    return stream << item.id << item.name << item.category;
}

inline QDataStream &operator>>(QDataStream &stream, Item &item)
{
    return stream >> item.id >> item.name >> item.category;
}
