import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property var notificationManager

    ColumnLayout {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10

        Repeater {
            model: root.notificationManager ? root.notificationManager.notifications : []

            delegate: Notification {
                Layout.preferredWidth: root.width / 3 //should work more properly on its size
                Layout.preferredHeight: 80
                Layout.margins: 10
                notification: modelData
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }
}
