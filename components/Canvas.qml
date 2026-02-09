import QtQuick
import QtQuick.Effects
import DrawingLib

Item {
    id: root
    anchors.fill: parent
    clip: true

    Item {
        id: cameraNode
        width: 2000
        height: 1500

        transform: [
            Scale {
                id: canvasScale
                origin.x: cameraNode.width / 2
                origin.y: cameraNode.height / 2
            },
            Translate {
                id: canvasTranslate
            }
        ]

        Rectangle {
            anchors.fill: parent
            color: Qt.darker("white", 1.05)
            border.color: "grey"
            border.width: 3
            radius: 20

            RectangularShadow {
                anchors.fill: parent
                offset.y: 8
                radius: parent.radius
                blur: 25
                color: "#40000000"
                z: -1
            }
        }

        DrawingCanvas {
            id: drawingCanvas
            anchors.fill: parent
            penColor: "black"

            onPointsBatched: function(batched) {
                logic.sendPoints(batched, drawingCanvas.penColor)
            }

            Connections {
                target: logic
                function onPointsBatched(points, color) {
                    drawingCanvas.drawRemoteBatch(points, color)
                }
            }
        }
    }

    // Keys.onPressed: function(event) {
    //     if (event.key === Qt.Key_Space) {
    //         // console.log(mainMouseArea)
    //     }
    // }

    MouseArea {
        id: mainMouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton

        property point lastPanPos

        onPressed: (mouse) => {
            var canvasPoint = mapToItem(cameraNode, mouse.x, mouse.y)

            if (mouse.button === Qt.LeftButton) {
                drawingCanvas.startDrawing(canvasPoint.x, canvasPoint.y)
            } else {
                lastPanPos = Qt.point(mouse.x, mouse.y)
            }
        }

        onPositionChanged: (mouse) => {
            var canvasPoint = mapToItem(cameraNode, mouse.x, mouse.y)

            if (mouse.buttons & Qt.LeftButton) {
                drawingCanvas.addPoint(canvasPoint.x, canvasPoint.y)
            } else if (mouse.buttons & (Qt.RightButton | Qt.MiddleButton)) {
                var dx = mouse.x - lastPanPos.x
                var dy = mouse.y - lastPanPos.y
                canvasTranslate.x += dx
                canvasTranslate.y += dy
                lastPanPos = Qt.point(mouse.x, mouse.y)
            }
        }

        onWheel: (wheel) => {
            var zoomFactor = 1.1
            var oldScale = canvasScale.xScale

            if (wheel.angleDelta.y > 0)
                canvasScale.xScale = Math.min(10, canvasScale.xScale * zoomFactor)
            else
                canvasScale.xScale = Math.max(0.1, canvasScale.xScale / zoomFactor)

            canvasScale.yScale = canvasScale.xScale

            var mouseInCanvas = mapToItem(cameraNode, wheel.x, wheel.y)
            canvasTranslate.x -= mouseInCanvas.x * (canvasScale.xScale - oldScale)
            canvasTranslate.y -= mouseInCanvas.y * (canvasScale.xScale - oldScale)
        }
    }

    RectButton {
        width: 30
        height: 30
        color: "transparent"
        borderWidth: 0
        anchors { top: parent.top; left: parent.left; margins: 10 }
        onClicked: logic.leaveRoom()

        Image {
            anchors.fill: parent
            source: "../icons/arrow.png"
            scale: parent.hovered ? 1.1 : 1
            anchors.centerIn: parent

            Behavior on scale {
                NumberAnimation {
                    duration: 400
                    easing.type: Easing.OutBack
                }
            }
        }
    }
}
