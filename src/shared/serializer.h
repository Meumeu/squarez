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

#ifndef SQUAREZ_SERIALIZER_H
#define SQUAREZ_SERIALIZER_H

#include <iostream>
#include <memory>
#include <set>
#include <vector>

namespace squarez {

class Serializer
{
public:
	std::unique_ptr<std::iostream> _stream;
	Serializer();
	Serializer(std::string const&);
	Serializer(std::unique_ptr<std::iostream> & stream): _stream(std::move(stream)) {}
	
	std::string get();
};

Serializer& operator<<(Serializer& ser, std::string const&);
Serializer& operator<<(Serializer& ser, char const *);
Serializer& operator>>(Serializer& ser, std::string&);

template<class T> Serializer& operator<<(Serializer& ser, T const&);
template<class T> Serializer& operator>>(Serializer& ser, T&);

template<class T1, class T2> Serializer& operator<<(Serializer& ser, std::pair<T1, T2> const&);
template<class T1, class T2> Serializer& operator>>(Serializer& ser, std::pair<T1, T2>&);

template<class T> Serializer& operator<<(Serializer& ser, std::vector<T> const&);
template<class T> Serializer& operator>>(Serializer& ser, std::vector<T>&);

template<class T> Serializer& operator<<(Serializer& ser, std::set<T> const&);
template<class T> Serializer& operator>>(Serializer& ser, std::set<T>&);


template<class T> Serializer& operator<<(Serializer& ser, T const& x)
{
	*ser._stream << x << " ";
	return ser;
}

template<class T> Serializer& operator>>(Serializer& ser, T& x)
{
	*ser._stream >> x;
	ser._stream->get();
	return ser;
}

template<typename T1, typename T2> Serializer& operator<<(Serializer& ser, std::pair<T1, T2> const& t)
{
	return ser << t.first << t.second;
}

template<typename T1, typename T2> Serializer& operator>>(Serializer& ser, std::pair<T1, T2>& t)
{
	return ser >> t.first >> t.second;
}

template<class T> Serializer& operator<<(Serializer& ser, std::vector<T> const& v)
{
	ser << v.size();
	for(T const& i: v)
	{
		ser << i;
	}
	
	return ser;
}

template<class T> Serializer& operator>>(Serializer& ser, std::vector<T>& v)
{
	std::size_t s;
	ser >> s;
	T x;
	
	while(s--)
	{
		ser >> x;
		v.push_back(std::move(x));
	}
	
	return ser;
}

template<class T> Serializer& operator<<(Serializer& ser, std::set<T> const& v)
{
	ser << v.size();
	for(T const& i: v)
	{
		ser << i;
	}

	return ser;
}

template<class T> Serializer& operator>>(Serializer& ser, std::set<T>& v)
{
	std::size_t s;
	ser >> s;
	T x;

	while(s--)
	{
		ser >> x;
		v.insert(std::move(x));
	}

	return ser;
}

}

#endif // SQUAREZ_SERIALIZER_H
