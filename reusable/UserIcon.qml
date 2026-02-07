import QtQuick 2.15

Rectangle {
    id: root

    property string letter: ""

    width: 40
    height: 40
    radius: width / 2
    border.width: 2
    border.color: "black"

    Text {
        text: root.letter.toUpperCase()
        font.pixelSize: 20
        font.weight: 550
        anchors.centerIn: parent
    }
}
