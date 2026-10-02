# 07 – Network access with QNetworkAccessManager

A small REST client for [jsonplaceholder.typicode.com](https://jsonplaceholder.typicode.com/), a free fake API.

- On start, the 100 posts are loaded (`GET /posts`) and listed on the left.
- Selecting a post loads its comments (`GET /comments?postId=N`) and shows them on the right.
- **New Post…** sends a `POST /posts`. The server answers with the created post (always id 101, since
  jsonplaceholder does not really store anything).
- A busy indicator in the status bar shows running requests, and errors appear in the status bar.

## Architecture

```
MainWindow ──calls──▶ JsonPlaceholderClient ──owns──▶ QNetworkAccessManager
    ▲                         │
    └──── typed signals ──────┘   postsReceived(QList<Post>), commentsReceived(...),
                                  postCreated(Post), errorOccurred(QString), busyChanged(bool)
```

The UI never sees `QNetworkReply` or JSON. `JsonPlaceholderClient` turns HTTP into typed C++ values
(`Post`, `Comment` in `Post.h`), so the window only deals with domain data.

## The request lifecycle

Qt networking is **asynchronous**: `get()` / `post()` return immediately with a `QNetworkReply`, and the result
arrives later through signals. All replies go through one function:

```cpp
void JsonPlaceholderClient::handleReply(QNetworkReply *reply, JsonHandler onSuccess)
{
    connect(reply, &QNetworkReply::finished, this, [this, reply, onSuccess] {
        reply->deleteLater();                          // 1. always release the reply
        if (reply->error() != QNetworkReply::NoError) { // 2. network / HTTP errors
            emit errorOccurred(reply->errorString());
            return;
        }
        const auto document = QJsonDocument::fromJson(reply->readAll(), &parseError);
        ...                                            // 3. JSON errors
        onSuccess(document);                           // 4. request-specific parsing
    });
}
```

Points worth noting:

- **The caller owns the reply.** It must be deleted, and `deleteLater()` is the safe way to do it from inside its
  own signal.
- **Timeouts**: `QNetworkAccessManager::setTransferTimeout()` aborts requests that stall. They then end with
  an error like any other.
- **One manager per application part** is enough. It queues and reuses connections.
- **Out-of-order replies**: if the user clicks through posts quickly, comments for an older post can arrive
  late. `commentsReceived` carries the `postId`, and the window ignores replies for posts that are no longer
  selected.

## JSON

`QJsonDocument::fromJson()` parses the body. `Post::fromJson()` / `Comment::fromJson()` map a `QJsonObject` to a
struct, and `Post::toJson()` builds the request body for POST. Missing fields become default values
(`0`, empty string).

## HTTPS on Windows

Qt 6 loads TLS through plugins. On Windows the `schannel` backend uses the system's TLS stack and certificate
store, so no OpenSSL is needed. `windeployqt` copies the `tls/` plugin folder next to the executable. If HTTPS
fails with "TLS initialization failed", that folder is missing.

## Alternatives

Since Qt 6.7 there is `QRestAccessManager` / `QRestReply`, a thin layer over `QNetworkAccessManager` with
helpers for JSON and HTTP status handling. This example uses the classic API because it works in every Qt 6
version and shows what happens underneath.

## Files

```
src/
  Post.h                         Post / Comment + JSON mapping
  JsonPlaceholderClient.h/.cpp   all networking
  PostListModel.h/.cpp           list model for the posts
  NewPostDialog.h/.cpp           input for a new post
  MainWindow.h/.cpp              UI
  main.cpp
```
