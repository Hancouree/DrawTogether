import QtQuick

Item {
    id: root

    property var room

    Rectangle {
        id: rect
        width: parent.width
        height: parent.height
        color: "#d1d1d1"
        border.width: 1
        border.color: "gray"

        Text {
            id: name
            text: root.room.name
            font.pixelSize: 16
            anchors { top: parent.top; left: parent.left; topMargin: 5; leftMargin: 10 }
        }

        Text {
            id: users
            text: `${root.room.currentlyUsers}/${root.room.maxCapacity}`
            font.pixelSize: 16
            anchors { top: parent.top; right: parent.right; topMargin: 5; rightMargin: 10 }
        }

        Text {
            id: createdAt
            text: root.room.createdAt
            font.pixelSize: 14
            anchors { bottom: parent.bottom; left: parent.left; bottomMargin: 5; leftMargin: 10 }
        }
    }
}
