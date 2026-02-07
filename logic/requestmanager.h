#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>
#include <QJsonObject>
#include <QHash>
#include <QUuid>
// #include <QDateTime>

#define TIMEOUT 5000
#define MAX_CONNECTION_ATTEMPTS 2

class Request;

class RequestManager : public QObject
{
    Q_OBJECT
public:
    explicit RequestManager(const QUrl& url, QObject* parent = nullptr);
    ~RequestManager();

    void connectOnceMore();

    Request& request(QJsonObject& json, const int& timeoutMs);

    bool connected() const { return _socket->state() == QAbstractSocket::ConnectedState; }
signals:
    void connectionFailed();
    void connectionChanged();
    void messageReceived(const QString& message);
private slots:
    void onMessage(const QString& message);
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
