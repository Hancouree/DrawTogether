import QtQuick

Item {
    id: root

    property var room
    property bool started: room && room.started

    implicitHeight: 80

    Rectangle {
        id: background
        anchors.fill: parent
        color: "#ffffff"
        radius: 8
        border.width: 1
        border.color: root.started ? "#e0e0e0" : "#4caf50"

        Rectangle {
            width: 4
            anchors {
                top: parent.top
                left: parent.left
                bottom: parent.bottom
                topMargin: 8
                leftMargin: 5
                bottomMargin: 8
            }
            radius: 2
            color: root.started ?  "#757575" : "#4caf50"
        }

        Row {
            anchors {
                left: parent.left
                leftMargin: 16
                right: parent.right
                rightMargin: 16
                verticalCenter: parent.verticalCenter
            }
            spacing: 16

            Column {
                width: parent.width - capacityLabel.width - parent.spacing
                spacing: 4

                Row {
                    width: parent.width
                    spacing: 5

                    Text {
                        text: root.room ? root.room.name : ""
                        font.pixelSize: 16
                        font.bold: true
                        color: "#212121"
                        elide: Text.ElideRight
                    }

                    Text {
                        visible: root.started
                        text: "● В игре"
                        font.pixelSize: 16
                        color: "#4caf50"
                    }
                }

                Text {
                    width: parent.width
                    text: root.room ? root.room.createdAt : ""
                    font.pixelSize: 13
                    color: "#757575"
                }
            }

            Column {
                id: capacityLabel
                width: 70
                spacing: 2

                Text {
                    text: root.room ? `${root.room.currentlyUsers}/${root.room.maxCapacity}` : "0/0"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.right: parent.right
                }

                Text {
                    text: "пользователей"
                    font.pixelSize: 11
                    color: "#9e9e9e"
                    anchors.right: parent.right
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: root.started ? Qt.PointingHandCursor : Qt.ArrowCursor

        onEntered: {
            background.color = "#fafafa"
        }
        onExited: {
            background.color = "#ffffff"
        }
    }
}
