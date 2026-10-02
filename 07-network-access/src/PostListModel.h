#pragma once

#include "Post.h"

#include <QAbstractListModel>
#include <QList>

class PostListModel: public QAbstractListModel
{
    Q_OBJECT

public:
    using QAbstractListModel::QAbstractListModel;

    void setPosts(QList<Post> posts);
    void prependPost(const Post &post);
    [[nodiscard]] const Post &postAt(const int row) const { return m_posts.at(row); }

    [[nodiscard]] int rowCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<Post> m_posts;
};
