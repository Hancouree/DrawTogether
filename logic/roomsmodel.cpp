#include "roomsmodel.h"

RoomsModel::RoomsModel(QObject* parent) : QAbstractTableModel(parent), _loading(false), total(0) {}

int RoomsModel::rowCount(const QModelIndex &parent) const
{
    return _rooms.size();
}

int RoomsModel::count() const
{
    return rowCount();
}

int RoomsModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QHash<int, QByteArray> RoomsModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames[RidRole] = "rid";
    roleNames[NameRole] = "name";
    roleNames[CreatedAtRole] = "createdAt";
    roleNames[MaxCapacityRole] = "maxCapacity";
    roleNames[CurrentlyUsersRole] = "currentlyUsers";

    return roleNames;
}
QVariant RoomsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount() || index.column() >= columnCount())
        return QVariant();

    const auto& row = _rooms[index.row()];

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0:
            return row.rid;
        case 1:
            return row.name;
        case 2:
            return row.createdAt;
        case 3:
            return row.maxCapacity;
        case 4:
            return row.currentlyUsers;
        default:
            return QVariant();
        }
        break;

    case RidRole:
        return row.rid;

    case NameRole:
        return row.name;

    case CreatedAtRole:
        return row.createdAt;

    case MaxCapacityRole:
        return row.maxCapacity;

    case CurrentlyUsersRole:
        return row.currentlyUsers;

    default:
        return QVariant();
    }
}

void RoomsModel::pushRoom(const Room &room)
{
    const int size = _rooms.size();
    beginInsertRows(QModelIndex(), size, size);
    _rooms.push_back(room);
    endInsertRows();
    emit canLoadMoreChanged();
}

void RoomsModel::setLoading(const bool &loading)
{
    _loading = loading;
    emit loadingChanged();
}

void RoomsModel::setTotal(const unsigned int &total)
{
    this->total = total;
    emit canLoadMoreChanged();
}

void RoomsModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, _rooms.size() - 1);
    _rooms.clear();
    endRemoveRows();
    emit canLoadMoreChanged();
}

RoomsModel::~RoomsModel() {}
