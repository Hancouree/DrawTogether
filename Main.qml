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

    NotificationsContainer {
        anchors.fill: parent
        z: 9999
        notificationManager: logic.notificationManager
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

    SearchRoomsPage {
        id: searchRoomsPage
        isInfoLoading: logic.roomsModel.loading
        model: logic.roomsModel
        visible: false
    }

    RoomCreationPage {
        id: roomCreationPage
        username: logic.username
        visible: false
    }

    RoomPage {
        id: roomPage
        roomInfo: logic.roomInfo
        visible: false
    }

    Connections {
        target: logic
        function onConnectionChanged() {
            if (!root.connected) {
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
            case FSM.LOGIN:
                stackView.replace(loginPage)
                break
            case FSM.MENU:
                stackView.replace(menuPage)
                break
            case FSM.SEARCHING_ROOMS:
                stackView.replace(searchRoomsPage);
                break
            case FSM.CREATING_ROOM:
                stackView.replace(roomCreationPage)
                break
            case FSM.IN_ROOM:
                stackView.replace(roomPage)
                break;
            }
        }
    }

    Connections {
        target: connectionPage
        function onConnectOnceMore() {
            connectionPage.connectionFailed = false
            logic.connectOnceMore()
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
            logic.openRoomCreationPage()
        }
    }

    Connections {
        target: searchRoomsPage
        function onBackClicked() {
            logic.undoTransition()
        }

        function onRoomClicked(rid) {
            logic.joinRoom(rid)
        }

        function onReloadRooms() {
            logic.findRooms()
        }
    }

    Connections {
        target: roomCreationPage
        function onBackClicked() {
            logic.undoTransition()
        }

        function onCreateRoom(roomName, maxCapacity) {
            logic.createRoom(roomName, maxCapacity)
        }
    }

    Connections {
        target: roomPage
        function onLeaveRoom() {

        }

        function onStartRoom() {

        }
    }
}
