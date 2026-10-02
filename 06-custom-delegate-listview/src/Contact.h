#pragma once

#include <QString>

enum class Presence
{
    Online,
    Away,
    Busy,
    Offline
};

struct Contact
{
    QString name;
    QString title;
    QString email;
    Presence presence = Presence::Offline;
    bool favorite = false;
};
