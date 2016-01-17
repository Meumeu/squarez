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
import Sailfish.Silica 1.0

import harbour.squarez 1.0
import "../squarez"

Page
{
	ListModel {
		ListElement {
			text: QT_TR_NOOP("Squarez rules\n(click to continue)")
		}
		ListElement {
			text: QT_TR_NOOP("Spot similar elements at the edges of a square shape")
		}
		ListElement {
			text: QT_TR_NOOP("Bigger squares give more points")
		}
		ListElement {
			text: QT_TR_NOOP("Squares not aligned with the grid give double score")
		}
		ListElement {
			text: QT_TR_NOOP("Time is limited\nEach square you select refills time based on its score")
		}
		ListElement {
			text: QT_TR_NOOP("Total amount of available time gets shorter as you progress")
		}
	}

	property string coverText: ""
	Rules {
		id: tutorialRules
		type: "tutorial"
		paused: !applicationActive || status !== PageStatus.Active
	}

	Column {
		id: column
		width: parent.width
		PageHeader
		{
			id: header
			title: qsTr("Score: %1").arg(tutorialRules.score)
		}

		GameArea
		{
			rules: tutorialRules
			width: parent.width
			height: width
			font.pixelSize: Theme.fontSizeExtraLarge
			textColor: Theme.primaryColor
		}

		TimerArea
		{
			id: timer
			rules: tutorialRules
			width: parent.width
		}

		Text
		{
			anchors.left: parent.left
			anchors.right: parent.right
			color: Theme.primaryColor
			font.family: Theme.fontFamily
			font.pixelSize: Theme.fontSizeSmall
			wrapMode: Text.Wrap
			text: qsTr(tutorialRules.message)
			horizontalAlignment: Text.AlignHCenter
		}
	}

	MouseArea
	{
		anchors.fill: parent
		onClicked: if (tutorialRules.message !== "") tutorialRules.next()
	}

	Timer
	{
		interval: 1000;
		repeat: true
		running: tutorialRules.message === "" && applicationActive && status === PageStatus.Active
		onTriggered: tutorialRules.next()
	}
}
