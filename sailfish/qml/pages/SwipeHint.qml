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

Loader {
	anchors.fill: parent

	active: true //counter.active
	sourceComponent: Component {
		Item {
			property bool pageActive: root.status == PageStatus.Active
			onPageActiveChanged: {
				if (pageActive) {
					timer.restart()
					//counter.increase()
					pageActive = false
				}
			}

			anchors.fill: parent
			Timer {
				id: timer
				interval: 500
				onTriggered: touchInteractionHint.restart()
				running: true
			}

			InteractionHintLabel {
				text: qsTr("Swipe to see other high scores")
				anchors.bottom: parent.bottom
				opacity: touchInteractionHint.running ? 1.0 : 0.0
				Behavior on opacity { FadeAnimation { duration: 1000 } }
			}
			TouchInteractionHint {
				id: touchInteractionHint

				direction: TouchInteraction.Left
				//anchors.top: parent.top
				anchors.verticalCenter: parent.verticalCenter
			}
		}
	}

	FirstTimeUseCounter {
		id: counter
		limit: 3
		defaultValue: 1 // display hint twice for existing users
		key: "/apps/harbour-squarez/high_score_hint_count"
	}
}
