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

namespace squarez {
namespace qt {

class SettingValue : public QObject
{
	Q_OBJECT
	QSettings _settings;
	QString _key;
	QString _defaultValue;

public:
	explicit SettingValue();
	~SettingValue();

	Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged)
	Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)
	Q_PROPERTY(QString defaultValue READ defaultValue WRITE setDefaultValue NOTIFY defaultValueChanged)

	void setValue(const QString& value);
	QString value() const { return _settings.value(_key, _defaultValue).toString(); }

	void setDefaultValue(const QString& defaultValue);
	QString defaultValue() const { return _defaultValue; }

	void setKey(const QString& key);
	QString key() const { return _key; }

signals:
	void keyChanged(QString key);
	void valueChanged(QString value);
	void defaultValueChanged(QString defaultValue);
};

}
}

#endif // SQUAREZ_QT_SETTINGS_H
