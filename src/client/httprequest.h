/*
 * Squarez puzzle game
 * Copyright (C) 2013  Guillaume Meunier <guillaume.meunier@centraliens.net>
 * Copyright (C) 2013  Patrick Nicolas <patricknicolas@laposte.net>
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

#include <string>
#include <functional>

#ifndef EMSCRIPTEN
	namespace std { class mutex; }
#endif

#ifdef SQUAREZ_QT
#include <QThread>
#endif

namespace squarez {

class HttpRequest
{
#ifndef EMSCRIPTEN
private:
	std::mutex * const _mutex;

public:
	HttpRequest(std::mutex & mutex): _mutex(&mutex) {}
	HttpRequest(): _mutex(nullptr) {}
#endif
public:

	void request(const std::string& url, std::function<void(std::string const&)> onload, std::function<void()> onerror);
	std::string request(const std::string& url);

};

#ifdef SQUAREZ_QT
class AsyncRequest: public QThread
{
	Q_OBJECT
	void run();
signals:
	void load(QString);
	void error();
public:
	AsyncRequest(const std::string url): _url(url) {}
private:
	std::string _url;
};

class Callback: public QObject
{
	Q_OBJECT
public:
	Callback(std::function<void(std::string const&)> onload, std::function<void()> onerror):
	_onload(onload), _onerror(onerror) {}
private:
	std::function<void(std::string const&)> _onload;
	std::function<void()> _onerror;

public slots:
	void onLoad(QString result) const {_onload(result.toStdString()); delete this;}
	void onErorr() const {_onerror(); delete this;}
};

#endif

}

#endif // SQUAREZ_XMLHTTPREQUEST_H
