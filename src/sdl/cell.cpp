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

#include "cell.h"
#include "shader.h"
#include <math.h>

squarez::Cell::Cell(shader * sh, GLuint id) : _shader(sh), _vbo_id(id)
{
	cell_coords_id = _shader->attribute("cell_coords");
	coords_id = _shader->attribute("coords");
	color_id = _shader->uniform("color");
	border_color_id = _shader->uniform("border_color");
	border_radius_id = _shader->uniform("border_radius");
	border_size_id = _shader->uniform("border_size");
}


void squarez::Cell::render(float xmin, float ymin, float xmax, float ymax)
{
	const GLuint indices[] = 
	{
		0, 1, 3,
		3, 2, 0
	};
	
	GLfloat vertices[] = 
	{
		0, 0, -1,  1,
		0, 0, -1, -1,
		0, 0,  1,  1,
		0, 0,  1, -1,
	};
	
	for (int i = 0; i < 4; ++i)
	{
		vertices[i*4]   = (x + 0.5 * size * (vertices[i*4+2] * cos(angle) + vertices[i*4+3] * sin(angle)) - xmin) / (xmax - xmin) * 2 - 1;
		vertices[i*4+1] = (y + 0.5 * size * (-vertices[i*4+2] * sin(angle) + vertices[i*4+3] * cos(angle)) - ymin) / (ymax - ymin) * -2 + 1;
	}
	
	_shader->use();
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(cell_coords_id);
	glEnableVertexAttribArray(coords_id);
	glEnableVertexAttribArray(color_id);
	glUniform1f(border_radius_id, border_radius);
	glUniform4f(color_id, colour.r, colour.g, colour.b, alpha);
	glUniform1f(border_size_id, border_size);
	glUniform4f(border_color_id, border_colour.r, border_colour.g, border_colour.b, border_alpha);
	
	glVertexAttribPointer(coords_id, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
	glVertexAttribPointer(cell_coords_id, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
	
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, indices);

	glDisableVertexAttribArray(cell_coords_id);
	glDisableVertexAttribArray(coords_id);
	glDisableVertexAttribArray(color_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	_shader->unuse();
}
