import QtQuick 2.15

Item {
    id: root

    property alias text: innerText.text
    property alias textColor: innerText.color
    property alias fontSize: innerText.font.pixelSize
    property alias fontWeight: innerText.font.weight
    property alias fontColor: innerText.color
    property alias hovered: button.hovered
    property alias borderWidth: button.border.width
    property alias radius: button.radius
    property alias color: button.color
    signal clicked()

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

        MouseArea {
            cursorShape: Qt.PointingHandCursor
            anchors.fill: parent
            onClicked: root.clicked()
        }
    }
}
