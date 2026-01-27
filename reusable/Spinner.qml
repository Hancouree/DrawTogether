import QtQuick 2.15

Item {
    id: spinner

    Repeater {
        model: 4

        Rectangle {
            width: 6
            height: 6
            radius: 3
            color: "green"
            opacity: 0.2 + index * 0.2

            x: spinner.width / 2 - width / 2
            y: -height / 2

            transform: Rotation {
                origin.x: spinner.width / 2
                origin.y: spinner.height / 2
                angle: index * 90
            }
        }
    }

    RotationAnimation {
        target: spinner
        property: "rotation"
        from: 0
        to: 360
        duration: 1200
        loops: Animation.Infinite
        running: true
    }
}
