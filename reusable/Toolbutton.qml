import QtQuick

Rectangle {
    id: toolButton

    property string toolId: ""
    property string iconSource: ""
    property bool isSelected: false

    signal clicked()

    implicitWidth: 30
    implicitHeight: 30
    radius: 5
    border.width: isSelected ? 1 : 0
    border.color: isSelected ? "black" : "transparent"

    Image {
        anchors.fill: parent
        anchors.margins: 5
        source: parent.iconSource
        fillMode: Image.PreserveAspectFit
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: toolButton.clicked()
    }
}
