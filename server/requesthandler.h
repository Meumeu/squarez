/*
 * Squarez puzzle game server binary
 * Copyright (C) 2013-2015  Patrick Nicolas <patricknicolas@laposte.net>
 * Copyright (C) 2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#include "highscores.h"
#include <Simple-Web-Server/server_http.hpp>

namespace squarez
{

class RequestHandler
{
	std::unique_ptr<squarez::HighScores> _highScores;
	std::unordered_map<std::string, std::string> parseGet(const std::string& uri);

public:
	typedef SimpleWeb::Server<SimpleWeb::HTTP>::Response Response;
	typedef SimpleWeb::Server<SimpleWeb::HTTP>::Request Request;

	RequestHandler(std::unique_ptr<squarez::HighScores>&& highScores);

	void gameInit(Response& response, std::shared_ptr<Request> request);
	void pushSelection(Response& response, std::shared_ptr<Request> request);
	void pause(Response& response, std::shared_ptr<Request> request);
	void getScores(Response& response, std::shared_ptr<Request> request);
};

}

#endif // SQUAREZ_REQUESTHANDLER_H
