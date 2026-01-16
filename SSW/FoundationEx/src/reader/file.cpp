/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "file.h"

#include <algorithm>
#include <fstream>
#include <ios>
#include <limits>
#include <utility>
#include <vector>

#include <Text.h>


namespace starshatter
{
namespace foundation
{


FileReader::FileReader(std::fstream src) :
	file {std::move(src)}
{
	file.seekg(0);
	file.ignore(std::numeric_limits<std::streamsize>::max());
	size = file.gcount();
	file.clear();
	file.seekg(0);
	position = 0;
}


bool
FileReader::valid() const
{
	return static_cast<bool>(file) && file.is_open();
}


Count
FileReader::available() const
{
	return size - position;
}


Count
FileReader::seek(Count pos)
{
	position = pos;
	if (position > size)
		position = size;
	file.seekg(position);
	return position;
}


Count
FileReader::seek(Offset offset, Direction dir)
{
	switch (dir) {
	case Direction::Start:
		break;  // no-op
	case Direction::End:
		offset = size + offset;
		break;
	case Direction::Current:
		offset = position + offset;
		break;
	}
	if (offset < 0)
		offset = 0;
	return seek(static_cast<Count>(offset));
}


Count
FileReader::read(char* dest)
{
	return read(dest, available());
}


Count
FileReader::read(char* dest, Count bytes, Count times)
{
	bytes = std::min(bytes * times, available());
	file.read(dest, bytes);
	position += bytes;
	return bytes;
}


Count
FileReader::peek(char* dest) const
{
	return peek(dest, available());
}


Count
FileReader::peek(char* dest, Count bytes, Count times) const
{
	bytes = std::min(bytes * times, available());
	const auto before = file.tellg();
	file.read(dest, bytes);
	file.seekg(before);
	return bytes;
}


Text
FileReader::more()
{
	const auto size = available();
	if (size < 1)
		return Text();
	std::vector<char> tmp(size);
	read(tmp.data(), size);
	return Text(tmp.data(), size);
}


}  // namespace foundation
}  // namespace starshatter
