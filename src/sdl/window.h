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

#ifndef SQUAREZ_WINDOW_H
#define SQUAREZ_WINDOW_H

#include <SDL2/SDL.h>

namespace squarez {

class Window
{
private:
	SDL_Window * _window;
	SDL_GLContext _gl_context;
	
public:
	Window();
	~Window();
	
	SDL_Window * getWindow() const { return _window; }
	int width();
	int height();
};
}

#endif // SQUAREZ_WINDOW_H
