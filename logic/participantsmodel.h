#ifndef PARTICIPANTSMODEL_H
#define PARTICIPANTSMODEL_H

#include <QObject>
#include <QAbstractTableModel>

class Participant : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString username READ username NOTIFY usernameChanged FINAL)
    Q_PROPERTY(bool isLeader READ isLeader NOTIFY isLeaderChanged FINAL)
public:
    Participant(QObject* parent = nullptr) : QObject(parent) {}
    ~Participant() {}

    QString id() const { return uid; }
    QString username() const { return _username; }
    bool isLeader() const { return _isLeader; }

    void setUid(const QString& uid) { this->uid = uid; }
    void setUsername(const QString& username) { _username = username; emit usernameChanged(); }
    void setIsLeader(const bool& isLeader) { _isLeader = isLeader; emit isLeaderChanged(); }
signals:
    void usernameChanged();
    void isLeaderChanged();
private:
    QString uid;
    QString _username;
    bool _isLeader;
};

class ParticipantsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        UsernameRole,
        IsLeaderRole
    };

    ParticipantsModel(QObject* parent = nullptr);
    ~ParticipantsModel();

    int rowCount(const QModelIndex& index = QModelIndex()) const override;
    int columnCount(const QModelIndex& index = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void pushParticipant(Participant* p);
    void removeParticipant(const QString& uid);
    void clear();
    void setLeader(const QString& leaderUid);
private:
    QVector<Participant*> participants;
};

#endif // PARTICIPANTSMODEL_H
