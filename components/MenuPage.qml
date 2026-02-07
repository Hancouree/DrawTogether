import QtQuick

CardPage {
    id: root

    signal findRooms();
    signal createRoom();

    Item {
        id: pulseContainer
        anchors.centerIn: column
        z: column.z + 1

        Repeater {
            model: 2

            Rectangle {
                id: wave
                property real baseSize: (2.0 + index * 0.4) * column.height

                width: baseSize
                height: width
                radius: width / 2
                border.width: 1.5
                border.color: "green"
                color: "transparent"
                anchors.centerIn: parent

                scale: 0
                opacity: 0

                ParallelAnimation {
                    running: true
                    loops: Animation.Infinite

                    PauseAnimation {
                        duration: index * 800
                    }

                    SequentialAnimation {
                        ParallelAnimation {
                            NumberAnimation {
                                target: wave
                                property: "scale"
                                from: 0.7
                                to: 1.2 + (index * 0.2)
                                duration: 2500
                                easing.type: Easing.OutQuart
                            }
                            NumberAnimation {
                                target: wave
                                property: "opacity"
                                from: 0.5 - (index * 0.1)
                                to: 0
                                duration: 2500
                                easing.type: Easing.OutQuart
                            }
                        }

                        PauseAnimation { duration: 200 }
                    }
                }
            }
        }
    }

    Column {
        id: column
        // width: childrenRect.width
        // height: childrenRect.height
        spacing: 20
        anchors.centerIn: parent

        Text {
            text: "DrawTogether"
            font.pixelSize: 20
            font.weight: 550
            anchors.horizontalCenter: parent.horizontalCenter
        }

        RectButton {
            id: findRooms
            width: 150
            height: 30
            text: "Find rooms"
            textColor: hovered ? "green" : "black"
            fontSize: 18
            borderWidth: 0
            onClicked: root.findRooms()
        }

        RectButton {
            id: createRoom
            width: 150
            height: 30
            text: "Create room"
            textColor: hovered ? "green" : "black"
            fontSize: 18
            borderWidth: 0
            onClicked: root.createRoom()
        }
    }
}
