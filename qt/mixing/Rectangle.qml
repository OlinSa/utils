import QtQuick 2.0
import QtQuick.Window 2.2

Rectangle {
    id: root
    width: 640
    height: 480
    color: "lightgray"

    Text {
        id: text
        anchors.centerIn: parent
        text: Data
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked:  {
            mixing.start();
            mixing.number = 10
        }
    }
    Connections {
        target: mixing
        onColorChanged: {
            root.color = color
            mixing.stop(color)
        }
        onNumberChanged:{
            console.log("new ball number is", mixing.number) // 10
        }
    }
}
