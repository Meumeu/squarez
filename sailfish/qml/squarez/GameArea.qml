import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQuick.Particles 2.0

Item {
    id: gameArea

    property real cellSize: Math.min(gameArea.width, gameArea.height) / rules.board.size
    property var rules
    property bool animate: true
    property var colors: [Qt.rgba(255,0,0,1), Qt.rgba(255,255,0,1), Qt.rgba(0,0,255,1)]
    onWidthChanged: animate = false

    property var selection: []
    signal onSelect(variant points)

    property var clickCell: function(cell)
    {
        var index = selection.indexOf(cell)
        if (index === -1)
        {
            selection.push(cell)
            cell.selected = true
        }
        else
        {
            selection.splice(index, 1)
            cell.selected = false
        }
        var points = selection.map(function(item){return Qt.point(item.logical_x, item.logical_y)})
        onSelect(points)
    }
    function resetSquare()
    {
        selectedSquare.visible = false
    }
    function resetSelection()
    {
        selection.forEach(function(cell){cell.selected = false})
        selection = []
    }


    Repeater
    {
        model:rules.board
        delegate: cellDelegate

        Component.onCompleted:
        {
            gameArea.onSelect.connect(rules.select)
            rules.selectionAccepted.connect(selectedSquare.setPoints)
            rules.selectionApplied.connect(gameArea.resetSelection)
            rules.selectionRejected.connect(gameArea.resetSelection)
            rules.selectionApplied.connect(gameArea.resetSquare)
            rules.selectionRejected.connect(gameArea.resetSquare)
            rules.nameRequired.connect(onNameRequired)
        }

        function onNameRequired(previousName)
        {
            var dialog = pageStack.push("../pages/NameInput.qml")
            if (previousName)
                dialog.name = previousName
            dialog.title = "Save high score"
            dialog.coverText = "Score: " + gameArea.rules.score
            dialog.accepted.connect(function() {
                gameArea.rules.playerName = dialog.name;
            })
        }

        onItemRemoved:
        {
            gameArea.animate = true
            burstEmitter.my_burst(item);
        }
    }

    Component
    {
        id: cellDelegate

        Rectangle
        {
            id: cell
            property int logical_x: modelData.x
            property int logical_y: modelData.y
            property bool selected: false
            property int symbol: modelData.symbol

            x: gameArea.cellSize * (modelData.x + 0.1)
            y: gameArea.cellSize * (modelData.y + 0.1)
            width: gameArea.cellSize * 0.8
            height: gameArea.cellSize * 0.8
            radius: gameArea.cellSize * 0.2
            antialiasing: true
            color: selected ? Qt.darker(gameArea.colors[symbol]) : gameArea.colors[symbol]

            Emitter
            {
                id: particles
                system: particleSystem
                anchors.fill: cell
                lifeSpan: 500
                emitRate: 20
                endSize: 0
                group: ""+symbol
                velocity: TargetDirection { magnitude: -gameArea.cellSize/3; targetItem: cell}
                enabled: cell.selected && ! rules.pause
                shape: RectangleShape {fill: false}
            }

            Behavior on y {SmoothedAnimation { velocity: gameArea.cellSize * 4} enabled: gameArea.animate}
            Behavior on x {SmoothedAnimation { velocity: gameArea.cellSize * 4} enabled: gameArea.animate}

            NumberAnimation on opacity { from: 0 ; to: 1; duration: 500 }

            MouseArea
            {
                anchors.fill: cell
                onClicked: gameArea.clickCell(cell)
                onPressAndHold: gameArea.resetSelection()
            }
        }
    }

    Rectangle
    {
        property real centerX: 0
        property real centerY: 0
        id: selectedSquare
        height: width
        width: 0
        x: (centerX + 0.5) * gameArea.cellSize - width / 2
        y: (centerY + 0.5) * gameArea.cellSize - height / 2
        color: "#00000000"
        border.color: "#80000000"
        border.width: gameArea.cellSize / 3
        visible: false
        function setPoints(points)
        {
            gameArea.resetSelection()
            // We assume it is really a square, we only need to compute the size, center and angle
            centerX = 0;
            centerY = 0
            for (var i = 0 ; i < 4 ; i++)
            {
                centerX += points[i].x/4;
                centerY += points[i].y/4;
            }

            var squareSize = (points[0].x - points[1].x)*(points[0].x - points[1].x) + (points[0].y - points[1].y)*(points[0].y - points[1].y);
            var side = 1;
            var otherSize = (points[0].x - points[2].x)*(points[0].x - points[2].x) + (points[0].y - points[2].y)*(points[0].y - points[2].y);
            if (otherSize < squareSize)
            {
                    squareSize = otherSize;
                    side = 2;
            }
            squareSize = Math.sqrt(squareSize)
            width = gameArea.cellSize * squareSize + border.width
            rotation = Math.atan2(points[side].x - points[0].x, points[0].y - points[side].y) * 180 / Math.PI
            visible = true
        }
        Behavior on width
        {
            SequentialAnimation
            {
                PropertyAction {value: 0}
                NumberAnimation {duration: 800; easing.type: Easing.OutElastic}
            }
        }
    }

    ParticleSystem
    {
        id: particleSystem
        anchors.fill: parent
        running: applicationActive && status === PageStatus.Active

        ImageParticle
        {
            source: "../../img/particle.png"
            groups: ["0"]
            color: Qt.lighter(gameArea.colors[0])
        }
        ImageParticle
        {
            source: "../../img/particle.png"
            groups: ["1"]
            color: Qt.lighter(gameArea.colors[1])
        }
        ImageParticle
        {
            source: "../../img/particle.png"
            groups: ["2"]
            color: Qt.lighter(gameArea.colors[2])
        }

        Gravity
        {
            magnitude: gameArea.cellSize
        }
        Emitter
        {
            id: burstEmitter
            width: gameArea.cellSize
            height: gameArea.cellSize
            lifeSpan: 500
            endSize: 0
            velocity: AngleDirection { magnitude: 2*gameArea.cellSize; angleVariation: 360}
            enabled: false
            function my_burst(target)
            {
                group = ""+target.symbol
                x = target.x
                y = target.y
                burst(100)
            }
        }
    }
}
