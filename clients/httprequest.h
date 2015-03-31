/*
 * Squarez puzzle game
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

#ifndef SQUAREZ_HTTPREQUEST_H
#define SQUAREZ_HTTPREQUEST_H

#include <functional>
#include <memory>
#include <string>

namespace squarez {
namespace http {

class Handle
{
public:
	virtual ~Handle();
};

std::unique_ptr<Handle> request(const std::string& url, std::function<void(std::string const&)> onload, std::function<void()> onerror);
std::string request(const std::string& url);

}
}

#endif // SQUAREZ_HTTPREQUEST_H