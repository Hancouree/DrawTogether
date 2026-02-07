#include "roominfo.h"

RoomInfo::RoomInfo(QObject* parent) : QObject(parent), model(new ParticipantsModel(this)), _isLeader(false) {}

void RoomInfo::pushParticipant(const QString &uid, const QString &username, const bool &isLeader)
{
    Participant* p = new Participant(this);

    p->setUid(uid);
    p->setUsername(username);
    p->setIsLeader(isLeader);

    model->pushParticipant(p);
    emit participantsCountChanged();
}

RoomInfo::~RoomInfo() {}

