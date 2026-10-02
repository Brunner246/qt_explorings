#include "PersonViewModel.h"

#include <QRegularExpression>

PersonViewModel::PersonViewModel(QObject *parent) : QObject(parent) { load(Person{}); }

void PersonViewModel::load(const Person &person)
{
    m_original = person;
    revert();
}

Person PersonViewModel::toPerson() const
{
    return Person{m_name.trimmed(), m_email.trimmed(), m_age, static_cast<Role>(m_roleIndex)};
}

QString PersonViewModel::displayName(const Role role)
{
    switch (role) {
    case Role::Developer:
        return tr("Developer");
    case Role::Designer:
        return tr("Designer");
    case Role::Manager:
        return tr("Manager");
    }
    return {};
}

QStringList PersonViewModel::roleNames() const
{
    return {displayName(Role::Developer), displayName(Role::Designer), displayName(Role::Manager)};
}

void PersonViewModel::setName(const QString &name)
{
    if (m_name == name)
        return;
    m_name = name;
    emit nameChanged();
    validate();
}

void PersonViewModel::setEmail(const QString &email)
{
    if (m_email == email)
        return;
    m_email = email;
    emit emailChanged();
    validate();
}

void PersonViewModel::setAge(const int age)
{
    if (m_age == age)
        return;
    m_age = age;
    emit ageChanged();
    validate();
}

void PersonViewModel::setRoleIndex(const int roleIndex)
{
    if (m_roleIndex == roleIndex || roleIndex < 0 || roleIndex >= roleNames().size())
        return;
    m_roleIndex = roleIndex;
    emit roleIndexChanged();
}

void PersonViewModel::revert()
{
    setName(m_original.name);
    setEmail(m_original.email);
    setAge(m_original.age);
    setRoleIndex(static_cast<int>(m_original.role));
    validate();
}

void PersonViewModel::validate()
{
    static const QRegularExpression emailPattern(QStringLiteral(R"(^[^@\s]+@[^@\s]+\.[^@\s]+$)"));

    QString message;
    if (m_name.trimmed().isEmpty())
        message = tr("Name is required.");
    else if (!emailPattern.match(m_email.trimmed()).hasMatch())
        message = tr("Please enter a valid email address.");
    else if (m_age < minimumAge() || m_age > maximumAge())
        message = tr("Age must be between %1 and %2.").arg(minimumAge()).arg(maximumAge());

    if (m_validationMessage == message)
        return;
    m_validationMessage = message;
    emit validationChanged();
}
