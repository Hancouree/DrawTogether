#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>
#include <QJsonObject>
#include <QHash>
#include <QUuid>

#define TIMEOUT 15000
#define MAX_CONNECTION_ATTEMPTS 5

class Request;

class RequestManager : public QObject
{
    Q_OBJECT
public:
    explicit RequestManager(const QUrl& url, QObject* parent = nullptr);
    ~RequestManager();

    void connectOnceMore();
    Request& request(QJsonObject& json, const int& timeoutMs);
    void sendBinaryMessage(const QByteArray& message);
    bool connected() const noexcept { return _socket->state() == QAbstractSocket::ConnectedState; }
signals:
    void connectionFailed();
    void connectionChanged();
    void messageReceived(const QString& message);
    void binaryMessageReceived(const QByteArray& message);
private slots:
    void onMessage(const QString& message);
    void onBinaryMessage(const QByteArray& message);
private:
    QWebSocket* _socket = nullptr;
    QHash<QString, Request*> pendingRequests;
    QTimer connectionTimer;
    int connectionAttempts = 0;
};

// Request

class Request : public QObject
{
    Q_OBJECT
public:
    explicit Request(int timeoutMs, QObject* parent = nullptr);
    ~Request() override;

    Request& then(std::function<void(const QJsonObject&)> handler);
    Request& catchError(std::function<void(const QJsonObject&)> handler);
    [[nodiscard]] QString rid() const { return _rid; }

signals:
    void requestDone(const QJsonObject& answer);
    void requestFailed(const QJsonObject& error);

private:
    QString _rid;
    QTimer timeoutTimer;
};

#endif // REQUESTMANAGER_H
