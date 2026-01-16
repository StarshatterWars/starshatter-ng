#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>


namespace ArchiveEx
{


static constexpr std::size_t NAMELEN {64};


struct Entry
{
	char name[NAMELEN];
	std::uint32_t original_size;
	std::uint32_t compressed_size;
	std::uint32_t offset;
};


class Archive
{
public:
	explicit Archive(const char* path, bool create=false);
	int Extract(int index, std::uint8_t*& buffer, bool null_terminated=false) const;
	int Extract(const char* filepath, std::uint8_t*& buffer, bool null_terminated=false) const;
	int Find(const char* filepath) const;
	int Insert(const char* filepath);
	std::size_t NumEntries() const;
	void WriteMeta(bool create=false);
	void GenerateBlockMap();
	std::size_t FindFreeSpot(std::size_t bytes) const;
	void ForEachEntry(std::function<void(const char*)> func) const;
	void ForEachBlock(std::function<void(const char*)> func) const;
	std::size_t DirectoryBlocks() const;
private:
	const char* path;
	std::size_t directory_offset;
	std::vector<Entry> entries;
	std::vector<int> blocks;
};


}  // namespace ArchiveEx
