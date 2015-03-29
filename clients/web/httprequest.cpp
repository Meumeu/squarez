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

typedef std::pair<std::function<void(std::string const&)>, std::function<void()>> callback_pair;

namespace {
void em_callback(callback_pair* callbacks, char* data, int size)
{
	std::unique_ptr<callback_pair> callbacks_ptr(callbacks);
	std::string res(data, size);
	callbacks_ptr->first(res);
}
void em_error(callback_pair* callbacks)
{
	std::unique_ptr<callback_pair> callbacks_ptr(callbacks);
	callbacks_ptr->second();
}
}

// FIXME: return std::unique_ptr<Handle>
void squarez::HttpRequest::request(const std::string& url, std::function<void(std::string const&)> onload, std::function<void()> onerror)
{
	callback_pair * callbacks = new callback_pair(onload, onerror);
	emscripten_async_wget_data(url.c_str(), callbacks, (void (*)(void*, void*, int))&em_callback, (void (*)(void*))&em_error);
}

std::string squarez::HttpRequest::request(const std::string& url)
{
	emscripten::val xhr = emscripten::val::global("XMLHttpRequest").new_();
	xhr.call<void>("open", std::string("get"), url, false);
	xhr.call<void>("send");
	return xhr["responseText"].as<std::string>();
}
