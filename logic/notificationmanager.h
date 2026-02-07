#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include "notification.h"

class NotificationManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<Notification*> notifications READ notifications NOTIFY notificationsChanged FINAL)
public:
    explicit NotificationManager(QObject *parent = nullptr);
    ~NotificationManager();

    QList<Notification*> notifications() const { return _notifications; }

    Q_INVOKABLE void error(const QString &message, const QString &title = "Error");
    Q_INVOKABLE void notification(const QString& message, const QString& title = "Notification");
signals:
    void notificationsChanged();
private:
    QList<Notification*> _notifications;
};

#endif
