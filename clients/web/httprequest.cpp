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

#include "httprequest.h"

#include <emscripten/emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>
namespace {

struct Callback
{
	std::function<void(std::string const&)> onload;
	std::function<void()> onerror;
	Callback *& _backReference;
	~Callback() { _backReference = nullptr;}
};

void em_callback(Callback* callback, char* data, int size)
{
	std::unique_ptr<Callback> callbacks_ptr(callback);
	std::string res(data, size);
	callbacks_ptr->onload(res);
}
void em_error(Callback* callback)
{
	std::unique_ptr<Callback> callbacks_ptr(callback);
	callbacks_ptr->onerror();
}

class JSHandle : public squarez::http::Handle
{
private:
	Callback * callback;
public:
	JSHandle(const std::string& url, std::function<void(std::string const&)> onload, std::function<void()> onerror)
	{
		callback = new Callback{onload, onerror, callback};
		emscripten_async_wget_data(url.c_str(), callback, (void (*)(void*, void*, int))&em_callback, (void (*)(void*))&em_error);
	}
	virtual ~JSHandle()
	{
		if (callback)
		{
			callback->onload = [](std::string const&){};
			callback->onerror = [](){};
		}
	}
};
}

std::unique_ptr<squarez::http::Handle> squarez::http::request(const std::string& url, std::function<void(std::string const&)> onload, std::function<void()> onerror)
{
	return std::unique_ptr<squarez::http::Handle>(new JSHandle(url, onload, onerror));
}