/*
 * Squarez puzzle game server binary
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

#ifndef SQUAREZ_REQUESTHANDLER_H
#define SQUAREZ_REQUESTHANDLER_H

// FastCGI
#include <fastcgi++/request.hpp>
#include "highscores.h"

namespace squarez
{

class RequestHandler: public Fastcgipp::Request<char>
{
public:
	static std::shared_ptr<HighScores> highScores;
	// Process the request
	bool response();
};

}

#endif // SQUAREZ_REQUESTHANDLER_H
