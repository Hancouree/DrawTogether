#ifndef ROOMINFO_H
#define ROOMINFO_H

#include "participantsmodel.h"

class RoomInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString rid READ rid NOTIFY ridChanged FINAL)
    Q_PROPERTY(bool isLeader READ isLeader NOTIFY isLeaderChanged FINAL)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged FINAL)
    Q_PROPERTY(int maxCapacity READ maxCapacity NOTIFY maxCapacityChanged FINAL)
    Q_PROPERTY(int participantsCount READ participantsCount NOTIFY participantsCountChanged FINAL)
    Q_PROPERTY(ParticipantsModel* participants READ participants CONSTANT)
public:
    RoomInfo(QObject* parent = nullptr);
    ~RoomInfo();

    //GETTERS
    QString rid() const { return _rid; }
    QString name() const { return _name; }
    bool isLeader() const { return _isLeader; }
    int maxCapacity() const { return _maxCapacity; }
    int participantsCount() const { return model->rowCount(); }
    ParticipantsModel* participants() const { return model; }

    //SETTERS
    void setRid(const QString& rid) { _rid = rid; emit ridChanged(); }
    void setName(const QString& name) { _name = name; emit nameChanged(); }
    void setMaxCapacity(const unsigned int maxCapacity) { _maxCapacity = maxCapacity; emit maxCapacityChanged(); }
    void setIsLeader(const bool& l) { _isLeader = l; emit isLeaderChanged(); }
    void pushParticipant(const QString& uid, const QString& username, const bool& isLeader);
signals:
    void ridChanged();
    void nameChanged();
    void maxCapacityChanged();
    void participantsCountChanged();
    void isLeaderChanged();
private:
    QString _rid;
    QString _name;
    unsigned int _maxCapacity;
    bool _isLeader;
    ParticipantsModel* model;
};

#endif // ROOMINFO_H
