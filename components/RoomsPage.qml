import QtQuick
import QtQuick.Controls

Item {
    id: root

    property alias model: listView.model
    property bool isInfoLoading: false

    signal backClicked()
    signal reloadRooms()

    Rectangle {
        id: outerRect
        color: "#f5f0f0"
        anchors.fill: parent

        Rectangle {
            id: innerRect
            width: Math.min(parent.width, 640)
            height: parent.height
            anchors.centerIn: parent

            Column {
                width: childrenRect.width
                height: childrenRect.height
                spacing: 20

                Row {
                    id: row
                    width: innerRect.width
                    height: childrenRect.height

                    RectButton {
                        width: 30
                        height: 30
                        text: "B"
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.topMargin: 5
                        anchors.leftMargin: 10

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: root.backClicked()
                        }
                    }

                    Text {
                        text: "Available rooms"
                        font.pixelSize: 20
                        font.weight: 550
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    RectButton {
                        width: 30
                        height: 30
                        text: "R"
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.topMargin: 5
                        anchors.rightMargin: 10

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: root.reloadRooms()
                        }
                    }
                }

                Item {
                    width: innerRect.width
                    height: innerRect.height - 20 - row.height

                    Text {
                        text: "No rooms are available right now..."
                        font.pixelSize: 16
                        color: "gray"
                        visible: listView.count <= 0 && !root.isInfoLoading
                        anchors.centerIn: parent
                    }

                    ListView {
                        id: listView
                        width: innerRect.width
                        height: parent.height
                        visible: count > 0 && !root.isInfoLoading

                        delegate: Rectangle {}
                    }

                    ListView {
                        id: listViewSkeleton
                        width: innerRect.width
                        height: parent.height
                        model: [1, 2, 3, 4, 5, 6, 7, 8]
                        spacing: 5
                        visible: root.isInfoLoading

                        delegate: RoomSkeleton {
                            width: listViewSkeleton.width
                            height: 60
                        }
                    }
                }
            }
        }
    }
}
