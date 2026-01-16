/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "starshatter/Resource.h"

#include <atomic>
#include <cstdint>


namespace starshatter
{


static std::atomic<std::uint64_t> g_counter {};


Resource::Resource() :
	m_handle {g_counter.fetch_add(1)}
{
}


std::uint64_t
Resource::Handle() const
{
	return m_handle;
}


bool
Resource::operator==(const Resource& other) const
{
	return Handle() == other.Handle();
}


}  // namespace starshatter
