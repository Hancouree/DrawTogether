#include "fsm.h"

FSM::FSM(QObject* parent) : QObject(parent),  _state(LOGIN) {
    transition = {
        {LOGIN, {
                    {USERNAME_ESTABLISHED, MENU}
                }
        },
        {MENU, {
                    {SEARCH_ROOMS, SEARCHING_ROOMS},
                    {CREATE_ROOM, CREATING_ROOM},
               },
        },
        {SEARCHING_ROOMS, {
                    {BACK, MENU},
                    {JOINED_ROOM, IN_ROOM}
                          }
        },
        {CREATING_ROOM, {
                    {BACK, MENU},
                    {JOINED_ROOM, IN_ROOM}
        }
        },
        {IN_ROOM, {
                    {LEFT_ROOM, MENU},
                    {KICKED, MENU},
                    {ROOM_STARTED, IN_GAME}
        }},
        {IN_GAME, {
                   {LEFT_ROOM, MENU}
        }}
    };
}

void FSM::applyEvent(const Events &event)
{
    if (transition[_state].contains(event)) {
        _state = transition[_state][event];
        emit stateChanged();
    }
}

FSM::~FSM() {}

