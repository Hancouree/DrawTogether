#ifndef FSM_H
#define FSM_H

#include <QHash>
#include <QObject>

class FSM : public QObject
{
    Q_OBJECT
public:
    enum States {
        LOGIN,
        MENU,
        SEARCHING_ROOMS,
        CREATING_ROOM,
        IN_ROOM,
        IN_GAME
    };
    Q_ENUM(States)

    enum Events {
        USERNAME_ESTABLISHED,
        BACK,
        SEARCH_ROOMS,
        CREATE_ROOM,
        JOINED_ROOM,
        LEFT_ROOM,
        KICKED,
        ROOM_STARTED,
        GAME_ENDED
    };
    Q_ENUM(Events);

    FSM(QObject* parent = nullptr);
    ~FSM();

    States state() const { return _state; }
    void applyEvent(const Events& event);
signals:
    void stateChanged();
private:
    States _state;
    QHash<States, QHash<Events, States>> transition;
};

#endif // FSM_H
