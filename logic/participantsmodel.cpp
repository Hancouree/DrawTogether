#include "participantsmodel.h"

ParticipantsModel::ParticipantsModel(QObject* parent) : QAbstractTableModel(parent) {}

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

void ParticipantsModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, participants.size() - 1);
    participants.clear();
    endInsertRows();
}

ParticipantsModel::~ParticipantsModel() {}
