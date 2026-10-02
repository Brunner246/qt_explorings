#pragma once

#include "Person.h"

#include <QObject>
#include <QStringList>
#include <QtQml/qqmlregistration.h>

class PersonViewModel: public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Provided by PersonDialog")
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(int age READ age WRITE setAge NOTIFY ageChanged)
    Q_PROPERTY(int roleIndex READ roleIndex WRITE setRoleIndex NOTIFY roleIndexChanged)
    Q_PROPERTY(QStringList roleNames READ roleNames CONSTANT)
    Q_PROPERTY(int minimumAge READ minimumAge CONSTANT)
    Q_PROPERTY(int maximumAge READ maximumAge CONSTANT)
    Q_PROPERTY(bool isValid READ isValid NOTIFY validationChanged)
    Q_PROPERTY(QString validationMessage READ validationMessage NOTIFY validationChanged)

public:
    explicit PersonViewModel(QObject *parent = nullptr);

    void load(const Person &person);
    [[nodiscard]] Person toPerson() const;
    [[nodiscard]] static QString displayName(Role role);

    [[nodiscard]] QString name() const { return m_name; }
    [[nodiscard]] QString email() const { return m_email; }
    [[nodiscard]] int age() const { return m_age; }
    [[nodiscard]] int roleIndex() const { return m_roleIndex; }
    [[nodiscard]] QStringList roleNames() const;
    [[nodiscard]] int minimumAge() const { return 16; }
    [[nodiscard]] int maximumAge() const { return 99; }
    [[nodiscard]] bool isValid() const { return m_validationMessage.isEmpty(); }
    [[nodiscard]] QString validationMessage() const { return m_validationMessage; }

public slots:
    void setName(const QString &name);
    void setEmail(const QString &email);
    void setAge(int age);
    void setRoleIndex(int roleIndex);
    void revert();

signals:
    void nameChanged();
    void emailChanged();
    void ageChanged();
    void roleIndexChanged();
    void validationChanged();

private:
    void validate();

    Person m_original;
    QString m_name;
    QString m_email;
    int m_age = 0;
    int m_roleIndex = 0;
    QString m_validationMessage;
};
