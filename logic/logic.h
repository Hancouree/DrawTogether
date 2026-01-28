#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject> //mayber incapsulate it later to functions
#include "fsm.h"
#include "requestmanager.h"
#include "roomsmodel.h"

class Logic : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectionChanged FINAL)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
    Q_PROPERTY(int state READ state NOTIFY stateChanged FINAL)
    Q_PROPERTY(RoomsModel* roomsModel READ roomsModel CONSTANT)
public:
    Logic(QObject* parent = nullptr);
    ~Logic();

    bool connected() const { return _requestManager->connected(); }
    QString username() const { return _username; }
    int state() const { return _state.state(); }
    RoomsModel* roomsModel() const { return _roomsModel; }
public slots:
    void setUsername(const QString& username);
    void connectOnceMore();
    void findRooms();
    void undoTransition();
    void createRoom();
signals:
    void connectionChanged();
    void connectionFailed();
    void usernameChanged();
    void stateChanged();
private:
    QString _username;
    RequestManager* _requestManager;
    FSM _state;
    RoomsModel* _roomsModel;
};

#endif // LOGIC_H
