import QtQuick 2.0

import Squarez 1.0

Rectangle {
    id: gameArea

    width: 360
    height: 360

    property var cellSize: Math.min(gameArea.width, gameArea.height) / rules.board.size

    SinglePlayer
    {
        id:rules
    }

    Item
    {
        id:selection
        property var points: []
        signal onSelect(variant points)
    }

    Repeater
    {
        model:rules.board
        delegate: cellDelegate

        Component.onCompleted:
        {
            selection.onSelect.connect(rules.select)
            rules.selectionAccepted.connect(acceptSelection)
        }

        function acceptSelection(points)
        {
            selection.points = []
        }
    }

    Component
    {
        id: cellDelegate

        Rectangle
        {
            x: gameArea.cellSize * modelData.x
            y: gameArea.cellSize * modelData.y
            width: gameArea.cellSize * 0.8
            height: gameArea.cellSize * 0.8
            color: modelData.symbol === 0 ? "red" : (modelData.symbol === 1 ? "yellow" : "blue")
            property var selected: false
            border.color: "black"
            border.width: selected ? 2 : 0

            Behavior on y {SmoothedAnimation { velocity: 100}}
            Behavior on x {SmoothedAnimation { velocity: 100}}

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    var point = Qt.point(modelData.x, modelData.y)
                    var index = selection.points.indexOf(point)
                    if (index === -1)
                    {
                        selection.points.push(point)
                        parent.selected = true
                    }
                    else
                    {
                        selection.points.splice(index, 1)
                        parent.selected = false
                    }
                    selection.onSelect(selection.points)
                }
            }
        }
    }
}
