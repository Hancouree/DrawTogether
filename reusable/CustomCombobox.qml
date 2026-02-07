import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property alias model: combobox.model
    property alias currentIndex: combobox.currentIndex
    property var currentValue: model && currentIndex >= 0 ? model[currentIndex] : null

    implicitWidth: 48
    implicitHeight: 30

    ComboBox {
        id: combobox
        anchors.fill: parent
        focusPolicy: Qt.StrongFocus

        // ===== Background =====
        background: Rectangle {
            radius: 4
            color: combobox.down ? "#e0e0e0" :
                   combobox.hovered ? "#f5f5f5" : "#ffffff"
            border.width: 1
            border.color: combobox.hovered ? "#0078d7" : "#cccccc"
        }

        // ===== Selected text =====
        contentItem: Text {
            text: combobox.displayText || "-"
            font.pixelSize: 12
            font.weight: Font.Medium
            color: "#333333"
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        // ===== Arrow indicator =====
        indicator: Text {
            text: "▼"
            font.pixelSize: 8
            color: "#666666"
            anchors.right: parent.right
            anchors.rightMargin: 4
            anchors.verticalCenter: parent.verticalCenter
        }

        // ===== Custom popup =====
        popup: Popup {
            y: combobox.height + 2
            width: Math.max(combobox.width, 80)
            implicitHeight: Math.min(contentHeight, 150)
            padding: 0

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: combobox.model
                currentIndex: combobox.currentIndex

                delegate: ItemDelegate {
                    width: parent.width
                    height: 28

                    onClicked: {
                        combobox.currentIndex = index
                        combobox.popup.close()
                    }

                    contentItem: Text {
                        text: modelData
                        font.pixelSize: 12
                        font.weight: Font.Medium
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: index === combobox.currentIndex ? "#0078d7" : "#333333"
                    }
                }
            }
        }
    }
}
