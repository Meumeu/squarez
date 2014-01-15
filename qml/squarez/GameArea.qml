import QtQuick 2.0

Rectangle {
    id: gameArea

    property real cellSize: Math.min(gameArea.width, gameArea.height) / rules.board.size
    property var rules
    property bool animate: true
    onWidthChanged: animate = false

    Item
    {
        id:selection
        property var cells: []
        signal onSelect(variant points)

        function clickCell(cell)
        {
            gameArea.animate = true
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
            rules.nameRequired.connect(onNameRequired)
        }

        function onNameRequired(previousName)
        {
            nameInput.text = previousName
            nameInputArea.visible = true
        }
    }

    Component
    {
        id: cellDelegate

        Rectangle
        {
            property int logical_x: modelData.x
            property int logical_y: modelData.y
            property bool selected: false

            x: gameArea.cellSize * modelData.x
            y: gameArea.cellSize * modelData.y
            width: gameArea.cellSize * 0.8
            height: gameArea.cellSize * 0.8
            radius: gameArea.cellSize * 0.2
            antialiasing: true
            color: modelData.symbol === 0 ? "red" : (modelData.symbol === 1 ? "yellow" : "blue")
            border.color: "black"
            border.width: selected ? 2 : 0

            Behavior on y {SmoothedAnimation { velocity: gameArea.cellSize * 4} enabled: gameArea.animate}
            Behavior on x {SmoothedAnimation { velocity: gameArea.cellSize * 4} enabled: gameArea.animate}

            MouseArea
            {
                anchors.fill: parent
                onClicked: {selection.clickCell(parent)}
            }
        }
    }

    Rectangle
    {
        anchors.fill: parent
        visible: gameArea.rules.gameOver
        color: "#000000A0"
        Text
        {
            text: "Game over"
            anchors.centerIn: parent
        }
    }
    Rectangle
    {
        id: nameInputArea
        anchors.fill: parent
        visible: false
        color: "#A0A0A0"
        Column
        {
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Enter name"
            }
            TextInput
            {
                width: parent.width
                id: nameInput
                onAccepted: {
                    nameInputArea.visible = false;
                    gameArea.rules.playerName = nameInput.text;
                }
            }
        }
    }
}
