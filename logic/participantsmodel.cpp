#include "participantsmodel.h"

ParticipantsModel::ParticipantsModel(QObject* parent) : QAbstractListModel(parent) {}

int ParticipantsModel::rowCount(const QModelIndex &index) const
{
    return participants.size();
}

int ParticipantsModel::columnCount(const QModelIndex &index) const
{
    return 3;
}

QVariant ParticipantsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return {};
    auto& participant = participants[index.row()];

    switch(role) {
    case IdRole: return participant->id();
    case UsernameRole: return participant->username();
    case IsLeaderRole: return participant->isLeader();
    };

    return {};
}

QHash<int, QByteArray> ParticipantsModel::roleNames() const
{
    return {
        {IdRole, "id"},
        {UsernameRole, "username"},
        {IsLeaderRole, "isLeader"}
    };
}

void ParticipantsModel::pushParticipant(Participant* p)
{
    int size = participants.size();
    beginInsertRows(QModelIndex(), size, size);
    participants.push_back(p);
    endInsertRows();
}

void ParticipantsModel::removeParticipant(const QString &uid)
{
    int row = -1;
    Participant* erased = nullptr;

    for (int i = 0; i < participants.size(); ++i) {
        if (participants[i]->id() == uid) {
            row = i;
            erased = participants[i];
            break;
        }
    }

    if (row == -1 || !erased)
        return;

    beginRemoveRows(QModelIndex(), row, row);

    participants.removeAt(row);
    erased->deleteLater();

    endRemoveRows();
}

void ParticipantsModel::clear()
{
    if (participants.empty())
        return;

    beginRemoveRows(QModelIndex(), 0, participants.size() - 1);

    qDeleteAll(participants);
    participants.clear();

    endRemoveRows();
}

void ParticipantsModel::setLeader(const QString &leaderUid)
{
    for (int i = 0; i < participants.size(); ++i) {
        auto* p = participants[i];
        if (p->id() == leaderUid) {
            p->setIsLeader(true);

            QModelIndex idx = index(i, 0);
            emit dataChanged(idx, idx, {IsLeaderRole});
        } else
            p->setIsLeader(false);
    }
}

ParticipantsModel::~ParticipantsModel() {}
