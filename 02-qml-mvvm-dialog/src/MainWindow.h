#pragma once

#include "Person.h"

#include <QWidget>

class QLabel;

class MainWindow: public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void editPerson();
    void showPerson();

    Person m_person{QStringLiteral("Ada Lovelace"), QStringLiteral("ada@example.com"), 36, Role::Developer};
    QLabel *m_summary = nullptr;
};
