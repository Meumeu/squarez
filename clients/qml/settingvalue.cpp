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

#include "settingvalue.h"

namespace squarez {
namespace qt {

SettingValue::SettingValue()
{
}

void SettingValue::setKey(const QString& key)
{
	if (key != _key)
	{
		_key = key;
		emit keyChanged(_key);
		emit valueChanged(value());
	}
}

void SettingValue::setValue(const QString& value)
{
	_settings.setValue(_key, value);
	emit valueChanged(value);
}

void SettingValue::setDefaultValue(const QString& defaultValue)
{
	_defaultValue = defaultValue;
	emit defaultValueChanged(_defaultValue);
}

SettingValue::~SettingValue()
{
}


}
}
