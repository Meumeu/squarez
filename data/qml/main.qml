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
import QtQuick.Window 2.0
import QtQuick.Controls 1.3

import harbour.squarez 1.0

ApplicationWindow {
	id: root
	width: 600; height: 400
	visible: true
	title: qsTr("Squarez")

	Rules {
		id: rules
// 		type: "singlePlayer"
		type: "onlineSinglePlayer"
		url: "http://heracles/"
		playerName: "test"
	}
	
	Rectangle {
		anchors.fill: parent
		color: "#000000"
	}
	GameArea {
		id: area
		anchors.left: parent.left
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		width: height
		rules: rules
	}
	
	property var msLeft
	
	Timer {
		interval: 16
		repeat: true
		running: !rules.paused && !rules.gameOver
		onTriggered: {
			root.msLeft = rules.msLeft
		}
	}
	
	Column {
		anchors.left: area.right
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		
		Text {
			color: "#ffffff"
			text: "Score: " + rules.score
		}
		
		Text {
			color: "#ffffff"
			text: "Player name: " + rules.playerName
		}
		
		Text {
			color: "#ffffff"
			text: "Time remaining: " + (root.msLeft / 1000) + " s"
		}
	}
	
	Rectangle {
		anchors.fill: parent
		color: rules.gameOver ? "#80000000" : rules.paused ? "#ff000000" : "#00000000"
		Text {
			anchors.centerIn: parent
			color: "#ffffff"
			font.pointSize: 36
			text: rules.gameOver ? "Game over" : rules.paused ? "Paused" : ""
			visible: rules.paused || rules.gameOver
		}
		
		focus: true
		Keys.onPressed: {
			if (event.key == Qt.Key_Pause)
				rules.paused = !rules.paused;
		}
	}
}