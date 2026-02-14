import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    implicitHeight: 50
    radius: 5

    property string selected: "brush"
    signal toolSelected(string toolId)

    ListModel {
        id: toolsModel
        ListElement { toolId: "brush";  iconSource: "../icons/paint-brush.png" }
        ListElement { toolId: "eraser"; iconSource: "../icons/eraser.png" }
        ListElement { toolId: "line";   iconSource: "../icons/diagonal-line.png" }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 5

        Repeater {
            model: toolsModel

            delegate: Toolbutton {
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                toolId: model.toolId
                iconSource: model.iconSource
                isSelected: root.selected === model.toolId

                Layout.alignment: Qt.AlignVCenter
                onClicked: {
                    root.selected = model.toolId
                    root.toolSelected(model.toolId)
                }
            }
        }

        Item { Layout.fillWidth: true }
    }
}
