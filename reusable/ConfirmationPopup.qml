import QtQuick
import QtQuick.Layouts
import QtQuick.Effects

Rectangle {
    id: root
    radius: 5
    border.width: 1
    border.color: "gray"
    color: Qt.darker("white", 1.1)
    opacity: 1
    visible: opacity > 0

    property alias text: confirmationText.text
    signal stayClicked()
    signal continueClicked()

    RectangularShadow {
        anchors.fill: parent
        offset.y: 8
        radius: parent.radius
        blur: 25
        color: "#40000000"
        z: -1
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Text {
            id: confirmationText
            text: "Are you sure?"
            font.pixelSize: 16
            Layout.alignment: Qt.AlignHCenter
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10

            RectButton {
                color: "steelblue"
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: "Stay"
                radius: 5
                borderWidth: 0
                fontSize: 16
                fontColor: "white"
                onClicked: root.stayClicked()
            }

            RectButton {
                color: "lightgrey"
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: "Continue"
                radius: 5
                borderWidth: 0
                fontSize: 16
                fontColor: "black"
                onClicked: root.continueClicked()
            }
        }
    }

    Behavior on opacity {
        NumberAnimation { duration: 300 }
    }
}
