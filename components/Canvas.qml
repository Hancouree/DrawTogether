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

            property int currentColor: 0
            penColor: colors[currentColor]
            anchors.fill: parent

            function syncToolState(key, value) {
                for (var t in toolStates)
                    if (parseInt(t) !== DrawingCanvas.Eraser) {
                        toolStates[t][key] = value
                    }
            }

            property var toolStates: ({
                [DrawingCanvas.Brush]: { color: "black", width: 3 },
                [DrawingCanvas.Eraser]: { color: Qt.darker("white", 1.05), width: 15 },
                [DrawingCanvas.Line]: { color: "black", width: 3 }
            })

            function applyToolSettings() {
                const setting = toolStates[tool]
                if (setting) {
                    penColor = setting.color
                    penWidth = setting.width
                }
            }

            property var colors: [
                "#000000",
                "#EF4444",
                "#3B82F6",
                "#10B981",
                "#F59E0B",
                "#8B5CF6"
            ]

            onToolChanged: applyToolSettings()
            onCurrentColorChanged: {
                penColor = colors[currentColor]
                if (tool != DrawingCanvas.Eraser) {
                    syncToolState("color", penColor)
                }
            }
            onPenWidthChanged: { if (tool !== DrawingCanvas.Eraser) syncToolState("width", penWidth) }
            onPointsBatched: function(batched) { logic.sendPoints(batched, drawingCanvas.penColor, drawingCanvas.penWidth) }

            Connections {
                target: logic
                function onPointsBatched(points, penColor, penWidth) { drawingCanvas.drawRemoteBatch(points, penColor, penWidth) }
            }
        }
    }

    // Keys.onPressed: function(event) {
    //     if (event.key === Qt.Key_Space) {
    //         var canvasPoint = mapToItem(cameraNode, mainMouseArea.mouseX, mainMouseArea.mouseY)
    //         drawingCanvas.startDrawing(canvasPoint.x, canvasPoint.y)
    //     }
    // }

    // Keys.onReleased: function(event) {
    //     if (event.key === Qt.Key_Space) {
    //         var canvasPoint = mapToItem(cameraNode, mainMouseArea.mouseX, mainMouseArea.mouseY)
    //         drawingCanvas.addPoint(canvasPoint.x, canvasPoint.y)
    //     }
    // }

    MouseArea {
        id: mainMouseArea
        anchors.fill: parent
        hoverEnabled: true
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
                switch(drawingCanvas.tool) {
                    case DrawingCanvas.Brush:
                        drawingCanvas.addPoint(canvasPoint.x, canvasPoint.y)
                    break;
                    case DrawingCanvas.Eraser:
                        drawingCanvas.addPoint(canvasPoint.x, canvasPoint.y)
                    break;
                   case DrawingCanvas.Line: break;
                }
            } else if (mouse.buttons & (Qt.RightButton | Qt.MiddleButton)) {
                var dx = mouse.x - lastPanPos.x
                var dy = mouse.y - lastPanPos.y
                canvasTranslate.x += dx
                canvasTranslate.y += dy
                lastPanPos = Qt.point(mouse.x, mouse.y)
            }
        }

        onReleased: function(mouse) {
            var canvasPoint = mapToItem(cameraNode, mouse.x, mouse.y)

            if (mouse.button === Qt.LeftButton) {
                switch(drawingCanvas.tool) {
                case DrawingCanvas.Line:
                    drawingCanvas.addPoint(canvasPoint.x, canvasPoint.y)
                default:
                    drawingCanvas.finishDrawing()
                }
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
        onClicked: confirmationPopup.opacity = 1

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

    RectButton {
        width: 30
        height: 30
        color: "transparent"
        borderWidth: 0
        anchors { top: parent.top; right: parent.right; margins: 10 }
        onClicked: logic.finishGame()

        Image {
            anchors.fill: parent
            source: "../icons/finish.png"
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

    ConfirmationPopup {
        id: confirmationPopup
        width: Math.min(parent.width / 2, 340)
        height: 85
        opacity: 0
        anchors.centerIn: parent
        onStayClicked: opacity = 0
        onContinueClicked: logic.leaveRoom()
    }

    Toolbar {
        id: toolbar
        width: Math.min(parent.width / 2, 340)
        toolSelected: drawingCanvas.tool
        colors: drawingCanvas.colors
        colorSelected: drawingCanvas.currentColor
        anchors { bottom: parent.bottom; bottomMargin: 10; horizontalCenter: parent.horizontalCenter }
        onToolChanged: function(toolId) { drawingCanvas.tool = toolId }
        onColorChanged: function(colorId) { drawingCanvas.currentColor = colorId }
    }
}
