#pragma once

#include <QString>

enum class Role
{
    Developer,
    Designer,
    Manager
};

struct Person
{
    QString name;
    QString email;
    int age = 30;
    Role role = Role::Developer;
};
