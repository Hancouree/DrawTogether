#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>

#define MAX_CONNECTION_ATTEMPTS 2

class Logic : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectionChanged FINAL)
    Q_PROPERTY(QString username READ username NOTIFY usernameChanged FINAL)
public:
    Logic(QObject* parent = nullptr);
    ~Logic();

    bool connected() const { return _socket->state() == QAbstractSocket::ConnectedState; }
    QString username() const { return _username; }
public slots:
    void connectOnceMore();
signals:
    void connectionChanged();
    void connectionFailed();
    void usernameChanged();
private:
    QString _username;
    QWebSocket* _socket;
    QTimer connectionTimer;
    int connectionAttempts;
};

#endif // LOGIC_H
