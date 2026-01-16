/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include <algorithm>
#include <cstring>
#include <memory>
#include <utility>
#include <vector>

#include <Text.h>


namespace starshatter
{
namespace foundation
{


// If extension is needed, use type_traits or update to C++20 and use concepts
inline bool ok(const char* data) { return data != nullptr; }
inline bool ok(const std::unique_ptr<const char[]>& data) { return static_cast<bool>(data); }
inline const char* at(const char* data) { return data; }
inline const char* at(const std::unique_ptr<const char[]>& data) { return data.get(); }


template <typename Source>
BufferReader<Source>::BufferReader()
{
}


template <typename Source>
BufferReader<Source>::BufferReader(Source src) :
	buffer {std::move(src)},
	size {std::strlen(at(buffer)) + 1}
{
}


template <typename Source>
BufferReader<Source>::BufferReader(Source src, Count bytes) :
	buffer {std::move(src)},
	size {bytes}
{
}


template <typename Source>
bool
BufferReader<Source>::valid() const
{
	return ok(buffer);
}


template <typename Source>
Count
BufferReader<Source>::available() const
{
	return size - position;
}


template <typename Source>
Count
BufferReader<Source>::seek(Count pos)
{
	position = pos;
	if (position > size)
		position = size;
	return position;
}


template <typename Source>
Count
BufferReader<Source>::seek(Offset offset, Direction dir)
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
	position = static_cast<Count>(std::clamp<Offset>(offset, 0, size));
	return position;
}


template <typename Source>
Count
BufferReader<Source>::read(char* dest)
{
	return read(dest, available());
}


template <typename Source>
Count
BufferReader<Source>::read(char* dest, Count bytes, Count times)
{
	bytes = peek(dest, bytes, times);
	position += bytes;
	return bytes;
}


template <typename Source>
Count
BufferReader<Source>::peek(char* dest) const
{
	return peek(dest, available());
}


template <typename Source>
Count
BufferReader<Source>::peek(char* dest, Count bytes, Count times) const
{
	bytes = std::min(bytes * times, available());
	std::copy(at(buffer) + position, at(buffer) + position + bytes, dest);
	return bytes;
}


template <typename Source>
Text
BufferReader<Source>::more()
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
