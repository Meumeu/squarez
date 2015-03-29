/*
 * Squarez puzzle game
 * Copyright (C) 2013-2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
 * Copyright (C) 2013-2015  Patrick Nicolas <patricknicolas@laposte.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

import QtQuick 2.0
import QtQuick.Particles 2.0

Item {
	id: gameArea

	property real cellSize: Math.min(gameArea.width, gameArea.height) / rules.boardSize
	property var rules
	property bool animate: true
	property var colors: [Qt.rgba(255,0,0,1), Qt.rgba(255,255,0,1), Qt.rgba(0,0,255,1)]
	onWidthChanged: animate = false

	property var selection: []
	signal onSelect(variant points)

	Repeater
	{
		model: rules
		delegate: cellDelegate

		Component.onCompleted:
		{
			rules.onNameRequired.connect(onNameRequired)
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

		Image
		{
			id: cell
			property int logical_x: modelData.x
			property int logical_y: modelData.y
			property int symbol: modelData.symbol

			x: gameArea.cellSize * (modelData.x + 0.1)
			y: gameArea.cellSize * (modelData.y + 0.1)
			width: gameArea.cellSize * 0.8
			height: gameArea.cellSize * 0.8
			antialiasing: true
			source: "qrc:/img/shape" + symbol + (modelData.selected ? "-selected" : "") + ".svg"
			sourceSize.width: width
			sourceSize.height: height

			Emitter
			{
				z: 100
				id: particles
				system: particleSystem
				anchors.fill: cell
				lifeSpan: 500
				emitRate: 20
				endSize: 0
				group: ""+symbol
				velocity: TargetDirection { magnitude: -gameArea.cellSize/3; targetItem: cell}
				enabled: modelData.selected && ! rules.pause
				shape: RectangleShape {fill: false}
			}

			Behavior on y {SmoothedAnimation { velocity: gameArea.cellSize * 4} enabled: gameArea.animate}
			Behavior on x {SmoothedAnimation { velocity: gameArea.cellSize * 4} enabled: gameArea.animate}

			NumberAnimation on opacity { from: 0 ; to: 1; duration: 500 }

			MouseArea
			{
				anchors.fill: cell
				onClicked: {
					gameArea.animate = true
					modelData.clicked()
				}
			}
		}
	}

	SingleSquare
	{
		id: selectedSquare
		color: "#00000000"
		border.color: "#80000000"
		border.width: gameArea.cellSize / 3
		ratio: gameArea.cellSize
		Behavior on width
		{
			SequentialAnimation
			{
				PropertyAction {value: 0}
				NumberAnimation {duration: 800; from: 0; easing.type: Easing.OutElastic}
			}
		}
	}

	ParticleSystem
	{
		z: 100
		id: particleSystem
		anchors.fill: parent
//         running: applicationActive && status === PageStatus.Active
		running: true

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
		ImageParticle
		{
			source: "../../img/particle.png"
			groups: ["selection"]
			color: "#202020"
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
		Emitter
		{
			id: selectionEmitter
			lifeSpan: 800
			endSize: 0
			enabled: false
			velocity: AngleDirection { magnitudeVariation: gameArea.cellSize / 2; angleVariation: 360}
			group: "selection"
			property var points: []
			function my_burst(points_)
			{
				for (var i = 0 ; i < points_.length ; i++)
				{
					points[i] = Qt.point((points_[i].x + 0.5) * gameArea.cellSize, (points_[i].y + 0.5) * gameArea.cellSize)
				}
				points.sort(function(a,b) {
					if (a.x < b.x)
						return -1;
					if (a.x === b.x && a.y < b.y)
						return -1;
					return 1;
				})
				var p3 = points[3]
				points[3] = points[2]
				points[2] = p3
				burst(Math.sqrt(Math.pow(points[1].x - points[0].x, 2) + Math.pow(points[1].y - points[0].y, 2))* 100 / gameArea.cellSize)
			}
			onEmitParticles: {
				for (var i = 0; i < particles.length ; i++)
				{
					var side = Math.floor(Math.random() * 4)
					var l = Math.random();
					particles[i].red = 1 - Math.random() * 0.5
					particles[i].green = 1 - Math.random() * 0.5
					particles[i].blue = 1 - Math.random() * 0.5
					particles[i].x = points[side].x * l + points[(side + 1) % 4].x * (1-l)
					particles[i].y = points[side].y * l + points[(side + 1) % 4].y * (1-l)
				}
			}
		}
	}
	
	MouseArea {
		anchors.fill: parent
		propagateComposedEvents: true
		acceptedButtons: Qt.LeftButton | Qt.RightButton
		onPressAndHold: rules.resetSelection()
		onClicked: {
			if (mouse.button == Qt.RightButton)
				rules.resetSelection()
			else
				mouse.accepted = false;
		}
	}
}
