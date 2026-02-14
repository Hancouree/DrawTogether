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

void Logic::findRooms(bool refresh)
{
    if (_roomsModel->count() > 0 && refresh) _roomsModel->clear();

    QJsonObject json;
    json["cmd"] = "find_rooms";
    json["limit"] = LIMIT;
    json["offset"] = _roomsModel->count();

    _requestManager->request(json, TIMEOUT)
        .then([this](const QJsonObject& answer) {
            qDebug() << "FIND ROOMS: " << answer;
            _roomsModel->setTotal(answer["total"].toInt());

            const QJsonArray rooms = answer["rooms"].toArray();
            for (const QJsonValue& room : rooms) {

                const QString& rid = room["rid"].toString(), timeAgo = toTimeAgo(room["created_at"].toInteger()),
                    name = room["name"].toString();
                const int currentlyUsers = room["currently_users"].toInt(), maxCapacity = room["max_capacity"].toInt();
                const bool started = room["started"].toBool();

                _roomsModel->pushRoom(RoomsModel::Room({ rid, name, timeAgo, (unsigned int)maxCapacity, (unsigned int)currentlyUsers, started }));
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
            _roomInfo->clearParticipants();
            _state.applyEvent(FSM::LEFT_ROOM);
        })
        .catchError([this](const QJsonObject error) {
            _notificationManager->error(error["error_message"].toString());
        });
}

void Logic::kickUser(const QString &uid)
{
    QJsonObject json;
    json["cmd"] = "kick";
    json["rid"] = _roomInfo->rid();
    json["kickedUid"] = uid;

    _requestManager->request(json, TIMEOUT)
        .then([this, uid](const QJsonObject& answer) {
            _roomInfo->removeParticipant(uid);
            _notificationManager->notification("You've kicked the user");
        })
        .catchError([this](const QJsonObject& error) {
            _notificationManager->error(error["error_message"].toString());
        });
}

void Logic::startRoom()
{
    QJsonObject json;
    json["cmd"] = "start_game";
    json["rid"] = _roomInfo->rid();

    _requestManager->request(json, TIMEOUT)
        .then([this](const QJsonObject& answer) {
            _state.applyEvent(FSM::ROOM_STARTED);
        })
        .catchError([this](const QJsonObject& error) {
            _notificationManager->error(error["error_message"].toString());
        });
}

void Logic::sendPoints(const QList<QPointF> &batchedPoints, const QColor& color, const int& penWidth)
{
    QJsonObject json;

    QJsonArray points;
    for (auto& p : batchedPoints) {
        QJsonObject point;
        point["x"] = p.x();
        point["y"] = p.y();
        points.append(point);
    }

    json["cmd"] = "sync_drawing";
    json["rid"] = _roomInfo->rid();
    json["points"] = points;
    json["penWidth"] = penWidth;
    json["color"] = color.name();

    _requestManager->request(json, TIMEOUT)
        .catchError([this](const QJsonObject& error) {
            _notificationManager->error(error["error_message"].toString());
        });
}

void Logic::finishGame()
{
    QJsonObject json;
    json["cmd"] = "finish_game";
    json["rid"] = _roomInfo->rid();

    _requestManager->request(json, TIMEOUT)
        .then([this](const QJsonObject& answer) {
            _state.applyEvent(FSM::GAME_ENDED);
        })
        .catchError([this](const QJsonObject& error) {
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
        [this, &request, &root]() { return onPointsReceived(request, root); },
        [this, &request, &root]() { return onUserJoined(request, root); },
        [this, &request, &root]() { return onUserLeft(request, root); },
        [this, &request, &root]() { return onKick(request, root); },
        [this, &request, &root]() { return onStartRoom(request, root); },
        [this, &request, &root]() { return onFinishGame(request, root); },
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
    if (request != "user_left") return false;

    const QString rid = root["rid"].toString();
    if (rid == _roomInfo->rid()) {
        qDebug() << root;
        _roomInfo->removeParticipant(root["leftUid"].toString());
        _notificationManager->notification("User left!");
    }

    return true;
}

bool Logic::onKick(const QString &request, QJsonObject &root)
{
    if (request != "user_kicked") return false;

    if (_roomInfo->isMeLeader()) return true; //I've kicked already

    const QString rid = root["rid"].toString();
    if (rid == _roomInfo->rid()) {
        const QString kickedUid = root["kickedUid"].toString();
        if (_roomInfo->uid() == kickedUid) {
            _roomInfo->clearParticipants();
            _notificationManager->notification("The leader kicked you out");
            _state.applyEvent(FSM::KICKED);
        } else {
            _roomInfo->removeParticipant(kickedUid);
            _notificationManager->notification("User was kicked!");
        }
    }

    return true;
}

bool Logic::onLeaderChanged(const QString &request, QJsonObject &root)
{
    if (request != "leader_changed") return false;

    const QString rid = root["rid"].toString();
    if (_roomInfo->rid() == rid) {
        _roomInfo->setLeader(root["leaderUid"].toString());
    }

    return true;
}

bool Logic::onStartRoom(const QString &request, QJsonObject &root)
{
    if (request != "start_game")
        return false;

    const QString rid = root["rid"].toString();
    if (_roomInfo->rid() == rid) {
        _state.applyEvent(FSM::ROOM_STARTED);
    }

    return true;
}

bool Logic::onPointsReceived(const QString &request, QJsonObject &root)
{
    if (request != "sync_drawing")
        return false;

    const QString rid = root["rid"].toString();

    if (_roomInfo->rid() == rid) {
        const auto& points = root["points"].toArray();

        QList<QPointF> batchedPoints;

        for (const QJsonValue& point : points) batchedPoints.append(QPointF{ point["x"].toDouble(), point["y"].toDouble() });

        emit pointsBatched(
            batchedPoints,
            QColor(root["color"].toString()),
            root["penWidth"].toInt()
        );
    }

    return true;
}

bool Logic::onFinishGame(const QString &request, QJsonObject &root)
{
    if (request != "finish_game")
        return false;

    const QString rid = root["rid"].toString();
    if (_roomInfo->rid() == rid) {
        _state.applyEvent(FSM::GAME_ENDED);
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


