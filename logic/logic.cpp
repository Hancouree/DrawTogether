#include "logic.h"

Logic::Logic(QObject *parent)
    : QObject(parent), _requestManager(new RequestManager(QUrl(("wss://26.209.218.198:5050")), this)), _roomsModel(new RoomsModel(this))
{
    connect(_requestManager, &RequestManager::connectionChanged, this, [this](){ emit connectionChanged(); });
    connect(_requestManager, &RequestManager::connectionFailed, this, [this](){ emit connectionFailed(); });
    connect(&_state, &FSM::stateChanged, this, [this]() { emit stateChanged(); });
}

void Logic::connectOnceMore()
{
    _requestManager->connectOnceMore();
}

void Logic::setUsername(const QString &username)
{
    _username = username;
    _state.applyEvent(FSM::USERNAME_ESTABLISHED);
}

void Logic::findRooms()
{
    if (_roomsModel->count() > 0)
        _roomsModel->clear();

    QJsonObject root;
    root["cmd"] = "find_rooms";
    root["limit"] = 10;
    root["offset"] = (int)_roomsModel->offset();
    root["timestamp"] = QDateTime::currentMSecsSinceEpoch();

    _requestManager->request(root, 5000)
        .then([this](const QJsonObject& answer) {
            qDebug() << "Hello: " << answer << '\n';
            _roomsModel->setLoading(false);
        })
        .catchError([this](const QJsonObject& error) { _roomsModel->setLoading(false); });
    _roomsModel->setLoading(true);
    _state.applyEvent(FSM::SEARCH_ROOMS);
}

void Logic::undoTransition()
{
    _state.applyEvent(FSM::BACK);
}

void Logic::createRoom()
{
    qDebug() << "Create room called\n";
}

Logic::~Logic() {}


