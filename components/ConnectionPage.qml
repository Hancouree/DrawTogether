import QtQuick
import QtQuick.Effects

Item {
    id: root

    property bool connectionFailed: false

    signal connectOnceMore()

    Rectangle {
        id: outerRect
        color: "#f5f0f0"
        anchors.fill: parent

        Rectangle {
            id: innerRect
            width: Math.min(parent.width, 640)
            height: Math.min(parent.height, 480)
            radius: parent.width > 640 ? 5 : 0
            anchors.centerIn: parent

            RectangularShadow {
                anchors.fill: innerRect
                offset.x: 0
                offset.y: 8
                radius: innerRect.radius
                blur: 25
                spread: 0.0
                color: "#40000000"
                z: innerRect.z - 1
            }

            Item {
                width: mainColumn.width
                height: mainColumn.height
                anchors.centerIn: parent

                Column {
                    id: mainColumn
                    spacing: 20
                    anchors.horizontalCenter: parent.horizontalCenter

                    Column {
                        spacing: 20
                        visible: root.connectionFailed
                        anchors.horizontalCenter: parent.horizontalCenter

                        Text {
                            text: "Failed to connect to the server, check your internet connection... :("
                            font.pixelSize: 16
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        RectButton {
                            width: 150
                            height: 30
                            text: "Try once more"
                            anchors.horizontalCenter: parent.horizontalCenter

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: root.connectOnceMore()
                            }
                        }
                    }

                    Column {
                        spacing: 20
                        visible: !root.connectionFailed
                        anchors.horizontalCenter: parent.horizontalCenter

                        Text {
                            text: "Connecting to the server..."
                            font.pixelSize: 16
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        Spinner {
                            id: spinner
                            width: 25
                            height: 25
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }
            }
        }
    }
}
