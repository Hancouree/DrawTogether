#include "roominfo.h"

RoomInfo::RoomInfo(QObject* parent) : QObject(parent), model(new ParticipantsModel(this)) {}

void RoomInfo::pushParticipant(const QString &uid, const QString &username, const bool &isLeader)
{
    Participant* p = new Participant(this);

    p->setUid(uid);
    p->setUsername(username);
    p->setIsLeader(isLeader);

    model->pushParticipant(p);
    emit participantsCountChanged();
}

void RoomInfo::removeParticipant(const QString &uid)
{
    model->removeParticipant(uid);
    emit participantsCountChanged();
}

void RoomInfo::clearParticipants()
{
    model->clear();
    emit participantsCountChanged();
}

void RoomInfo::setLeader(const QString &newLeaderUid) {
    if (leaderUid != newLeaderUid) {
        leaderUid = newLeaderUid;
        model->setLeader(newLeaderUid);
        emit isMeLeaderChanged();
    }
}

RoomInfo::~RoomInfo() {}

