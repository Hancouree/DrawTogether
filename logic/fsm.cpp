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
                            {BACK, MENU}
                          }
        },
        {CREATING_ROOM, {
                            {BACK, MENU}
                        }
        }
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

