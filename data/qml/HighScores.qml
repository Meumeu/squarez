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

import QtQuick 2.4
import QtQuick.Layouts 1.1

import harbour.squarez 1.0

Rectangle {
	color: "#000000"
	id: root
	
	HighScores {
		id: scores
		url: "http://squarez.meumeu.org/"
	}
	
	ListView {
		anchors.fill: parent
		model: scores
		
		headerPositioning: ListView.OverlayHeader
		header: Component {
			Item {
				width: parent.width
				height: header.height
				
				Rectangle {
					anchors.fill: header
					z: 1
					color: "#808080"
				}
				
				RowLayout {
					id: header
					z: 1
					width: parent.width
					Text {
						Layout.minimumWidth: 50
						color: "#ffffff"
						text: qsTr("Score")
					}
					
					Text {
						Layout.fillWidth: true
						color: "#ffffff"
						text: qsTr("Player name")
					}
					
					Text {
						Layout.minimumWidth: 150
						color: "#ffffff"
						text: qsTr("Date")
					}
				}
				
			}
		}
		
		delegate: Component {
			RowLayout {
				width: parent.width
				Text {
					Layout.minimumWidth: 50
					color: "#ffffff"
					text: score
				}
				
				Text {
					Layout.fillWidth: true
					color: "#ffffff"
					text: playerName
				}
				
				Text {
					Layout.minimumWidth: 150
					color: "#ffffff"
					text: date.toLocaleString(Locale.ShortFormat)
				}
			}
		}
	}
	
	focus: true
	Keys.onPressed: {
		if (event.key == Qt.Key_Escape)
			stackView.pop()
	}
}