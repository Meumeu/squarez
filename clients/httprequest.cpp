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

#ifdef EMSCRIPTEN
#error
#endif


#include "httprequest.h"
#include <sstream>
#include <iostream>
#include <curl/curl.h>
#include <QObject>
#include <QThread>
#include <mutex>


#define USERAGENT PACKAGE "/" PACKAGE_VERSION

squarez::http::Handle::~Handle()
{
}


namespace {
class HttpRequest: public QThread
{
	Q_OBJECT
	
	std::string _url;
	
protected:
	void run() override
	{
		try
		{
			emit onload(QString::fromStdString(squarez::http::request(_url)));
		}
		catch(std::exception& e)
		{
			emit onerror();
		}
	}
	
public:
	~HttpRequest() {}
	
	HttpRequest(const std::string& url) : _url(url)
	{
		connect(this, &QThread::finished, this, &QObject::deleteLater);
	}
	
signals:
	void onload(QString response);
	void onerror();
};

class HttpHandle : public QObject, public squarez::http::Handle
{
	Q_OBJECT
	std::function<void(std::string const&)> _onload;
	std::function<void()> _onerror;

public:
    ~HttpHandle() {}
	
	HttpHandle(std::string url, std::function<void(std::string const&)> onload, std::function<void()> onerror): _onload(onload), _onerror(onerror)
	{
		HttpRequest * request = new HttpRequest(url);
		connect(request, &HttpRequest::onload, this, &HttpHandle::onload);
		connect(request, &HttpRequest::onerror, this, &HttpHandle::onerror);
		request->start();
	}
	
private:
	void onload(QString response)
	{
		_onload(response.toStdString());
	}
	
	void onerror()
	{
		_onerror();
	}
};
}

namespace squarez {
namespace http {

std::unique_ptr<Handle> request(const std::string& url, std::function<void(std::string const&)> onload, std::function<void()> onerror)
{
	return std::unique_ptr<Handle>(new HttpHandle(url, onload, onerror));
}

static size_t write_in_memory(char * ptr, size_t size, size_t nmemb, void * userdata)
{
	std::stringstream& strstr = *reinterpret_cast<std::stringstream *>(userdata);

	strstr.write(ptr, size * nmemb);

	return size * nmemb;
}

std::string request(const std::string& url)
{
	auto del = std::function<void(CURL*)>(curl_easy_cleanup);
	std::unique_ptr<CURL, decltype(del)> curl(curl_easy_init(), del);

	if (!curl)
		throw std::runtime_error("Cannot initialize libcurl");

	std::stringstream strstr;

	curl_easy_setopt(curl.get(), CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, USERAGENT);
	curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, write_in_memory);
	curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, (void *)&strstr);
	curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());

	CURLcode rc = curl_easy_perform(curl.get());

	if (rc != CURLE_OK)
		throw std::runtime_error("CURL error " + std::to_string(rc));
	
	long response_code = 0;
	rc = curl_easy_getinfo(curl.get(), CURLINFO_RESPONSE_CODE, &response_code);
	if (rc != CURLE_OK)
		throw std::runtime_error("CURL error " + std::to_string(rc));
	
	if (response_code != 200)
		throw std::runtime_error("HTTP response code " + std::to_string(response_code));
	
	return strstr.str();
}

}
}

#include "httprequest.moc"
