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
#ifdef EMSCRIPTEN
	#include <emscripten/emscripten.h>
	#include <emscripten/val.h>
#else
	#include <curl/curl.h>
	#include <memory>
	#include <sstream>
	#include <stdexcept>

	#ifndef SQUAREZ_QT
		#include <thread>
		#include <mutex>
	#endif

	#ifndef USERAGENT
		#include "config.h"
		#define USERAGENT "squarez/" PACKAGE_VERSION
	#endif
#endif

#ifndef EMSCRIPTEN
namespace
{
size_t http_write_in_memory(char * ptr, size_t size, size_t nmemb, void * userdata)
{
	std::stringstream& strstr = *reinterpret_cast<std::stringstream *>(userdata);

	strstr.write(ptr, size * nmemb);

	return size * nmemb;
}

std::string synchronous_request(const std::string & url)
{
	auto del = std::function<void(CURL*)>(curl_easy_cleanup);
	std::unique_ptr<CURL, decltype(del)> curl(curl_easy_init(), del);

	if (!curl)
		throw std::runtime_error("Cannot initialize libcurl");

	std::stringstream strstr;

	curl_easy_setopt(curl.get(), CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, USERAGENT);
	curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, http_write_in_memory);
	curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, (void *)&strstr);
	curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());

	CURLcode rc = curl_easy_perform(curl.get());

	if (rc != CURLE_OK)
		throw std::runtime_error("CURL error " + std::to_string(rc));
	else
		return strstr.str();
}

#ifndef SQUAREZ_QT
void async_request(const std::string url, std::function<void(std::string const&)> onload, std::function<void()> onerror, std::mutex * mutex)
{
	std::unique_lock<std::mutex> lock;
	std::string response;

	try
	{
		response = std::move(synchronous_request(url));
	}
	catch(...)
	{
		if (mutex)
			lock = std::unique_lock<std::mutex>(*mutex);

		onerror();
		return;
	}

	try
	{
		if (mutex)
			lock = std::unique_lock<std::mutex>(*mutex);

		onload(response);
	}
	catch(...)
	{
	}
}
#endif

}

#ifdef SQUAREZ_QT
void squarez::AsyncRequest::run()
{
	try
	{
		QString response = QString::fromStdString(synchronous_request(_url));
		emit load(response);
	}
	catch (...)
	{
		emit error();
	}
}

void squarez::HttpRequest::request(const std::string& url, std::function<void(std::string const&)> onload, std::function<void()> onerror)
{
	squarez::AsyncRequest * worker = new squarez::AsyncRequest(url);
	squarez::Callback * callback = new squarez::Callback(onload, onerror);
	callback->connect(worker, &squarez::AsyncRequest::load, callback, &squarez::Callback::onLoad);
	callback->connect(worker, &squarez::AsyncRequest::error, callback, &squarez::Callback::onErorr);
	worker->connect(worker, &squarez::AsyncRequest::finished, &QObject::deleteLater);
	worker->start();
}

#else

void squarez::HttpRequest::request(const std::string& url, std::function<void(std::string const&)> onload, std::function<void()> onerror)
{
	std::thread worker(async_request, url, onload, onerror, _mutex);
	worker.detach();
}
#endif

std::string squarez::HttpRequest::request(const std::string& url)
{
	return synchronous_request(url);
}

#else //EMSCRIPTEN

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

#endif
