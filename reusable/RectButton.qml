import QtQuick 2.15

Item {
    id: root

    property alias text: innerText.text

    Rectangle {
        id: button

        property color borderColor: hoverHandler.hovered ? "green" : "black"

        border.color: borderColor
        anchors.fill: parent

        HoverHandler { id: hoverHandler }

        Text {
            id: innerText
            anchors.centerIn: parent
        }
    }
}
