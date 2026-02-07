#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QObject>
#include <QTimer>

class Notification : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged FINAL)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged FINAL)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged FINAL)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged FINAL)
    Q_PROPERTY(QString color READ color NOTIFY colorChanged FINAL)
public:
    explicit Notification(int durationMs = 3000, QObject *parent = nullptr);
    ~Notification();

    QString title() const { return _title; }
    QString message() const { return _message; }
    bool visible() const { return _visible; }
    qreal progress() const { return _progress; }
    QString color() const { return _color; }

    void setTitle(const QString &t);
    void setMessage(const QString &m);
    void setVisible(const bool& v);
    void setColor(const QString& c);
signals:
    void titleChanged();
    void messageChanged();
    void visibleChanged();
    void colorChanged();
    void progressChanged();
private:
    QString _title;
    QString _message;
    bool _visible = true;
    QString _color;
    qreal _progress = 1.0;
    int _duration;
    QTimer _timer;
};

#endif
