#include "logic.h"

Logic::Logic(QObject *parent) : QObject(parent), _socket(new QWebSocket), connectionAttempts(0)
{
    _socket->setParent(this);
    connect(_socket, &QWebSocket::stateChanged, this, [this]() {
        emit connectionChanged();
    });
    connect(_socket, &QWebSocket::sslErrors, this, [this](const QList<QSslError> errors) {
        for (const auto& error : errors)
            qDebug() << error.errorString() << '\n';
        _socket->ignoreSslErrors();
    });
    connect(_socket, &QWebSocket::connected, this, [this]() {
        if (connectionTimer.isActive()) {
            connectionTimer.stop();
            connectionAttempts = 0;
        }
    });
    connect(_socket, &QWebSocket::disconnected, this, [this]() {
        qDebug() << "Socket disconnected, reconnecting...\n";
        connectionTimer.start();
    });

    connectionTimer.setInterval(2000);
    connect(&connectionTimer, &QTimer::timeout, this, [this]() {
        if (connectionAttempts >= MAX_CONNECTION_ATTEMPTS)
            emit connectionFailed();
        else {
            _socket->open(QUrl("wss://26.209.218.198:5050"));
            connectionAttempts++;
            qDebug() << "Connection attempt: " << connectionAttempts << '\n';
        }
    });

    _socket->open(QUrl("wss://26.209.218.198:5050"));
    connectionTimer.start();
}

void Logic::connectOnceMore()
{
    connectionAttempts = 0;
}

Logic::~Logic() {}

