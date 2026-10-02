#include "ContactListModel.h"

ContactListModel::ContactListModel(QList<Contact> contacts, QObject *parent)
    : QAbstractListModel(parent), m_contacts(std::move(contacts))
{
}

int ContactListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_contacts.size());
}

QVariant ContactListModel::data(const QModelIndex &index, const int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return {};

    const auto &contact = m_contacts.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case NameRole:
        return contact.name;
    case TitleRole:
        return contact.title;
    case EmailRole:
        return contact.email;
    case PresenceRole:
        return QVariant::fromValue(contact.presence);
    case FavoriteRole:
        return contact.favorite;
    default:
        return {};
    }
}

bool ContactListModel::setData(const QModelIndex &index, const QVariant &value, const int role)
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid) || role != FavoriteRole)
        return false;

    m_contacts[index.row()].favorite = value.toBool();
    emit dataChanged(index, index, {FavoriteRole});
    return true;
}
