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

Page {
	id: page
	property string coverText: ""
	property bool serverReachable: false
	Settings { id: settings }

	/*SilicaListView {
		model: rules.highScores;
		anchors.fill: parent
		header: PageHeader {
			title: "High scores"
		}
		delegate: BackgroundItem {
			id: delegate

			Label {
				x: Theme.paddingLarge
				text: name
				anchors.verticalCenter: parent.verticalCenter
				color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
			}
			Label
			{
				text: Qt.formatDate(date)
				anchors.centerIn: parent
			}
			Label
			{
				width: parent.width - Theme.paddingLarge
				text: score
				anchors.verticalCenter: parent.verticalCenter
				horizontalAlignment: Text.AlignRight
			}
		}*/
	SilicaListView {
		model: 3
		anchors.fill: parent
		header: PageHeader {
			title: qsTr("Plop")
		}
		delegate: Component {
			BackgroundItem {
				id: delegate

				Label {
					x: Theme.paddingLarge
					text: index
					anchors.verticalCenter: parent.verticalCenter
					color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
				}
			}
		}

		// PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
		PullDownMenu {
			MenuItem
			{
				text: qsTr("How to play")
				onClicked: pageStack.push(Qt.resolvedUrl("TutorialPage.qml"))
			}

			/*MenuLabel
			{
				text: "Multiplayer (server unreachable)"
				visible: ! page.serverReachable
			}
			MenuItem
			{
				text: "Multiplayer"
				visible: page.serverReachable
				onClicked: {
					var dialog = pageStack.push("../pages/NameInput.qml")
					dialog.title = "Enter your name"
					dialog.accepted.connect(function() {
						var page = pageStack.replace(Qt.resolvedUrl("MultiPlayerPage.qml"))
						page.playerName = dialog.name
					})
				}
			}*/
			MenuItem
			{
				text: qsTr("New game")
				onClicked:  pageStack.push("NameInput.qml")
			}
		}

		VerticalScrollDecorator {}
	}
}


