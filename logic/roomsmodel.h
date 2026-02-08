#ifndef ROOMSMODEL_H
#define ROOMSMODEL_H

#include <QAbstractTableModel>

class RoomsModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged FINAL)
    Q_PROPERTY(bool canLoadMore READ canLoadMore NOTIFY canLoadMoreChanged FINAL)
public:
    struct Room {
        QString rid;
        QString name;
        QString createdAt;
        unsigned int maxCapacity;
        unsigned currentlyUsers;
    };

    RoomsModel(QObject* parent = nullptr);
    ~RoomsModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int count() const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool loading() const { return _loading; }
    bool canLoadMore() const { return _rooms.size() < total; }

    void pushRoom(const Room& room);
    void setLoading(const bool& loading);
    void setTotal(const unsigned int& total);
    void clear();
signals:
    void loadingChanged();
    void canLoadMoreChanged();
private:
    enum RoomsRoles {
        RidRole = Qt::UserRole + 1,
        NameRole,
        CreatedAtRole,
        MaxCapacityRole,
        CurrentlyUsersRole
    };
    Q_ENUM(RoomsRoles)

    QVector<Room> _rooms;
    bool _loading;
    unsigned int total;
};

#endif // ROOMSMODEL_H
