import QtQuick 2.0

import Squarez 1.0

Rectangle {
    width: 360
    height: 360
    SinglePlayer
    {
        id:rules
        property var selection : []
    }
    Repeater
    {
        model:rules.board
        Rectangle
        {
            x: 25 * modelData.x
            y: 25 * modelData.y
            width: 20
            height: 20
            color: modelData.symbol === 0 ? "red" : (modelData.symbol === 1 ? "yellow" : "blue")

            MouseArea {
                anchors.fill: parent
                onClicked:
                {
                    console.log("clicked " + modelData.x + "," + modelData.y)
                    rules.selection.push(Qt.point(modelData.x, modelData.y))
                }
            }
        }
    }
}
