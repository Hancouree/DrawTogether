#include "notificationmanager.h"

NotificationManager::NotificationManager(QObject *parent) : QObject(parent) {}

void NotificationManager::error(const QString &message, const QString &title)
{
    auto *n = new Notification(3000, this);
    n->setTitle(title);
    n->setMessage(message);

    connect(n, &Notification::visibleChanged, this, [this, n]() {
        if (!n->visible()) {
            _notifications.removeOne(n);
            emit notificationsChanged();
            n->deleteLater();
        }
    });

    _notifications.append(n);
    emit notificationsChanged();
}

NotificationManager::~NotificationManager() {}
