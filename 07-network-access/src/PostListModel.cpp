#include "PostListModel.h"

void PostListModel::setPosts(QList<Post> posts)
{
    beginResetModel();
    m_posts = std::move(posts);
    endResetModel();
}

void PostListModel::prependPost(const Post &post)
{
    beginInsertRows({}, 0, 0);
    m_posts.prepend(post);
    endInsertRows();
}

int PostListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_posts.size());
}

QVariant PostListModel::data(const QModelIndex &index, const int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return {};

    const auto &post = m_posts.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return QStringLiteral("#%1  %2").arg(post.id).arg(post.title);
    case Qt::ToolTipRole:
        return post.body;
    default:
        return {};
    }
}
