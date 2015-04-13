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

#ifndef SQUAREZ_QT_SETTINGS_H
#define SQUAREZ_QT_SETTINGS_H

#include <QObject>
#include <QSettings>

class QQmlEngine;
class QJSEngine;

namespace squarez {
namespace qt {

class Settings : public QObject
{
	Q_OBJECT
	QSettings _settings;

	explicit Settings();

public:
	~Settings();

	Q_INVOKABLE void setValue(const QString& key, const QVariant& value);
	Q_INVOKABLE QVariant value(const QString& key, const QVariant& defaultValue = QVariant());

	static QObject * provider(QQmlEngine*, QJSEngine*);
signals:

public slots:

};

}
}

#endif // SQUAREZ_QT_SETTINGS_H
