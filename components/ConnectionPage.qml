import QtQuick

CardPage {
    id: root
    property bool connectionFailed: false

    signal connectOnceMore()

    Item {
        width: mainColumn.width
        height: mainColumn.height
        anchors.centerIn: parent

        Column {
            id: mainColumn
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter

            Column {
                spacing: 20
                visible: root.connectionFailed
                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    text: "Failed to connect to the server, check your internet connection... :("
                    font.pixelSize: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                RectButton {
                    width: 150
                    height: 30
                    text: "Try once more"
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: root.connectOnceMore()
                }
            }

            Column {
                spacing: 20
                visible: !root.connectionFailed
                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    text: "Connecting to the server..."
                    font.pixelSize: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Spinner {
                    id: spinner
                    width: 25
                    height: 25
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }
}
