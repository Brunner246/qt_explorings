#pragma once

#include "Post.h"

#include <QList>
#include <QObject>

#include <functional>

class QJsonDocument;
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

class JsonPlaceholderClient: public QObject
{
    Q_OBJECT

public:
    explicit JsonPlaceholderClient(QObject *parent = nullptr);

    void fetchPosts();
    void fetchComments(int postId);
    void createPost(const Post &post);

    [[nodiscard]] bool isBusy() const { return m_pendingRequests > 0; }

signals:
    void postsReceived(const QList<Post> &posts);
    void commentsReceived(int postId, const QList<Comment> &comments);
    void postCreated(const Post &post);
    void errorOccurred(const QString &message);
    void busyChanged(bool busy);

private:
    using JsonHandler = std::function<void(const QJsonDocument &)>;

    [[nodiscard]] QNetworkRequest createRequest(const QString &path) const;
    void handleReply(QNetworkReply *reply, JsonHandler onSuccess);
    void setPendingRequests(int count);

    QNetworkAccessManager *m_network = nullptr;
    int m_pendingRequests = 0;
};
