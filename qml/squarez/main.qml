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
        property var cells: []
        signal onSelect(variant points)

        function clickCell(cell)
        {
            var index = cells.indexOf(cell)
            if (index === -1)
            {
                cells.push(cell)
                cell.selected = true
            }
            else
            {
                cells.splice(index, 1)
                cell.selected = false
            }
            var points = cells.map(function(item){return Qt.point(item.logical_x, item.logical_y)})
            onSelect(points)
        }
        function reset()
        {
            cells.forEach(function(cell){cell.selected = false})
            cells = []
        }
    }

    Repeater
    {
        model:rules.board
        delegate: cellDelegate

        Component.onCompleted:
        {
            selection.onSelect.connect(rules.select)
            rules.selectionAccepted.connect(selection.reset)
        }
    }

    Component
    {
        id: cellDelegate

        Rectangle
        {
            property var logical_x: modelData.x
            property var logical_y: modelData.y
            x: gameArea.cellSize * modelData.x
            y: gameArea.cellSize * modelData.y
            width: gameArea.cellSize * 0.8
            height: gameArea.cellSize * 0.8
            radius: gameArea.cellSize * 0.2
            antialiasing: true
            color: modelData.symbol === 0 ? "red" : (modelData.symbol === 1 ? "yellow" : "blue")
            property var selected: false
            border.color: "black"
            border.width: selected ? 2 : 0

            Behavior on y {SmoothedAnimation { velocity: gameArea.cellSize * 4}}
            Behavior on x {SmoothedAnimation { velocity: gameArea.cellSize * 4}}

            MouseArea
            {
                anchors.fill: parent
                onClicked: {selection.clickCell(parent)}
            }
        }
    }
}
