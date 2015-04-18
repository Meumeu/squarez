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

CoverBackground {

	Column
	{
		anchors.fill: parent
		anchors.topMargin: parent.height / 10
		anchors.bottomMargin: parent.height / 10
		spacing: parent.height / 20

		Image {
			anchors.horizontalCenter: parent.horizontalCenter
			width: parent.width * 0.9
			height: width
			sourceSize.height: height
			sourceSize.width: width
			source: "../../img/harbour-squarez.svg"
			opacity: 0.3
		}

		Label {
			anchors.horizontalCenter: parent.horizontalCenter
			id: custom
			text: pageStack.currentPage.coverText
			visible: text !== ""
		}
	}
}
