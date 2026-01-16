/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "OverlayDataSource.h"

#include <cstdint>

#include <starshatter/foundation/reader.h>
#include <List.h>
#include <Text.h>


using starshatter::foundation::Reader;


OverlayDataSource::OverlayDataSource()
{
}


OverlayDataSource::~OverlayDataSource()
{
	m_sources.destroy();
}


bool
OverlayDataSource::Find(const Text& prefix, const char* name) const
{
	for (int i = m_sources.size() - 1; i >= 0; --i) {
		DataSource* src = m_sources[i];
		if (src->Find(prefix, name))
			return true;
	}
	return false;
}


int
OverlayDataSource::ListFiles(const Text& prefix, Text filter, List<Text>& items, bool recurse) const
{
	for (int i = m_sources.size() - 1; i >= 0; --i) {
		DataSource* src = m_sources[i];
		src->ListFiles(prefix, filter, items, recurse);
	}
	return items.size();
}


int
OverlayDataSource::Load(const Text& prefix, const char* name, std::uint8_t*& buf, bool null_terminate) const
{
	for (int i = m_sources.size() - 1; i >= 0; --i) {
		DataSource* src = m_sources[i];
		const int len = src->Load(prefix, name, buf, null_terminate);
		if (len > 0)
			return len;
	}
	return 0;
}


DataSource*
OverlayDataSource::Mount(DataSource* source, int pos)
{
	if (pos < 0)
		pos = m_sources.size() + pos + 1;
	m_sources.insert(source, pos);
	return source;
}


DataSource*
OverlayDataSource::Unmount(DataSource* source)
{
	return m_sources.remove(source);
}


DataSource*
OverlayDataSource::Unmount(int index)
{
	return m_sources.removeIndex(index);
}


DataSource*
OverlayDataSource::UnmountId(int ref)
{
	ListIter<DataSource> iter = m_sources;
	while (++iter) {
		if (iter.value()->Id() == ref)
			return iter.removeItem();
	}
	return nullptr;
}


DataSource*
OverlayDataSource::ById(int ref)
{
	ListIter<DataSource> iter = m_sources;
	while (++iter) {
		if (iter.value()->Id() == ref)
			return iter.value();
	}
	return nullptr;
}


Reader
OverlayDataSource::Open(const Text& prefix, const char* name) const
{
    for (int i = m_sources.size() - 1; i >= 0; --i) {
        DataSource* src = m_sources[i];
        if (src->Find(prefix, name))
            return src->Open(prefix, name);
    }
    return {};
}
