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

Rectangle
{
	property real ratio: 1
	property real centerX: 0
	property real centerY: 0
	height: width
	//width: 0
	x: (centerX + 0.5) * ratio - width / 2
	y: (centerY + 0.5) * ratio - height / 2
	visible: false
	function setPoints(points)
	{
		// We assume it is really a square, we only need to compute the size, center and angle
		centerX = 0;
		centerY = 0
		for (var i = 0 ; i < 4 ; i++)
		{
			centerX += points[i].x/4;
			centerY += points[i].y/4;
		}

		var squareSize = (points[0].x - points[1].x)*(points[0].x - points[1].x) + (points[0].y - points[1].y)*(points[0].y - points[1].y);
		var side = 1;
		var otherSize = (points[0].x - points[2].x)*(points[0].x - points[2].x) + (points[0].y - points[2].y)*(points[0].y - points[2].y);
		if (otherSize < squareSize)
		{
				squareSize = otherSize;
				side = 2;
		}
		squareSize = Math.sqrt(squareSize)
		widthAnimation.to = ratio * squareSize + border.width
		widthAnimation.restart()
		rotation = Math.atan2(points[side].x - points[0].x, points[0].y - points[side].y) * 180 / Math.PI
		visible = true
	}

	NumberAnimation on width {
		id: widthAnimation
		from: 0
		to: 0
		duration: 800
		easing.type: Easing.OutElastic
	}
}
