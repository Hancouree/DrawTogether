#ifndef LOGIC_H
#define LOGIC_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject> //mayber incapsulate it later to functions
#include <QPointF>
#include <QColor>
#include "fsm.h"
#include "requestmanager.h"
#include "roomsmodel.h"
#include "roominfo.h"
#include "notificationmanager.h"

#define LIMIT 20

class Logic : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectionChanged FINAL)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
    Q_PROPERTY(FSM::States state READ state NOTIFY stateChanged FINAL)
    Q_PROPERTY(RoomsModel* roomsModel READ roomsModel CONSTANT)
    Q_PROPERTY(RoomInfo* roomInfo READ roomInfo CONSTANT)
    Q_PROPERTY(NotificationManager* notificationManager READ notificationManager CONSTANT)
public:
    Logic(QUrl url, QObject* parent = nullptr);
    ~Logic();

    bool connected() const { return _requestManager->connected(); }
    QString username() const { return _username; }
    FSM::States state() const { return _state.state(); }
    RoomsModel* roomsModel() const { return _roomsModel; }
    RoomInfo* roomInfo() const { return _roomInfo; }
    NotificationManager* notificationManager() const { return _notificationManager; }
public slots:
    void setUsername(const QString& username);
    void connectOnceMore();
    void findRooms(bool refresh = true);
    void undoTransition();
    void openRoomCreationPage();
    void createRoom(const QString& roomName, const int& maxCapacity);
    void joinRoom(const QString& rid);
    void leaveRoom();
    void kickUser(const QString& uid);
    void startRoom();
    void sendPoints(const QList<QPointF>& batchedPoints, const QColor& color, const int& penWidth);
    void finishGame();
private slots:
    void onMessageReceived(const QString& message);
    bool onUserJoined(const QString &request, QJsonObject &root);
    bool onUserLeft(const QString& request, QJsonObject& root);
    bool onKick(const QString& request, QJsonObject& root);
    bool onLeaderChanged(const QString& request, QJsonObject& root);
    bool onStartRoom(const QString& request, QJsonObject& root);
    bool onPointsReceived(const QString& request, QJsonObject& root);
    bool onFinishGame(const QString& request, QJsonObject& root);
signals:
    void connectionChanged();
    void connectionFailed();
    void usernameChanged();
    void stateChanged();
    void pointsBatched(const QList<QPointF>& points, const QColor& color, const int& width);
private:
    void parseUsers(const QJsonArray& users);

    QString _username;
    RequestManager* _requestManager;
    FSM _state;
    RoomsModel* _roomsModel;
    RoomInfo* _roomInfo;
    NotificationManager* _notificationManager;
};

#endif // LOGIC_H
