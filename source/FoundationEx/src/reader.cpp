/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include <starshatter/foundation/reader.h>

#include <memory>
#include <utility>

#include <Text.h>

#include "reader/buffer.h"


namespace starshatter
{
namespace foundation
{


Reader::Reader()
{
}


Reader::Reader(Reader&& other) :
	actual {std::move(other.actual)}
{
}


Reader&
Reader::operator=(Reader&& other)
{
	actual = std::move(other.actual);
	return *this;
}


Reader::Reader(Base src) :
	actual {std::move(src)}
{
}


Reader::Reader(Chars src) :
	actual {std::make_unique<BufferReader<Chars>>(std::move(src))}
{
}


Reader::Reader(Chars src, Count bytes) :
	actual {std::make_unique<BufferReader<Chars>>(std::move(src), bytes)}
{
}


Reader::Reader(const char* src) :
	actual {std::make_unique<BufferReader<const char*>>(src)}
{
}


Reader::Reader(const char* src, Count bytes) :
	actual {std::make_unique<BufferReader<const char*>>(src, bytes)}
{
}


bool Reader::valid() const { return static_cast<bool>(actual); }
Count Reader::available() const { return actual->available(); }
Count Reader::seek(Count pos) { return actual->seek(pos); }
Count Reader::seek(Offset offset, Direction dir) { return actual->seek(offset, dir); }
Count Reader::read(char* dest) { return actual->read(dest); }
Count Reader::read(char* dest, Count bytes, Count times) { return actual->read(dest, bytes, times); }
Count Reader::peek(char* dest) const { return actual->peek(dest); }
Count Reader::peek(char* dest, Count bytes, Count times) const { return actual->peek(dest, bytes, times); }
Text Reader::more() { return actual->more(); }


}  // namespace foundation
}  // namespace starshatter
