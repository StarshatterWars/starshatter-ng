/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#pragma once

#include <cstddef>
#include <memory>

#include <Text.h>


namespace starshatter
{
namespace foundation
{


using Count = std::size_t;
using Offset = std::ptrdiff_t;


enum class Direction
{
	Start,
	End,
	Current,
};


template <typename Char>
struct BaseReader
{
	virtual ~BaseReader() = default;
	virtual bool valid() const = 0;
	virtual Count available() const = 0;
	virtual Count seek(Count pos) = 0;
	virtual Count seek(Offset offset, Direction dir) = 0;
	virtual Count read(Char* dest) = 0;
	virtual Count read(Char* dest, Count bytes, Count times=1) = 0;
	virtual Count peek(Char* dest) const = 0;
	virtual Count peek(Char* dest, Count bytes, Count times=1) const = 0;
	virtual Text more() = 0;
};


struct Reader : public BaseReader<char>
{
	using Base = std::unique_ptr<BaseReader>;
	using Chars = std::unique_ptr<const char[]>;

	Reader();

	Reader(const Reader&) = delete;
	Reader& operator=(const Reader&) = delete;

	Reader(Reader&& other);
	Reader& operator=(Reader&& other);

	Reader(Base src);
	Reader(Chars src);
	Reader(Chars src, Count bytes);
	Reader(const char* src);
	Reader(const char* src, Count bytes);

	~Reader() override = default;

	bool valid() const override;
	Count available() const override;
	Count seek(Count pos) override;
	Count seek(Offset offset, Direction dir) override;
	Count read(char* dest) override;
	Count read(char* dest, Count bytes, Count times=1) override;
	Count peek(char* dest) const override;
	Count peek(char* dest, Count bytes, Count times=1) const override;
	Text more() override;
private:
	Base actual;
};


}  // namespace foundation
}  // namespace starshatter
