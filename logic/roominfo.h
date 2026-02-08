#ifndef ROOMINFO_H
#define ROOMINFO_H

#include "participantsmodel.h"

class RoomInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString rid READ rid NOTIFY ridChanged FINAL)
    Q_PROPERTY(bool isMeLeader READ isMeLeader NOTIFY isMeLeaderChanged FINAL)
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
    int maxCapacity() const { return _maxCapacity; }
    int participantsCount() const { return model->rowCount(); }
    QString uid() const { return myUid; }
    bool isMeLeader() const { return myUid == leaderUid; }
    ParticipantsModel* participants() const { return model; }

    //SETTERS
    void setRid(const QString& rid) { _rid = rid; emit ridChanged(); }
    void setName(const QString& name) { _name = name; emit nameChanged(); }
    void setMaxCapacity(const unsigned int maxCapacity) { _maxCapacity = maxCapacity; emit maxCapacityChanged(); }    
    void setLeaderUid(const QString& leaderUid) { this->leaderUid = leaderUid; emit isMeLeaderChanged(); }
    void setMyUid(const QString& myUid) { this->myUid = myUid; }
    void setLeader(const QString& newLeaderUid);

    void pushParticipant(const QString& uid, const QString& username, const bool& isLeader);
    void removeParticipant(const QString& uid);
    void clearParticipants();
signals:
    void ridChanged();
    void nameChanged();
    void maxCapacityChanged();
    void participantsCountChanged();
    void isMeLeaderChanged();
private:
    QString _rid;
    QString myUid, leaderUid;
    QString _name;
    unsigned int _maxCapacity;
    ParticipantsModel* model;
};

#endif // ROOMINFO_H
