import QtQuick

Item {
    id: root

    property color selectedColor

    Rectangle {
        anchors.fill: parent
        radius: 10
        border.color: "grey"
        border.width: 1

        Row {
            spacing: 10

            Rectangle {
                width: 20
                height: 20
                color: root.selectedColor
                radius: 10
            }
        }
    }
}
