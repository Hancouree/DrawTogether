import QtQuick
import QtQuick.Controls

CardPage {
    id: root

    property string username: ""
    signal backClicked()
    signal createRoom(string roomName, int maxCapacity)

    function handleCreateRoom() {
        const title = roomName.text.trim(), maxCapacity = combobox.currentValue
        if (title.length >= 3 && title.length < 20) {
            root.createRoom(title, maxCapacity)
            roomName.clear()
        }
    }

    RectButton {
        width: 30
        height: 30
        borderWidth: 0
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        onClicked: root.backClicked()

        Image {
            width: parent.width
            height: width
            source: "../icons/arrow.png"
            scale: parent.hovered ? 1.1 : 1
            anchors.verticalCenter: parent.verticalCenter

            Behavior on scale {
                NumberAnimation {
                    duration: 400
                    easing.type: Easing.OutBack
                }
            }
        }
    }

    Text {
        text: "Settings"
        font.pixelSize: 24
        font.weight: Font.DemiBold
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
    }

    Column {
        id: column
        spacing: 10
        anchors.centerIn: parent

        Rectangle {
            width: 0.6 * root.width
            height: 64
            radius: 14
            color: "#f6f7fb"
            anchors.horizontalCenter: parent.horizontalCenter

            Row {
                anchors.centerIn: parent
                spacing: 10

                UserIcon {
                    letter: root.username.length > 0 ? root.username.charAt(0) : "?"
                    anchors.verticalCenter: userInfo.verticalCenter
                }

                Column {
                    id: userInfo
                    spacing: 2

                    Text {
                        text: root.username
                        font.pixelSize: 20
                        font.weight: Font.DemiBold
                    }

                    Text {
                        text: "You'll be the leader"
                        color: "gray"
                    }
                }
            }
        }

        TextField {
            id: roomName

            property color borderColor: focus ? "green" : "black"

            width: 300
            height: 30
            placeholderText: "Write the room's name..."
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter

            background: Rectangle {
                border.width: 1
                border.color: roomName.borderColor
                radius: 5
            }

            Keys.onPressed: function(event) {
                if (event.key === Qt.Key_Return) root.handleCreateRoom()
            }
        }

        Rectangle {
            width: 0.4 * root.width
            height: 1
            color: "gray"
            radius: 5
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: "Max capacity"
                font.pixelSize: 22
                font.weight: Font.DemiBold
            }

            CustomCombobox {
                id: combobox
                width: 40
                height: 30
                model: [2, 3, 5]
                currentIndex: 0
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        RectButton {
            id: createBtn

            property bool blocked: false
            property int blockDuration: 3000

            width: 120
            height: 30
            text: "Create room"
            radius: 5
            enabled: !blocked
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: root.handleCreateRoom()

            Timer {
                id: blockTimer
                interval: createBtn.blockDuration
                onTriggered: createBtn.blocked = true
            }
        }
    }
}
