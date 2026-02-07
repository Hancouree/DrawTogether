import QtQuick
import QtQuick.Controls

Item {
    id: root

    property alias model: listView.model
    property bool isInfoLoading: false

    signal backClicked()
    signal reloadRooms()
    signal roomClicked(string rid)

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

                Item {
                    id: row
                    width: parent.width
                    height: 40

                    RectButton {
                        width: 30
                        height: 30
                        borderWidth: 0
                        anchors { top: parent.top; left: parent.left; topMargin: 5; leftMargin: 10  }
                        onClicked: root.backClicked()

                        Image {
                            width: 30
                            height: 30
                            source: "../icons/arrow.png"
                            scale: parent.hovered ? 1.1 : 1
                            anchors.centerIn: parent

                            Behavior on scale {
                                NumberAnimation {
                                    duration: 400
                                    easing.type: Easing.OutBack
                                }
                            }
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
                        borderWidth: 0
                        anchors { top: parent.top; right: parent.right; topMargin: 5; rightMargin: 15 }

                        Image {
                            width: 30
                            height: 30
                            source: "../icons/reload.png"
                            rotation: parent.hovered ? 360 : 0
                            anchors.centerIn: parent

                            Behavior on rotation {
                                NumberAnimation {
                                    duration: 800
                                    easing.type: Easing.OutBack
                                }
                            }
                        }

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
                        spacing: 5
                        visible: count > 0 && !root.isInfoLoading

                        signal roomClicked(string rid)

                        delegate: Room {
                            width: listView.width
                            height: 60
                            rid: model.rid
                            setName: model.name
                            setCreatedAt: model.createdAt
                            maxCapacity: model.maxCapacity
                            currentlyUsers: model.currentlyUsers

                            MouseArea {
                                anchors.fill: parent
                                onClicked: listView.roomClicked(parent.rid)
                            }
                        }
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

    Connections {
        target: listView
        function onRoomClicked(rid) {
            root.roomClicked(rid)
        }
    }
}
