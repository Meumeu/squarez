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

BackgroundItem {
	property alias playerName: playerNameLabel.text
	property alias score: scoreLabel.text
	property var date

	Column {
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.leftMargin: Theme.paddingMedium
		anchors.rightMargin: Theme.paddingMedium
		id: delegate

		Row {
			spacing: Theme.paddingMedium
			Label {
				id: playerNameLabel
				width: delegate.width - scoreLabel.width - Theme.paddingMedium
				font.pixelSize: Theme.fontSizeMedium
				truncationMode: TruncationMode.Fade
			}
			Label {
				id: scoreLabel
				font.pixelSize: Theme.fontSizeMedium
			}
		}

		Label {
			text: (new Date() - date) < 86400000 ? date.toLocaleTimeString(Qt.locale(), Locale.ShortFormat) : date.toLocaleDateString(Qt.locale(), Locale.LongFormat)
			color: Theme.secondaryColor
			font.pixelSize: Theme.fontSizeExtraSmall
		}
	}
}
