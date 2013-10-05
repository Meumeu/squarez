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

#include "httprequest.h"
#include <string>
#include <stdexcept>
#include "config.h"

#ifndef EMSCRIPTEN

#include <curl/curl.h>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <thread>
#include <mutex>

namespace
{
static size_t http_write_in_memory(char * ptr, size_t size, size_t nmemb, void * userdata)
{
	std::stringstream& strstr = *reinterpret_cast<std::stringstream *>(userdata);

	strstr.write(ptr, size * nmemb);

	return size * nmemb;
}
}

void squarez::HttpRequest::request(const std::string& url, std::function<void(std::string const&)> onload, std::function<void()> onerror)
{
	std::thread worker([this, url, onload, onerror]() {
		std::unique_lock<std::mutex> lock;
		std::string response;
		
		try
		{
			response = std::move(request(url));
		}
		catch(...)
		{
			if (_mutex)
				lock = std::unique_lock<std::mutex>(*_mutex);
			
			onerror();
			return;
		}
		
		try
		{
			if (_mutex)
				lock = std::unique_lock<std::mutex>(*_mutex);
			
			onload(response);
		}
		catch(...)
		{
		}
	});
	worker.detach();
}

std::string squarez::HttpRequest::request(const std::string& url)
{
	auto del = std::function<void(CURL*)>(curl_easy_cleanup);
	std::unique_ptr<CURL, decltype(del)> curl(curl_easy_init(), del);
	
	if (!curl)
		throw std::runtime_error("Cannot initialize libcurl");
	
	std::stringstream strstr;
	
	curl_easy_setopt(curl.get(), CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, "squarez/" PACKAGE_VERSION);
	curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, http_write_in_memory);
	curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, (void *)&strstr);
	curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
	
	CURLcode rc = curl_easy_perform(curl.get());
	
	if (rc != CURLE_OK)
		throw std::runtime_error("CURL error " + boost::lexical_cast<std::string>(rc));
	else
		return strstr.str();
}

#else //EMSCRIPTEN
typedef std::pair<std::function<void(std::string const&)>, std::function<void()>> callback_pair;

namespace {
void em_callback(callback_pair* callbacks, char* data, int size)
{
	std::string res(data, size);
	callbacks->first(res);
	delete callbacks;
}
void em_error(callback_pair* callbacks)
{
	callbacks->second();
	delete callbacks;
}
}
void squarez::HttpRequest::request(const std::string& url, std::function<void(std::string const&)> onload, std::function<void()> onerror)
{
	callback_pair * callbacks = new callback_pair(onload, onerror);
	emscripten_async_wget_data(url.c_str(), callbacks, (void (*)(void*, void*, int))&em_callback, (void (*)(void*))&em_error);
}

std::string squarez::HttpRequest::request(const std::string& url)
{
	std::string request("var xhr=new XMLHttpRequest();"
	"xhr.open('get','" + url + "',false);"
	"xhr.send();"
	"xhr.responseText;");

	const char* data = emscripten_run_script_string(request.data());
	return std::string(data);
}

#endif