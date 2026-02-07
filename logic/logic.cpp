#include "logic.h"

QString toTimeAgo(qint64 timestamp) {
    auto now = QDateTime::currentDateTime();
    auto then = QDateTime::fromMSecsSinceEpoch(timestamp);
    qint64 secs = qAbs(then.secsTo(now));
    QString suffix = (timestamp < now.toMSecsSinceEpoch()) ? "ago" : "later";

    if (secs < 60) return "just now";
    if (secs < 3600) return QString("%1 m. %2").arg(secs/60).arg(suffix);
    if (secs < 86400) return QString("%1 h. %2").arg(secs/3600).arg(suffix);
    if (secs < 2592000) return QString("%1 d. %2").arg(secs/86400).arg(suffix);
    if (secs < 31536000) return QString("%1 mo. %2").arg(secs/2592000).arg(suffix);
    return QString("%1 y. %2").arg(secs/31536000).arg(suffix);
}

Logic::Logic(QObject *parent)
    : QObject(parent),
    _requestManager(new RequestManager(QUrl(("wss://26.209.218.198:5050")), this)),
    _roomsModel(new RoomsModel(this)),
    _roomInfo(new RoomInfo(this)),
    _notificationManager(new NotificationManager(this))
{
    connect(_requestManager, &RequestManager::connectionChanged, this, [this](){ emit connectionChanged(); });
    connect(_requestManager, &RequestManager::connectionFailed, this, [this](){ emit connectionFailed(); });
    connect(_requestManager, &RequestManager::messageReceived, this, &Logic::onMessageReceived);
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
    emit usernameChanged();
}

void Logic::findRooms()
{
    if (_roomsModel->count() > 0)
        _roomsModel->clear();

    QJsonObject json;
    json["cmd"] = "find_rooms";
    json["limit"] = LIMIT;
    json["offset"] = _roomsModel->count();

    _requestManager->request(json, TIMEOUT)
        .then([this](const QJsonObject& answer) {
            qDebug() << "ANSWER FIND ROOMS: " << answer << "\n";

            const QJsonArray rooms = answer["rooms"].toArray();
            for (const QJsonValue& room : rooms) {

                const QString& rid = room["rid"].toString(), timeAgo = toTimeAgo(room["created_at"].toInteger()),
                    name = room["name"].toString();
                const int currentlyUsers = room["currently_users"].toInt(), maxCapacity = room["max_capacity"].toInt();

                _roomsModel->pushRoom(RoomsModel::Room({ rid, name, timeAgo, (unsigned int)maxCapacity, (unsigned int)currentlyUsers }));
            }

            _roomsModel->setLoading(false);
        })
        .catchError([this](const QJsonObject& error) {
            _notificationManager->error(error["error_message"].toString());
            _roomsModel->setLoading(false);
        });
    _roomsModel->setLoading(true);
    _state.applyEvent(FSM::SEARCH_ROOMS);
}

void Logic::undoTransition()
{
    _state.applyEvent(FSM::BACK);
}

void Logic::openRoomCreationPage()
{
    _state.applyEvent(FSM::CREATE_ROOM);
}

void Logic::createRoom(const QString& roomName, const int& maxCapacity)
{
    QJsonObject json;
    json["cmd"] = "create_room";
    json["name"] = roomName;
    json["username"] = _username;
    json["max_capacity"] = maxCapacity;

    _requestManager->request(json, TIMEOUT)
        .then([this](const QJsonObject& answer) {
            const QString myUid = answer["uid"].toString();

            _roomInfo->setRid(answer["rid"].toString());
            _roomInfo->setName(answer["name"].toString());
            _roomInfo->setMaxCapacity(answer["maxCapacity"].toInt());
            _roomInfo->setMyUid(myUid);
            _roomInfo->setLeaderUid(myUid);

            parseUsers(answer["users"].toArray());

            _state.applyEvent(FSM::JOINED_ROOM);
        })
        .catchError([this](const QJsonObject& error) {
            _notificationManager->error(error["error_message"].toString());
        });
}

void Logic::joinRoom(const QString &rid)
{
    QJsonObject json;
    json["cmd"] = "join_room";
    json["username"] = _username;
    json["rid"] = rid;

    _requestManager->request(json, TIMEOUT)
        .then([this](const QJsonObject& answer) {
            _roomInfo->setRid(answer["rid"].toString());
            _roomInfo->setName(answer["name"].toString());
            _roomInfo->setMaxCapacity(answer["maxCapacity"].toInt());
            _roomInfo->setMyUid(answer["uid"].toString());
            _roomInfo->setLeaderUid(answer["leaderUid"].toString());

            parseUsers(answer["users"].toArray());

            _state.applyEvent(FSM::JOINED_ROOM);
        })
        .catchError([this](const QJsonObject& error) {
            _notificationManager->error(error["error_message"].toString());
        });
}

void Logic::leaveRoom()
{
    QJsonObject json;
    json["cmd"] = "leave_room";
    json["rid"] = _roomInfo->rid();

    _requestManager->request(json, TIMEOUT)
        .then([this](const QJsonObject& answer) {
            qDebug() << answer << "\n";
            if (answer["ok"].toBool()) {
                _roomInfo->clearParticipants();
                _state.applyEvent(FSM::LEFT_ROOM);
            }
            else
                _notificationManager->error("Something occurred, try once more");
        })
        .catchError([this](const QJsonObject error) {
            _notificationManager->error(error["error_message"].toString());
        });
}

void Logic::onMessageReceived(const QString &message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (doc.isNull()) {
        qDebug() << "Message contains an invalid JSON\n";
        return;
    }

    QJsonObject root = doc.object();

    const QString& request = root["cmd"].toString();

    QVector<std::function<bool()>> handlers = {
        [this, &request, &root]() { return onUserJoined(request, root); },
        [this, &request, &root]() { return onUserLeft(request, root); },
        [this, &request, &root]() { return onLeaderChanged(request, root); }
    };

    for(auto& handler : handlers) {
        if (handler())
            return;
    }

    qDebug() << "Request was not processed: " << message << "\n";
}

bool Logic::onUserJoined(const QString &request, QJsonObject &root)
{
    if (request != "user_joined")
        return false;

    const QString rid = root["rid"].toString();
    if (rid == _roomInfo->rid()) {
        auto user = root["user"].toObject();
        _roomInfo->pushParticipant(
            user["uid"].toString(),
            user["username"].toString(),
            user["isLeader"].toBool()
        );
        _notificationManager->notification("User joined!");
    }

    return true;
}

bool Logic::onUserLeft(const QString &request, QJsonObject &root)
{
    if (request != "user_left")
        return false;

    const QString rid = root["rid"].toString();
    if (rid == _roomInfo->rid()) {
        _roomInfo->removeParticipant(root["leftUid"].toString());
        _notificationManager->notification("User left!");
    }

    return true;
}

bool Logic::onLeaderChanged(const QString &request, QJsonObject &root)
{
    if (request != "leader_changed")
        return false;

    const QString rid = root["rid"].toString();
    if (_roomInfo->rid() == rid) {
        _roomInfo->setLeader(root["leaderUid"].toString());
    }

    return true;
}

void Logic::parseUsers(const QJsonArray &users)
{
    for (const QJsonValue& user : users) {
        _roomInfo->pushParticipant(
            user["uid"].toString(),
            user["username"].toString(),
            user["isLeader"].toBool()
        );
    }
}

Logic::~Logic() {}


