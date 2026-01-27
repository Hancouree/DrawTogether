import QtQuick

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

            Item {
                width: childrenRect.width
                height: childrenRect.height
                anchors.centerIn: parent

                Column {
                    width: childrenRect.width
                    height: childrenRect.height
                    spacing: 20
                    visible: root.connectionFailed
                    anchors.centerIn: parent

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
                    width: childrenRect.width
                    height: childrenRect.height
                    spacing: 20
                    visible: !root.connectionFailed
                    anchors.centerIn: parent

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
