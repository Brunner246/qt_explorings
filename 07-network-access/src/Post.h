#pragma once

#include <QJsonObject>
#include <QString>

struct Post
{
    int id = 0;
    int userId = 1;
    QString title;
    QString body;

    [[nodiscard]] static Post fromJson(const QJsonObject &json)
    {
        return Post{json["id"].toInt(), json["userId"].toInt(), json["title"].toString(), json["body"].toString()};
    }

    [[nodiscard]] QJsonObject toJson() const { return {{"userId", userId}, {"title", title}, {"body", body}}; }
};

struct Comment
{
    int id = 0;
    int postId = 0;
    QString name;
    QString email;
    QString body;

    [[nodiscard]] static Comment fromJson(const QJsonObject &json)
    {
        return Comment{json["id"].toInt(),
                       json["postId"].toInt(),
                       json["name"].toString(),
                       json["email"].toString(),
                       json["body"].toString()};
    }
};
