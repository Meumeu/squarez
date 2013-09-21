/*
 * Squarez puzzle game
 * Copyright (C) 2013  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#ifndef SQUAREZ_CELL_H
#define SQUAREZ_CELL_H

#include "animatable.h"
#include "colour.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

class shader;

namespace squarez {
	
class Cell
{
private:
	shader * _shader;
	GLuint _vbo_id;
	GLint cell_coords_id;
	GLint coords_id;
	GLint color_id;
	GLint border_color_id;
	GLint border_radius_id;
	GLint border_size_id;
	
public:
	Cell(shader * shader, GLuint vbo_id);
	
	
	Animatable x, y;
	Colour colour;
	Colour border_colour;
	Animatable alpha;
	Animatable size;
	float angle;
	float border_radius;
	float border_size;
	float border_alpha;
	int symbol;
	
	void render(float xmin, float ymin, float xmax, float ymax);
};

}

#endif // SQUAREZ_CELL_H
