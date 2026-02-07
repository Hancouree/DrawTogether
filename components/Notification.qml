import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: root

    property var notification

    Layout.maximumHeight: notification && notification.visible ? 80 : 0
    opacity: notification && notification.visible ? 1.0 : 0.0
    visible: opacity > 0
    color: notification.color
    radius: 8
    clip: true

    Behavior on Layout.maximumHeight { NumberAnimation { duration: 200 } }
    Behavior on opacity { NumberAnimation { duration: 200 } }

    ColumnLayout {
        anchors.fill: parent
        spacing: 4
        anchors.margins: 10

        Text {
            text: root.notification ? root.notification.title : ""
            color: "white"
            font.bold: Font.DemiBold
            Layout.alignment: Qt.AlignHCenter
            font.pixelSize: 16
        }

        Text {
            id: message
            text: root.notification ? root.notification.message : ""
            color: "white"
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 3
            color: "#66FFFFFF"
            Rectangle {
                width: parent.width * (root.notification ? root.notification.progress : 0)
                height: parent.height
                color: "white"
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            if (root.notification) root.notification.visible = false
        }
    }
}
