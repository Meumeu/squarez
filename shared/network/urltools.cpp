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

#include "urltools.h"

#include <sstream>
#include <iomanip>
#include <cstdlib>

std::string squarez::urlTools::urlencode(const std::string& in)
{
	std::stringstream out;

	for(unsigned char c: in)
	{
		if ((c >= 'a' and c <= 'z') or
			(c >= 'A' and c <= 'Z') or
			(c >= '0' and c <= '9') or
			c == '-' or c == '_' or c == '.' or c == '~' or c == '(' or c == ')' or c == '\'' or c == '*' or c == '!')
			out << c;
		else
		{
			out << '%' << std::hex << std::setfill('0') << std::setw(2) << (int)c;

		}
	}

	return out.str();
}

#ifndef ANDROID
std::string squarez::urlTools::urldecode(const std::string& in)
{
	std::stringstream out;

	for(size_t i = 0, sz = in.size(); i < sz; )
	{
		if (in[i] == '%' && i + 2 < sz)
		{
			out << (unsigned char)(std::stoi(in.substr(i+1,2), nullptr, 16));
			i += 3;
		}
		else if (in[i] == '+')
		{
			out << ' ';
			++i;
		}
		else
		{
			out << in[i];
			++i;
		}
	}

	return out.str();
}
#endif
