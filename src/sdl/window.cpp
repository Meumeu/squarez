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

#include "window.h"
#include <stdexcept>
#include <iostream>

squarez::Window::Window() : _window(nullptr), _gl_context(nullptr)
{
	_window = SDL_CreateWindow("Squarez", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL);
// 	_window = SDL_CreateWindow("Squarez", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!_window)
		throw std::runtime_error(SDL_GetError());
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	_gl_context = SDL_GL_CreateContext(_window);
	if (!_gl_context)
		throw std::runtime_error(SDL_GetError());
	
	
// 	SDL_DisplayMode mode;
// 	SDL_GetWindowDisplayMode(_window, &mode);
	
	std::cout << "Mode: " << width() << "x" << height() << std::endl;
	
}

squarez::Window::~Window()
{
	if (_gl_context)
		SDL_GL_DeleteContext(_gl_context);
	
	if (_window)
		SDL_DestroyWindow(_window);
}

int squarez::Window::height()
{
// 	SDL_DisplayMode mode;
// 	SDL_GetWindowDisplayMode(_window, &mode);
// 	return mode.h;
	int w, h;
	SDL_GetWindowSize(_window, &w, &h);
	return h;
}

int squarez::Window::width()
{
// 	SDL_DisplayMode mode;
// 	SDL_GetWindowDisplayMode(_window, &mode);
// 	return mode.w;
	int w, h;
	SDL_GetWindowSize(_window, &w, &h);
	return w;
}
