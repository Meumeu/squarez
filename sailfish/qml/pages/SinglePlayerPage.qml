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

import QtQuick 2.1
import Sailfish.Silica 1.0

import harbour.squarez 1.0
import "../squarez"

Page
{
	property string coverText: header.title
	property alias playerName: rules.playerName
	property alias rulesType: rules.type
	id: page

	function networkError()
	{
		message.text = qsTr("Network error: score will not be saved")
	}

	Component.onCompleted: {
		rules.onNetworkError.connect(networkError);
	}

	Rules {
		id: rules
		url: "http://squarez.meumeu.org/"
		paused: !applicationActive || status !== PageStatus.Active

	}

	Column {
		id: column
		width: parent.width
		PageHeader
		{
			id: header
			title: qsTr("Score: %1").arg(rules.score)
		}

		GameArea
		{
			rules: rules
			width: parent.width
			height: width
			font.pixelSize: Theme.fontSizeExtraLarge
			textColor: Theme.primaryColor

			BusyIndicator {
				anchors.centerIn: parent
				running: !rules.ready
				size: BusyIndicatorSize.Large
			}
		}

		TimerArea
		{
			id: timer
			rules: rules
			width: parent.width
			visible: !rules.gameOver
		}

		Text {
			id: message
			anchors.left: parent.left
			anchors.right: parent.right
			color: Theme.primaryColor
			font.family: Theme.fontFamily
			font.pixelSize: Theme.fontSizeMedium
			wrapMode: Text.Wrap
			text: ""
			horizontalAlignment: Text.AlignHCenter

			SequentialAnimation {
				id: messageFadeOut
				PauseAnimation {
					duration: 2000
				}
				NumberAnimation {
					target: message
					property: "opacity"
					from: 1
					to: 0
					duration: 1000
				}
			}

			onTextChanged: messageFadeOut.restart()
		}
	}

}
