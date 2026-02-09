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

        replaceEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: stackView.width * 0.3
                to: 0
                duration: 400
                easing.type: Easing.OutCubic
            }

            PropertyAnimation {
                property: "opacity"
                from: 0.0
                to: 1.0
                duration: 400
            }
        }

        replaceExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1.0
                to: 0.0
                duration: 400
            }
        }
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
                    stackView.replace("components/LoginPage.qml"/*"components/Canvas.qml"*/)
                }
            }
        }

        function onConnectionFailed() {
            if (stackView.currentItem.hasOwnProperty("connectionFailed")) {
                stackView.currentItem.connectionFailed = true
            }
        }

        function onStateChanged() {
            switch(logic.state) {
            case FSM.LOGIN:
                stackView.replace("components/LoginPage.qml")
                break
            case FSM.MENU:
                stackView.replace("components/MenuPage.qml")
                break
            case FSM.SEARCHING_ROOMS:
                stackView.replace("components/SearchRoomsPage.qml", {
                    "rooms": logic.roomsModel
                })
                break
            case FSM.CREATING_ROOM:
                stackView.replace("components/RoomCreationPage.qml", {
                    "username": logic.username
                })
                break
            case FSM.IN_ROOM:
                stackView.replace("components/RoomPage.qml", {
                    "roomInfo": logic.roomInfo
                })
                break
            case FSM.IN_GAME:
                stackView.replace("components/Canvas.qml")
                break;
            }
        }
    }
}
