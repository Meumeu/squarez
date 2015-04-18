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


#include "../httprequest.h"
#include <sstream>
#include <iostream>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#define USERAGENT PACKAGE "/" PACKAGE_VERSION

namespace {
	QNetworkAccessManager nam;
	
	class HttpHandle : public QObject, public squarez::http::Handle
	{
		Q_OBJECT

		QNetworkReply * _reply;
		std::function<void(std::string const&)> _onload;
		std::function<void()> _onerror;

		void onload()
		{
			_onload(_reply->readAll().constData());
		}

		void onerror(QNetworkReply::NetworkError)
		{
			_onerror();
		}

	public:
		HttpHandle(QUrl url, std::function<void(std::string const&)> onload, std::function<void()> onerror) :
		_reply(), _onload(onload), _onerror(onerror)
		{
			QNetworkRequest req(url);
			req.setHeader(QNetworkRequest::UserAgentHeader, USERAGENT);
			_reply = nam.get(req);

			connect(_reply, &QNetworkReply::finished, this, &HttpHandle::onload);
			connect(_reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &HttpHandle::onerror);
		}

		~HttpHandle()
		{
			if (_reply)
				_reply->deleteLater();
			_reply = nullptr;
		}
	};
}

namespace squarez {
namespace http {

std::unique_ptr<Handle> request(const std::string& url, std::function<void(std::string const&)> onload, std::function<void()> onerror)
{
	return std::unique_ptr<Handle>(new HttpHandle(QUrl(QString::fromStdString(url)), onload, onerror));
}

}
}

#include "httprequest.moc"