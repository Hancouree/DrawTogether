import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    implicitHeight: 50
    radius: 5

    property int toolSelected: 0
    property int colorSelected: 0
    signal toolChanged(int toolId)
    signal colorChanged(int colorId)

    ListModel {
        id: toolsModel
        ListElement { toolId: 0;  iconSource: "../icons/paint-brush.png" }
        ListElement { toolId: 1; iconSource: "../icons/eraser.png" }
        ListElement { toolId: 2;   iconSource: "../icons/diagonal-line.png" }
        // ListElement { toolId: "rect";   iconSource: "../icons/diagonal-line.png" }
        // ListElement { toolId: "circle";   iconSource: "../icons/diagonal-line.png" }
    }

    property var colors: [];

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 5

        Repeater {
            model: toolsModel

            delegate: Toolbutton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                toolId: model.toolId
                iconSource: model.iconSource
                isSelected: root.toolSelected === model.toolId

                Layout.alignment: Qt.AlignVCenter
                onClicked: root.toolChanged(model.toolId)
            }
        }

        Rectangle {
            id: selectedColor
            Layout.fillHeight: true
            Layout.preferredWidth: 30
            Layout.preferredHeight: 30
            radius: 5
            color: root.colors[root.colorSelected]

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: colorPicker.opacity = colorPicker.opacity > 0 ? 0 : 1
            }
        }
    }

    Rectangle {
        id: colorPicker
        width: 100
        height: 70
        radius: 5
        color: "gray"
        opacity: 0
        anchors { right: parent.right; bottom: parent.top; margins: 10 }

        Behavior on opacity { NumberAnimation { duration: 300 } }

        GridLayout {
            anchors.fill: parent
            anchors.margins: 8
            columns: 3
            columnSpacing: 6
            rowSpacing: 6

            Repeater {
                model: root.colors

                Rectangle {
                    property bool isSelected: index == root.colorSelected

                    Layout.fillWidth:  true
                    Layout.fillHeight: true
                    radius: 4
                    color: modelData
                    border.width: isSelected ? 1 : 0;
                    border.color: isSelected ? "white" : "transparent"

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            root.colorChanged(index)
                            colorPicker.opacity = 0
                        }
                    }
                }
            }
        }
    }
}
