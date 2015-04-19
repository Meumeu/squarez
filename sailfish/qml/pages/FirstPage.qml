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
	id: root
	property string coverText: ""
	property bool serverReachable: false

	ListModel {
		id: scoreDates
		ListElement {
			title: QT_TR_NOOP("Today")
			dateMin: "1970-01-01T00:00:00Z"
			dateMax: "1970-01-01T00:00:00Z"
		}
		ListElement {
			title: QT_TR_NOOP("This week")
			dateMin: "1970-01-01T00:00:00Z"
			dateMax: "1970-01-01T00:00:00Z"
		}
		ListElement {
			title: "" // this month
			dateMin: "1970-01-01T00:00:00Z"
			dateMax: "1970-01-01T00:00:00Z"
		}
		ListElement {
			title: "" // last month
			dateMin: "1970-01-01T00:00:00Z"
			dateMax: "1970-01-01T00:00:00Z"
		}
		ListElement {
			title: QT_TR_NOOP("All time")
			dateMin: "1970-01-01T00:00:00Z"
			dateMax: "1970-01-01T00:00:00Z"
		}
	}

	function monthName(date)
	{
		var name = date.toLocaleDateString(Qt.locale(), "MMMM")
		return name.charAt(0).toUpperCase() + name.slice(1);
	}

	function updateDates()
	{
		var t0 = new Date()
		var t1 = new Date()
		t0.setHours(0, 0, 0, 0)
		t1.setHours(23, 59, 59, 999)
		scoreDates.setProperty(0, "dateMin", t0.toISOString())
		scoreDates.setProperty(0, "dateMax", t1.toISOString())

		var delta = (t0.getDay() - Qt.locale().firstDayOfWeek + 7) % 7
		t0 = new Date(t0.getTime() - delta * 86400000);
		t1 = new Date(t0.getTime() + 7 * 86400000);
		scoreDates.setProperty(1, "dateMin", t0.toISOString())
		scoreDates.setProperty(1, "dateMax", t1.toISOString())

		t0 = new Date()
		t1 = new Date()
		t0.setDate(1)
		t0.setHours(0, 0, 0, 0)
		t1.setMonth(t1.getMonth() + 1, 1)
		t1.setHours(0, 0, 0, 0)
		scoreDates.setProperty(2, "dateMin", t0.toISOString())
		scoreDates.setProperty(2, "dateMax", t1.toISOString())

		scoreDates.setProperty(2, "title", monthName(t0))

		t0.setMonth(t0.getMonth() - 1)
		t1.setMonth(t1.getMonth() - 1)
		scoreDates.setProperty(3, "dateMin", t0.toISOString())
		scoreDates.setProperty(3, "dateMax", t1.toISOString())
		scoreDates.setProperty(3, "title", monthName(t0))

		scoreDates.setProperty(4, "dateMin", "1970-01-01T00:00:00Z")
		scoreDates.setProperty(4, "dateMax", new Date().toISOString())
	}

	Component.onCompleted: updateDates()

	Timer {
		interval: 3600
		repeat: Qt.application.state === Qt.ApplicationActive
		onTriggered: {
			updateDates()
			//scores.refresh()
		}
	}

	SilicaFlickable {
		anchors.fill: parent

		SlideshowView {
			id: view
			width: parent.width
			model: scoreDates
			property bool moving: dragging || flicking

			delegate: Component {
				SilicaListView {
					id: scoreList
					HighScores {
						id: scores
						url: "http://squarez.meumeu.org/"
						minDate: dateMin
						maxDate: dateMax
						updateAllowed: !view.moving
					}

					width: parent.width
					height: parent.height
					header: Label {
						width: view.width
						height: contentHeight + Theme.paddingMedium * 2
						verticalAlignment: Text.AlignVCenter
						horizontalAlignment: Text.AlignHCenter
						id: label
						text: qsTr(title)
						font.pixelSize: Theme.fontSizeMedium
						color: Theme.highlightColor
					}
					model: scores
					delegate: Component {
						HighScoreItem {
							playerName: model.playerName
							score: model.score
							date: model.date
						}
					}

					ViewPlaceholder {
						enabled: scores.count === 0 && !scores.loading
						text: qsTr("No high score")
					}

					BusyIndicator {
						running: scores.count === 0 && scores.loading
						anchors.centerIn: parent
						size: BusyIndicatorSize.Large
					}
				}
			}
		}

		SwipeHint {}

		// PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
		PullDownMenu {
			MenuItem
			{
				text: qsTr("How to play")
				onClicked: pageStack.push(Qt.resolvedUrl("TutorialPage.qml"))
			}
			//MenuItem
			//{
			//	text: qsTr("Settings")
			//	onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
			//}
			MenuItem
			{
				text: qsTr("New game")
				onClicked:  pageStack.push("NameInput.qml")
			}
		}

		VerticalScrollDecorator {}
	}

	//onStatusChanged: if (status === PageStatus.Activating) scores.refresh()
}
