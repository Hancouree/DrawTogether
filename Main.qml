import QtQuick
import QtQuick.Controls

Window {
    id: root

    property bool connected: logic.connected

    width: 640
    height: 480
    visible: true
    title: "DrawTogether"

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: connectionPage
    }

    ConnectionPage {
        id: connectionPage
        visible: true
    }

    LoginPage {
        id: loginPage
        visible: false
    }

    MenuPage {
        id: menuPage
        visible: false
    }

    RoomsPage {
        id: roomsPage
        visible: false
        model: logic.roomsModel
        isInfoLoading: logic.roomsModel.loading
    }

    Connections {
        target: logic
        function onConnectionChanged() {
            if (!logic.connected) {
                if (stackView.currentItem !== connectionPage) {
                    stackView.push(connectionPage)
                }
            } else {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    stackView.replace(loginPage)
                }
            }
        }

        function onConnectionFailed() {
            connectionPage.connectionFailed = true
        }

        function onStateChanged() {
            console.log("Current state: ", logic.state)
            switch(logic.state) {
            case 0: //LOGIN
                stackView.replace(loginPage)
                break;
            case 1: //MENU
                stackView.replace(menuPage)
                break
            case 2: //SEARCHING_ROOMS
                stackView.replace(roomsPage);
            case 3: //CREATING_ROOM
                console.log("Creating room is not yet implemented")
                break;
            }
        }
    }

    Connections {
        target: connectionPage
        function onConnectOnceMore() {
            logic.connectOnceMore()
            connectionPage.connectionFailed = false
        }
    }

    Connections {
        target: loginPage
        function onSetUsername(username) {
            logic.setUsername(username)
        }
    }

    Connections {
        target: menuPage
        function onFindRooms() {
            logic.findRooms()
        }

        function onCreateRoom() {
            // logic.createRoom()
        }
    }

    Connections {
        target: roomsPage
        function onBackClicked() {
            logic.undoTransition()
        }

        function onReloadRooms() {
            logic.findRooms()
        }
    }
}
