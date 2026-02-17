import QtQuick
import QtQuick.Effects

Rectangle {
    id: toolButton

    property int toolId: 0
    property string iconSource: ""
    property bool isSelected: false

    signal clicked()

    implicitWidth: 30
    implicitHeight: 30
    radius: 5

    RectangularShadow {
        anchors.fill: parent
        offset.y: 3
        radius: parent.radius
        blur: parent.isSelected ? 12 : 0
        color: parent.isSelected ? "#40000000" : "transparent"
        z: -1
    }

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
