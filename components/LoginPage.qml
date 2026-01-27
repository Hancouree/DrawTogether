import QtQuick
import QtQuick.Controls

Item {
    id: root

    function handleUsername() {
        const username = input.text.trim()
        if (username.length > 3) {
            setUsername(username)
            input.clear()
        }
    }

    signal setUsername(string username)

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

            Column {
                width: input.width
                height: childrenRect.height
                spacing: 15
                anchors.centerIn: parent

                Text {
                    id: text
                    text: "Select your username"
                    font.pixelSize: 18
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                TextField {
                    id: input

                    property color borderColor: focus ? "green" : "black"

                    width: 300
                    height: 30
                    placeholderText: "Write here..."
                    verticalAlignment: Text.AlignVCenter

                    background: Rectangle {
                        border.width: 1
                        border.color: input.borderColor
                        radius: 5
                    }

                    Keys.onPressed: function(event) {
                        if (event.key === Qt.Key_Return)
                            root.handleUsername()
                    }
                }

                RectButton {
                    id: button
                    width: input.width / 2
                    height: 30
                    anchors.horizontalCenter: parent.horizontalCenter

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.handleUsername()
                    }
                }
            }
        }
    }
}
