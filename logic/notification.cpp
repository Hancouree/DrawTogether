#include "notification.h"

Notification::Notification(int durationMs, QObject *parent) : QObject(parent),
    _duration(durationMs)
{
    _timer.setInterval(16);
    connect(&_timer, &QTimer::timeout, this, [this]() {
        _progress -= 16.0 / _duration;
        if (_progress <= 0.0) {
            _progress = 0.0;
            emit progressChanged();
            setVisible(false);
            _timer.stop();
            return;
        }
        emit progressChanged();
    });

    _timer.start();
}

void Notification::setTitle(const QString &t)
{
    if (_title == t) return;
    _title = t;
    emit titleChanged();
}

void Notification::setMessage(const QString &m)
{
    if (_message == m) return;
    _message = m;
    emit messageChanged();
}

void Notification::setVisible(const bool& v)
{
    if (_visible == v) return;
    _visible = v;
    emit visibleChanged();
}

Notification::~Notification() {}
