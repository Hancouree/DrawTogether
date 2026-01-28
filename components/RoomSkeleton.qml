import QtQuick

Item {
    id: root
    implicitWidth: 300
    implicitHeight: 100

    readonly property color baseColor: "#d1d1d1"
    readonly property color shineColor: "#ffffff"
    readonly property color bgColor: "#e8e8e8"

    Rectangle {
        id: mainRect
        anchors.fill: parent
        color: root.bgColor
        clip: true

        Rectangle {
            id: shimmerBar
            width: parent.width * 1.5
            height: parent.height * 2
            y: -parent.height / 2

            rotation: 20

            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "transparent" }
                GradientStop { position: 0.4; color: "transparent" }
                GradientStop { position: 0.5; color: root.shineColor }
                GradientStop { position: 0.6; color: "transparent" }
                GradientStop { position: 1.0; color: "transparent" }
            }

            NumberAnimation on x {
                from: -shimmerBar.width
                to: mainRect.width
                duration: 1200
                loops: Animation.Infinite
                easing.type: Easing.InOutSine
            }
        }

        Item {
            anchors.fill: parent

            Rectangle {
                width: 120; height: 24
                color: root.baseColor; radius: 6
                anchors { top: parent.top; left: parent.left; topMargin: 5; leftMargin: 10 }
            }

            Rectangle {
                width: 50; height: 24
                color: root.baseColor; radius: 12
                anchors { top: parent.top; right: parent.right; topMargin: 5; rightMargin: 10 }
            }

            Rectangle {
                width: 200; height: 18
                color: root.baseColor; radius: 4
                anchors { bottom: parent.bottom; left: parent.left; bottomMargin: 5; leftMargin: 10 }
            }
        }

        layer.enabled: true
    }
}
