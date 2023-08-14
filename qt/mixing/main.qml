import QtQuick 2.12
import QtQuick.Window 2.12
import an.qt.Mixing 1.0

Window {
    id:root
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    signal qmlSignal(string message)

    onQmlSignal: console.log("This is signal：",message)

    function qmlFunction(parameter)
    {
        console.log("This is method",parameter)
        return "function from qml"

    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if (mouse.button == Qt.LeftButton){
                mixing.start(Mixing.BALL_COLOR_BLUE);
            }else if (mouse.button == Qt.RightButton){
                mixing.start(Mixing.BALL_COLOR_GREEN);
            }
            mixing.start()
            mixing.number = 10
            qmlSignal("This is qml signal")
        }
        onDoubleClicked: {
            mixing.start(Mixing.BALL_COLOR_YELLOW)
            mixing.number = 10;
        }
    }


    Mixing {
        id:mixing
        onColorChanged: {
            root.color = color
            mixing.stop(color);
        }
        Component.onCompleted: {
            console.log("default ball is", number)
        }

        onNumberChanged: {
            console.log("new ball is", number)
        }

    }
}
