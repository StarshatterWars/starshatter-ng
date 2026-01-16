/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#pragma once

#include <fstream>

#include <starshatter/foundation/reader.h>
#include <Text.h>


namespace starshatter
{
namespace foundation
{


struct FileReader : public BaseReader<char>
{
	FileReader() = default;
	FileReader(std::fstream src);
	FileReader(const FileReader& other) = delete;
	FileReader& operator=(const FileReader& other) = delete;
	FileReader(FileReader&& other) = default;
	FileReader& operator=(FileReader&& other) = default;
	~FileReader() override = default;

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
	mutable std::fstream file = {};
	Count size = {};
	Count position = {};
};


}  // namespace foundation
}  // namespace starshatter
