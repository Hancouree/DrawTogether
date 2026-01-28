#ifndef ROOMSMODEL_H
#define ROOMSMODEL_H

#include <QAbstractTableModel>

class RoomsModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged FINAL)
public:
    struct Room {
        QString rid;
        QString name;
        QString createdAt; // add it to the rolenames
        bool isCreatedByMe;
    };

    RoomsModel(QObject* parent = nullptr);
    ~RoomsModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int count() const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool loading() const { return _loading; }
    unsigned int offset() const { return _offset; }

    void pushRoom(const Room& room);
    void setLoading(const bool& loading);
    void setOffset(const unsigned int& offset);
    void clear();
signals:
    void loadingChanged();
private:
    enum RoomsRoles {
        RidRole = Qt::UserRole + 1,
        NameRole,
        IsCreatedByMeRole
    };
    Q_ENUM(RoomsRoles)

    QVector<Room> _rooms;
    bool _loading;
    unsigned int _offset;
};

#endif // ROOMSMODEL_H
