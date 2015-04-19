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
	id: page
	property string coverText: header.title
	property alias playerName: multiPlayerRules.playerName
	Settings {id: settings}
	MultiPlayer
	{
		id: multiPlayerRules
		url: settings.url
	}

	Column
	{
		id: content
		width: parent.width
		PageHeader
		{
			id: header
			title: "Score: " + multiPlayerRules.score
		}

		GameArea
		{
			rules: multiPlayerRules
			width: parent.width
			height: width
		}

		TimerArea
		{
			id: timer
			rules: multiPlayerRules
			round: true
			width: parent.width
		}
	}
	SilicaListView {
		model: multiPlayerRules.highScores;
		anchors.top: content.bottom
		anchors.bottom: page.bottom
		anchors.left: page.left
		anchors.right: page.right
		delegate: BackgroundItem {
			id: delegate
			highlighted: name === multiPlayerRules.playerName && score == multiPlayerRules.score
			height: Theme.fontSizeSmall + Theme.paddingSmall

			Label {
				x: Theme.paddingLarge
				font.pixelSize: Theme.fontSizeSmall
				text: 1+index + ": " + name
				anchors.verticalCenter: parent.verticalCenter
				color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
			}
			Label
			{
				width: parent.width - Theme.paddingLarge
				font.pixelSize: Theme.fontSizeSmall
				text: score
				anchors.verticalCenter: parent.verticalCenter
				horizontalAlignment: Text.AlignRight
			}
		}

		VerticalScrollDecorator {}
	}
}
