/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#pragma once

#include <cstdint>


namespace starshatter
{


class Resource
{
public:
	Resource();
	virtual ~Resource() = default;
	std::uint64_t Handle() const;
	bool operator==(const Resource& other) const;

private:
	std::uint64_t m_handle;
};


}  // namespace starshatter
