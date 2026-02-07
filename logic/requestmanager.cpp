#include "requestmanager.h"
#include <QJsonDocument>

// Request

Request::Request(int timeoutMs, QObject* parent)
    : QObject(parent),
    _rid(QUuid::createUuid().toString(QUuid::WithoutBraces))
{
    timeoutTimer.setSingleShot(true);

    connect(&timeoutTimer, &QTimer::timeout, this, [this]() {
        emit requestFailed(QJsonObject{
            { "cmd", "error" }, { "error_message", "Request expired" }
        });
    });

    timeoutTimer.start(timeoutMs);
}

Request::~Request()
{
    timeoutTimer.stop();
}

Request& Request::then(std::function<void(const QJsonObject&)> handler)
{
    connect(this, &Request::requestDone, this,
            [handler](const QJsonObject& json) {
                handler(json);
            },
            Qt::SingleShotConnection);
    return *this;
}

Request& Request::catchError(std::function<void (const QJsonObject &)> handler)
{
    connect(this, &Request::requestFailed, this,
            [handler](const QJsonObject& json) {
                handler(json);
            },
            Qt::SingleShotConnection);

    return *this;
}

//REQUEST MANAGER

RequestManager::RequestManager(const QUrl& url, QObject *parent)
    : QObject(parent),
    _socket(new QWebSocket())
{
    _socket->setParent(this);

    connect(_socket, &QWebSocket::stateChanged, this, [this]() {
        emit connectionChanged();
    });

    connect(_socket, &QWebSocket::sslErrors, this, [this](const QList<QSslError> &errors) {
        for (const auto &error : errors) {
            qDebug() << "SSL Error:" << error.errorString() << "\n";
        }
        _socket->ignoreSslErrors();
    });

    connect(_socket, &QWebSocket::connected, this, [this]() {
        if (connectionTimer.isActive()) {
            connectionTimer.stop();
        }
        connectionAttempts = 0;
    });

    connect(_socket, &QWebSocket::textMessageReceived, this, &RequestManager::onMessage);

    connect(_socket, &QWebSocket::disconnected, this, [this]() {
        qDebug() << "Socket disconnected, reconnecting...\n";
        connectionTimer.start();
    });

    connectionTimer.setInterval(2000);
    connect(&connectionTimer, &QTimer::timeout, this, [this, url]() {
        if (connectionAttempts >= MAX_CONNECTION_ATTEMPTS) {
            emit connectionFailed();
            qWarning() << "Maximum connection attempts reached:" << MAX_CONNECTION_ATTEMPTS << "\n";
        } else {
            _socket->open(url);
            connectionAttempts++;
            qDebug() << "Connection attempt:" << connectionAttempts << "\n";
        }
    });

    _socket->open(url);
    connectionTimer.start();
}

RequestManager::~RequestManager()
{
    qDeleteAll(pendingRequests);
    pendingRequests.clear();
}

void RequestManager::connectOnceMore()
{
    connectionAttempts = 0;
}

Request& RequestManager::request(QJsonObject& json, const int& timeoutMs)
{
    auto* req = new Request(timeoutMs, this);

    pendingRequests.insert(req->rid(), req);
    json["requestId"] = req->rid();
    json["timestamp"] = QDateTime::currentMSecsSinceEpoch();

    connect(req, &Request::requestFailed, this,
            [this, req](const QJsonObject& error) {
                qDebug() << "Request failed:" << error << "\n";
                pendingRequests.remove(req->rid());
                req->deleteLater();
            });

    connect(req, &Request::requestDone, this,
            [this, req](const QJsonObject&) {
                pendingRequests.remove(req->rid());
                req->deleteLater();
            });

    _socket->sendTextMessage(QJsonDocument(json).toJson(QJsonDocument::Compact));

    return *req;
}

void RequestManager::onMessage(const QString &message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) {
        qDebug() << "Invalid JSON received";
        return;
    }

    const QJsonObject root = doc.object();

    if (root["type"] == "message") {
        emit messageReceived(message);
        return;
    }

    const QString rid = root["requestId"].toString();

    if (rid.isEmpty()) {
        qDebug() << "Message without requestId ignored\n";
        return;
    }

    if (pendingRequests.contains(rid)) {
        if (root["cmd"] != "error")
            emit pendingRequests[rid]->requestDone(root);
        else
            emit pendingRequests[rid]->requestFailed(root);
    }
}
