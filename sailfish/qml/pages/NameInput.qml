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

Dialog
{
	property string coverText: ""
	id: dialog
	acceptDestination: Qt.resolvedUrl("SinglePlayerPage.qml")
	acceptDestinationAction: PageStackAction.Replace

	SettingValue {
		key: "playerName"
		defaultValue: ""
		id: playerNameSetting
	}

	SettingValue {
		key: "online"
		defaultValue: "true"
		id: onlineSetting
	}

	onAccepted: {
		dialog.acceptDestinationInstance.playerName = playerName.text.trim();
		if (playOnline.checked)
			dialog.acceptDestinationInstance.rulesType = "onlineSinglePlayer";
		else
			dialog.acceptDestinationInstance.rulesType = "singlePlayer";

		onlineSetting.value = playOnline.checked
		playerNameSetting.value = playerName.text.trim()
	}

	onOpened: {
		playOnline.checked = onlineSetting.value === "true"
		playerName.text = playerNameSetting.value
	}

	canAccept: playerName.text !== ""

	Column
	{
		anchors.fill: parent

		DialogHeader {
			acceptText: qsTr("Start")
		}

		TextSwitch {
			id: playOnline
			width: parent.width
			text: qsTr("Play online")
		}

		TextField {
			id: playerName
			width: parent.width
			focus: true
			placeholderText: qsTr("Enter your name")
			EnterKey.enabled: true
			EnterKey.onClicked: dialog.accept()
		}
	}
}
