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

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

class shader
{
	GLuint vertex_shader_id;
	GLuint fragment_shader_id;
	GLuint program_id;

public:
	shader(const std::string& vertex_shader, const std::string& fragment_shader);
	shader(shader&& rhs);
	shader(const shader& rhs) = delete;
	shader operator=(const shader& ) = delete;
	shader operator=(shader&& ) = delete;
	~shader();
	
	void use();
	static void unuse();
	
	GLint attribute(const std::string& name);
	GLint attribute(const char * name);

	GLint uniform(const std::string& name);
	GLint uniform(const char * name);
	
	GLuint getId() const { return program_id; }
};

#endif // SHADER_H
