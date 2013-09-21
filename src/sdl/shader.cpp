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

#include "shader.h"
#include <fstream>
#include <stdexcept>
#include <memory>
#include <string.h>

static void compile_shader(GLuint id, const std::string& filename)
{
	std::ifstream f(filename);
	
	f.seekg(0, std::ios::end);
	int size = f.tellg();
	char src[size];
	f.seekg(0);
	f.read(src, size);
	
	const char * data = src;
	
	glShaderSource(id, 1, &data, &size);
	glCompileShader(id);
	
	int status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		int len = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
		std::unique_ptr<char[]> log(new char[len]);
		glGetShaderInfoLog(id, len, &len, log.get());

		std::string log_str(log.get(), len);

		throw std::runtime_error("shader compilation error:\n" + log_str);
	}
}

shader::shader(const std::string & vertex_shader, const std::string & fragment_shader) :
	vertex_shader_id(0), fragment_shader_id(0), program_id(0)
{
	vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

	if (!vertex_shader_id)
		throw std::runtime_error("cannot create vertex shader");

	fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	if (!fragment_shader_id)
		throw std::runtime_error("cannot create fragment shader");

	compile_shader(vertex_shader_id, vertex_shader);
	compile_shader(fragment_shader_id, fragment_shader);

	program_id = glCreateProgram();

	if (!program_id)
		throw std::runtime_error("cannot create program");

	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);

	GLint status;
	glGetProgramiv(program_id, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		int len = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
		std::unique_ptr<char[]> log(new char[len]);
		glGetProgramInfoLog(program_id, len, &len, log.get());

		std::string log_str(log.get(), len);

		throw std::runtime_error("shader link error:\n" + log_str);
	}
}

shader::shader(shader && rhs)
{
	vertex_shader_id = rhs.vertex_shader_id;
	fragment_shader_id = rhs.fragment_shader_id;
	program_id = rhs.program_id;

	rhs.vertex_shader_id = 0;
	rhs.fragment_shader_id = 0;
	rhs.program_id = 0;
}

shader::~shader()
{
	glDetachShader(program_id, vertex_shader_id);
	glDetachShader(program_id, fragment_shader_id);
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
	glDeleteProgram(program_id);
}

void shader::use()
{
	glUseProgram(program_id);
}

void shader::unuse()
{
	glUseProgram(0);
}

GLint shader::attribute(const char * name)
{
    GLint ret = glGetAttribLocation(program_id, name);

//     if (ret == -1)
//         throw std::runtime_error(std::string("cannot bind attribute ") + name);

    return ret;
}

GLint shader::attribute(const std::string & name)
{
    return attribute(name.c_str());
}

GLint shader::uniform(const char * name)
{
    GLint ret = glGetUniformLocation(program_id, name);

//     if (ret == -1)
//         throw std::runtime_error(std::string("cannot bind uniform ") + name);

    return ret;
}

GLint shader::uniform(const std::string & name)
{
    return uniform(name.c_str());
}
