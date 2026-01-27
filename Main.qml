import QtQuick
import QtQuick.Controls

Window {
    id: root

    property bool connected: logic.connected

    width: 640
    height: 480
    visible: true
    title: "DrawTogether"

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: root.connected ? loginPage : connectionPage
    }

    ConnectionPage {
        id: connectionPage
        visible: !root.connected
    }

    LoginPage {
        id: loginPage
        visible: root.connected
    }

    Connections {
        target: logic
        function onConnectionChanged() {
            if (!logic.connected) {
                if (stackView.currentItem !== connectionPage) {
                    stackView.push(connectionPage)
                }
            } else {
                if (stackView.currentItem === connectionPage) {
                    stackView.pop()
                }

                if (!stackView.currentItem) {
                    stackView.push(loginPage)
                }
            }
        }

        function onConnectionFailed() {
            connectionPage.connectionFailed = true
        }
    }

    Connections {
        target: connectionPage
        function onConnectOnceMore() {
            logic.connectOnceMore()
            connectionPage.connectionFailed = false
        }
    }
}
