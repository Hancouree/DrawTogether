import QtQuick 2.15

Item {
    id: root

    property alias text: innerText.text
    property alias textColor: innerText.color
    property alias fontSize: innerText.font.pixelSize
    property alias hovered: button.hovered
    property alias borderWidth: button.border.width

    Rectangle {
        id: button

        property bool hovered: hoverHandler.hovered
        property color borderColor: hovered ? "green" : "black"

        border.width: 1
        border.color: borderColor
        anchors.fill: parent

        HoverHandler { id: hoverHandler }

        Text {
            id: innerText
            anchors.centerIn: parent
        }
    }
}
