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

#include "serializer.h"
#include <vector>
#include <sstream>
#include <iterator>

namespace squarez {

Serializer::Serializer(): _stream(new std::stringstream()){}

Serializer::Serializer(const std::string& str): _stream(new std::stringstream(str)) {}
	
Serializer& operator<<(Serializer& ser, std::string const& str)
{
	ser << str.size();
	ser._stream->write(str.data(), str.size());
	return ser;
}

Serializer& operator<<(Serializer& ser, char const * str)
{
	return ser << std::string(str);
}

Serializer& operator>>(Serializer& ser, std::string& str)
{
	std::size_t s;
	ser >> s;

	std::vector<char> buffer(s);

	ser._stream->read(buffer.data(), s);
	str.assign(buffer.data(), s);

	ser._stream->seekg(s, std::ios_base::cur);
	
	return ser;
}

std::string Serializer::get()
{
	auto strstream = dynamic_cast<std::stringstream *>(_stream.get());
	if (strstream)
		return strstream->str();

	_stream->seekg(0);
	std::istream_iterator<char> begin(*_stream), eos;
	std::string res(begin, eos);
	return res;
}

}
