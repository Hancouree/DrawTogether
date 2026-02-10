import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property var roomInfo;

    Rectangle {
        id: outerRect
        color: "#f5f0f0"
        anchors.fill: parent

        Rectangle {
            id: innerRect
            width: Math.min(parent.width, 640)
            height: parent.height
            anchors.centerIn: parent

            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                Column {
                    Layout.preferredWidth: childrenRect.width
                    Layout.preferredHeight: childrenRect.height
                    spacing: 5
                    Layout.alignment: Qt.AlignHCenter

                    Text {
                        text: root.roomInfo.name
                        font.pixelSize: 20
                        font.weight: Font.DemiBold
                        anchors.horizontalCenter: parent.horizontalCenter //if its smaller than roomId
                    }

                    Text {
                        text: "#" +root.roomInfo.rid.slice(1, root.roomInfo.rid.indexOf("-"))
                        font.pixelSize: 16
                        anchors.horizontalCenter: parent.horizontalCenter //if its smaller then name
                    }
                }

                Text {
                    text: root.roomInfo.participantsCount + "/" + root.roomInfo.maxCapacity
                    font.pixelSize: 20
                    font.weight: Font.DemiBold
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: 10
                }

                Rectangle {
                    Layout.preferredWidth: 0.98 * innerRect.width
                    Layout.fillHeight: true
                    color: Qt.darker("white", 1.05)
                    radius: 5
                    Layout.alignment: Qt.AlignHCenter

                    ListView {
                        id: listView
                        anchors.fill: parent
                        model: root.roomInfo ? root.roomInfo.participants : []
                        clip: true
                        anchors.margins: 10
                        spacing: 10

                        delegate: Item {
                            width: listView.width
                            height: childrenRect.height

                            Rectangle {
                                anchors.fill: parent
                                color: "#95d695"
                                visible: model.isLeader
                                radius: 5
                            }

                            RowLayout {
                                width: parent.width
                                spacing: 5

                                UserIcon {
                                    letter: model.username.length > 0 ? model.username.charAt(0) : "?"
                                    Layout.alignment: Qt.AlignLeft
                                    Layout.margins: 10
                                }

                                Text {
                                    Layout.fillWidth: true
                                    text: model.username
                                    font.pixelSize: 20
                                    font.weight: Font.DemiBold
                                    elide: Text.ElideRight
                                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                                }

                                RectButton {
                                    Layout.preferredWidth: 20
                                    Layout.preferredHeight: 20
                                    borderWidth: 0
                                    color: "transparent"
                                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                                    visible: root.roomInfo.isMeLeader && !model.isLeader
                                    Layout.rightMargin: 10
                                    onClicked: logic.kickUser(model.id)

                                    Image {
                                        anchors.fill: parent
                                        source: "../icons/kick.png"
                                    }
                                }
                            }
                        }
                    }
                }

                Row {
                    Layout.preferredWidth: childrenRect.width
                    Layout.preferredHeight: childrenRect.height
                    spacing: 10
                    Layout.alignment: Qt.AlignRight
                    Layout.margins: {
                        rightMargin: 10;
                        bottomMargin: 10
                    }

                    RectButton {
                        id: leaveBtn
                        width: 90
                        height: 30
                        text: "Leave"
                        fontColor: "white"
                        fontSize: 16
                        fontWeight: Font.DemiBold
                        color: "red"
                        borderWidth: 0
                        radius: 5
                        scale: hovered ? 1.1 : 1
                        onClicked: /*logic.leaveRoom()*/confirmationPopup.opacity = 1

                        Behavior on scale {
                            NumberAnimation { duration: 200 }
                        }
                    }

                    RectButton {
                        id: startBtn
                        width: visible ? 90 : 0
                        height: 30
                        text: "Start"
                        fontColor: "white"
                        fontSize: 16
                        fontWeight: Font.DemiBold
                        color: "green"
                        borderWidth: 0
                        radius: 5
                        scale: hovered ? 1.1 : 1
                        visible: root.roomInfo.isMeLeader
                        clip: true
                        onClicked: logic.startRoom()

                        Behavior on scale {
                            NumberAnimation { duration: 200 }
                        }
                    }
                }
            }
        }

        ConfirmationPopup {
            id: confirmationPopup
            width: Math.min(parent.width / 2, 340)
            height: 85
            opacity: 0
            anchors.centerIn: parent
            onStayClicked: opacity = 0
            onContinueClicked: logic.leaveRoom()
        }
    }
}
