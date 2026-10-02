#include "JsonPlaceholderClient.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

namespace
{
const QString baseUrl = QStringLiteral("https://jsonplaceholder.typicode.com");
constexpr std::chrono::seconds requestTimeout{10};

template<typename T> QList<T> parseArray(const QJsonDocument &document)
{
    QList<T> result;
    for (const auto &value : document.array())
        result.append(T::fromJson(value.toObject()));
    return result;
}
} // namespace

JsonPlaceholderClient::JsonPlaceholderClient(QObject *parent)
    : QObject(parent), m_network(new QNetworkAccessManager(this))
{
    m_network->setTransferTimeout(requestTimeout);
}

void JsonPlaceholderClient::fetchPosts()
{
    handleReply(m_network->get(createRequest(QStringLiteral("/posts"))),
                [this](const QJsonDocument &document) { emit postsReceived(parseArray<Post>(document)); });
}

void JsonPlaceholderClient::fetchComments(int postId)
{
    auto request = createRequest(QStringLiteral("/comments"));
    auto url = request.url();
    url.setQuery(QUrlQuery{{QStringLiteral("postId"), QString::number(postId)}});
    request.setUrl(url);

    handleReply(m_network->get(request),
                [this, postId](const QJsonDocument &document)
                { emit commentsReceived(postId, parseArray<Comment>(document)); });
}

void JsonPlaceholderClient::createPost(const Post &post)
{
    const auto payload = QJsonDocument(post.toJson()).toJson(QJsonDocument::Compact);
    handleReply(m_network->post(createRequest(QStringLiteral("/posts")), payload),
                [this](const QJsonDocument &document) { emit postCreated(Post::fromJson(document.object())); });
}

QNetworkRequest JsonPlaceholderClient::createRequest(const QString &path) const
{
    QNetworkRequest request(QUrl(baseUrl + path));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json; charset=UTF-8"));
    request.setRawHeader("Accept", "application/json");
    return request;
}

void JsonPlaceholderClient::handleReply(QNetworkReply *reply, JsonHandler onSuccess)
{
    setPendingRequests(m_pendingRequests + 1);

    connect(reply,
            &QNetworkReply::finished,
            this,
            [this, reply, onSuccess = std::move(onSuccess)]
            {
                reply->deleteLater();
                setPendingRequests(m_pendingRequests - 1);

                if (reply->error() != QNetworkReply::NoError) {
                    emit errorOccurred(reply->errorString());
                    return;
                }

                QJsonParseError parseError;
                const auto document = QJsonDocument::fromJson(reply->readAll(), &parseError);
                if (parseError.error != QJsonParseError::NoError) {
                    emit errorOccurred(tr("Invalid JSON: %1").arg(parseError.errorString()));
                    return;
                }

                onSuccess(document);
            });
}

void JsonPlaceholderClient::setPendingRequests(const int count)
{
    const bool wasBusy = isBusy();
    m_pendingRequests = count;
    if (wasBusy != isBusy())
        emit busyChanged(isBusy());
}
