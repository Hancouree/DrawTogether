import QtQuick 2.15
import QtQuick.Effects

Item {
    id: root

    property int innerWidth: innerRect.width
    property int innerHeight: innerRect.height
    default property alias content: innerRect.data

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
            color: "white"

            RectangularShadow {
                anchors.fill: innerRect
                offset.y: 8
                radius: innerRect.radius
                blur: 25
                color: "#40000000"
                z: -1
            }

            Rectangle {
                anchors.fill: parent
                color: "transparent"
                radius: parent.radius
                border.width: 1
                border.color: "gray"
            }
        }
    }
}
