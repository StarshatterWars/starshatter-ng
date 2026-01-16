/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#pragma once

#include <starshatter/foundation/reader.h>
#include <Text.h>


namespace starshatter
{
namespace foundation
{


template <typename Source>
struct BufferReader : public BaseReader<char>
{
	BufferReader();
	BufferReader(Source src);
	BufferReader(Source src, Count bytes);
	BufferReader(const BufferReader& other) = default;
	BufferReader& operator=(const BufferReader& other) = default;
	BufferReader(BufferReader&& other) = default;
	BufferReader& operator=(BufferReader&& other) = default;
	~BufferReader() override = default;

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
	Source buffer = {};
	Count size = {};
	Count position = {};
};


}  // namespace foundation
}  // namespace starshatter


#include "buffer.inl.h"
