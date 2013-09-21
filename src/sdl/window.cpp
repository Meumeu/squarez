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
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#ifdef USE_SDL2
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
	int w, h;
	SDL_GetWindowSize(_window, &w, &h);
	return h;
}

int squarez::Window::width()
{
	int w, h;
	SDL_GetWindowSize(_window, &w, &h);
	return w;
}
#else

squarez::Window::Window()
{
	const SDL_VideoInfo * video = SDL_GetVideoInfo();
	if (!video)
		throw std::runtime_error(SDL_GetError());
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	if (SDL_SetVideoMode(video->current_w, video->current_h, video->vfmt->BitsPerPixel, SDL_OPENGL | SDL_FULLSCREEN) == 0)
		throw std::runtime_error(SDL_GetError());

}

squarez::Window::~Window()
{

}

int squarez::Window::height()
{
	const SDL_VideoInfo * video = SDL_GetVideoInfo();
	return video->current_h;
}

int squarez::Window::width()
{
	const SDL_VideoInfo * video = SDL_GetVideoInfo();
	return video->current_w;
	
}

#endif



