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

#ifdef USE_SDL2
#include <SDL2/SDL.h>
#else
#include <SDL/SDL.h>
#endif

#include "client/singleplayerrules.h"
#include "sdlui.h"
#include "demo.h"
#include "window.h"

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	
	try
	{
		squarez::Window win;
		std::shared_ptr<squarez::SinglePlayerRules> rules(new squarez::SinglePlayerRules(8, 3, 10, 60, 180));
		squarez::SdlUI ui(win, rules);
		ui.run();
	}
	catch(std::exception& e)
	{
		std::cerr << "Caught exception: " << e.what() << std::endl;
	}
	
	
	SDL_Quit();
	
	return 0;
}
