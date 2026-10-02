#pragma once

#include "Contact.h"

#include <QAbstractListModel>
#include <QList>

class ContactListModel: public QAbstractListModel
{
    Q_OBJECT

public:
    enum Role
    {
        NameRole = Qt::UserRole + 1,
        TitleRole,
        EmailRole,
        PresenceRole,
        FavoriteRole
    };

    explicit ContactListModel(QList<Contact> contacts, QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:
    QList<Contact> m_contacts;
};
