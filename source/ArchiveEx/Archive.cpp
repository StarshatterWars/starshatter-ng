#include "Archive.h"

#ifdef _WIN32
#include <string.h>
#else
#include <strings.h>
#endif

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <memory>

#include <zlib.h>


namespace ArchiveEx
{


struct Header
{
	std::uint32_t version;
	std::uint32_t total_entries;
	struct {
		std::uint32_t total_blocks;
		std::uint32_t compressed_size;
		std::uint32_t offset;
	} directory;
};


using UniqueFileHandle = std::unique_ptr<std::FILE, decltype(&std::fclose)>;


static inline std::size_t BytesToBlocks(std::size_t bytes);
static inline std::size_t NormalizePath(const char* src, char* dest);


static constexpr std::uint32_t VERSION {0x0010};
static constexpr std::size_t BLOCK_SIZE {1024};
static constexpr std::size_t DIRECTORY_MARGIN {64};
static constexpr int UNUSED_BLOCK {-1};
static constexpr int DIRECTORY_BLOCK {-2};


Archive::Archive(const char* p, bool create) :
	path {p},
	directory_offset {0},
	entries {},
	blocks {DIRECTORY_BLOCK}
{
	UniqueFileHandle file {std::fopen(path, "rb"), &std::fclose};
	if (!file) {
		if (!create)
			throw "could not open archive";
		WriteMeta(true);
		return;
	}
	Header header {};
	std::size_t length = std::fread(&header, sizeof(Header), 1, file.get());
	if (1 != length)
		throw "could not read";
	if (VERSION != header.version)
		throw "bad version";
	int err = std::fseek(file.get(), sizeof(Header) + header.directory.offset, SEEK_SET);
	if (-1 == err)
		throw "could not find directory in archive";
	auto compressed = std::make_unique<Bytef[]>(header.directory.compressed_size);
	length = std::fread(compressed.get(), 1, header.directory.compressed_size, file.get());
	if (header.directory.compressed_size != length)
		throw "could not read compressed directory";
	const std::size_t total_entries = header.total_entries + DIRECTORY_MARGIN;
	entries.resize(header.total_entries);
	entries.reserve(total_entries);  // In original impl entries were switched on/off with their uncompressed size value.
	uLongf uncompressed_size = sizeof(Entry) * total_entries;
	err = uncompress(
		reinterpret_cast<Bytef*>(entries.data()),
		&uncompressed_size,
		compressed.get(),
		header.directory.compressed_size);
	if (Z_OK != err)
		throw "could not uncompress directory";
	directory_offset = header.directory.offset;
	GenerateBlockMap();
}


int
Archive::Extract(const int index, std::uint8_t*& buffer, const bool null_terminated) const
{
	if (0 > index && entries.size() <= static_cast<decltype(entries)::size_type>(index))
		return -1;  // Can be replaced by error-checked access to std::vector holding entries.
	UniqueFileHandle file {std::fopen(path, "rb"), &std::fclose};
	if (!file)
		return -1;
	const auto& entry = entries[index];
	auto compressed = std::make_unique<Bytef[]>(entry.compressed_size);
	int err = std::fseek(file.get(), sizeof(Header) + entry.offset, SEEK_SET);
	if (-1 == err)
		return -1;
	const std::size_t length = std::fread(compressed.get(), 1, entry.compressed_size, file.get());
	if (entry.compressed_size != length)
		return -1;
	uLongf output_length = entry.original_size;
	auto uncompressed = std::make_unique<std::uint8_t[]>(output_length + null_terminated);
	err = uncompress(uncompressed.get(), &output_length, compressed.get(), entry.compressed_size);
	if (Z_OK != err)
		return -1;
	buffer = uncompressed.release();
	if (null_terminated)
		buffer[output_length] = 0;
	return output_length;
}


int
Archive::Extract(const char* filepath, std::uint8_t*& buffer, const bool null_terminated) const
{
	const int index = Find(filepath);
	if (-1 == index)
		return -1;
	return Extract(index, buffer, null_terminated);
}


int
Archive::Find(const char* filepath) const
{
	auto path = std::make_unique<char[]>(std::strlen(filepath) + 1);
	NormalizePath(filepath, path.get());
	for (std::size_t i = 0; i < entries.size(); ++i) {
#ifdef _WIN32
		if (0 == _stricmp(entries[i].name, path.get()))
#else
		if (0 == strcasecmp(entries[i].name, path.get()))
#endif
			return i;
	}
	return -1;
}


int
Archive::Insert(const char* filepath)
{
	const std::size_t namelen = std::strlen(filepath);
	if (NAMELEN <= namelen)
		return -1;
	UniqueFileHandle source {std::fopen(filepath, "rb"), &std::fclose};
	int err = std::fseek(source.get(), 0, SEEK_END);
	if (-1 == err)
		return -1;
	const std::size_t uncompressed_size = std::ftell(source.get());
	err = std::fseek(source.get(), 0, SEEK_SET);
	if (-1 == err || 0 == uncompressed_size)
		return -1;
	auto uncompressed = std::make_unique<Bytef[]>(uncompressed_size);
	const auto read_result = std::fread(uncompressed.get(), 1, uncompressed_size, source.get());
	if (read_result != uncompressed_size)
		return -1;
	source.reset(nullptr);
	uLongf compressed_size = uncompressed_size * 1.1 + 12;
	auto compressed = std::make_unique<Bytef[]>(compressed_size);
	err = compress(compressed.get(), &compressed_size, uncompressed.get(), uncompressed_size);
	if (Z_OK != err)
		return -1;
	uncompressed.reset();
	int index;
	const int previous_index = Find(filepath);
	if (-1 != previous_index) {
		for (auto& block : blocks) {
			if (previous_index == block)
				block = UNUSED_BLOCK;
		}
		index = previous_index;
	}
	else {
		index = entries.size();
		entries.emplace_back();
	}
	const auto destination_block = FindFreeSpot(compressed_size);
	const auto total_blocks = BytesToBlocks(compressed_size);
	if (blocks.size() < destination_block + total_blocks)
		blocks.resize(destination_block + total_blocks, UNUSED_BLOCK);
	for (std::size_t i = destination_block; i < destination_block + total_blocks; ++i)
		blocks[i] = index;
	auto normalized_path = std::make_unique<char[]>(std::strlen(filepath) + 1);
	NormalizePath(filepath, normalized_path.get());
	std::strcpy(entries[index].name, normalized_path.get());
	entries[index].original_size = uncompressed_size;
	entries[index].compressed_size = compressed_size;
	entries[index].offset = destination_block * BLOCK_SIZE;
	UniqueFileHandle archive {std::fopen(path, "rb+"), &std::fclose};
	err = std::fseek(archive.get(), sizeof(Header) + entries[index].offset, SEEK_SET);
	if (-1 == err)
		return -1;
	const auto write_result = std::fwrite(compressed.get(), 1, compressed_size, archive.get());
	if (write_result != compressed_size)
		return -1;
	archive.reset(nullptr);
	WriteMeta();
	return index;
}


std::size_t
Archive::NumEntries() const
{
	return entries.size();
}


void
Archive::WriteMeta(bool create)
{
	const std::size_t total_entries = entries.size();
	const std::size_t dirsize = total_entries + DIRECTORY_MARGIN;
	const std::size_t uncompressed_size = dirsize * sizeof(Entry);
	entries.reserve(dirsize);
	uLongf compressed_size = uncompressed_size * 1.1 + 12;
	auto compressed = std::make_unique<Bytef[]>(compressed_size);
	int err = compress(
		compressed.get(),
		&compressed_size,
		reinterpret_cast<Bytef*>(entries.data()),
		uncompressed_size);
	if (Z_OK != err)
		throw "could not compress directory metadata";
	for (auto& block : blocks) {
		if (DIRECTORY_BLOCK == block)
			block = UNUSED_BLOCK;
	}
	const auto destination_block = FindFreeSpot(compressed_size);
	const auto total_blocks = BytesToBlocks(compressed_size);
	if (blocks.size() < destination_block + total_blocks)
		blocks.resize(destination_block + total_blocks, UNUSED_BLOCK);
	for (std::size_t i = destination_block; i < destination_block + total_blocks; ++i)
		blocks[i] = DIRECTORY_BLOCK;
	directory_offset = destination_block * BLOCK_SIZE;
	Header header {};
	header.version = VERSION;
	header.total_entries = total_entries;
	header.directory.total_blocks = total_blocks;  // Unused
	header.directory.compressed_size = compressed_size;
	header.directory.offset = directory_offset;
	UniqueFileHandle archive {std::fopen(path, "rb+"), &std::fclose};
	if (!archive) {
		if (!create)
			throw "could not open archive file";
		archive.reset(std::fopen(path, "wb"));
		if (!archive)
			throw "could not create archive file";
	}
	err = std::fseek(archive.get(), 0, SEEK_SET);
	if (-1 == err)
		throw "could not write header (seek)";
	err = std::fwrite(&header, sizeof(Header), 1, archive.get());
	if (1 != err)
		throw "could not write header (write)";
	err = std::fseek(archive.get(), sizeof(Header) + directory_offset, SEEK_SET);
	if (-1 == err)
		throw "could not write directory (seek)";
	err = std::fwrite(compressed.get(), 1, compressed_size, archive.get());
	if (static_cast<int>(compressed_size) != err)
		throw "could not write directory (write)";
}


void
Archive::GenerateBlockMap()
{
	const std::size_t last_directory_block = directory_offset / BLOCK_SIZE + DirectoryBlocks() - 1;
	std::size_t last_block = last_directory_block;
	for (const auto& entry : entries) {
		const std::size_t block = entry.offset / BLOCK_SIZE + BytesToBlocks(entry.compressed_size) - 1;
		if (last_block < block)
			last_block = block;
	}
	blocks.clear();
	blocks.resize(last_block + 1, UNUSED_BLOCK);
	for (std::size_t n = directory_offset / BLOCK_SIZE; n <= last_directory_block; ++n)
		blocks[n] = DIRECTORY_BLOCK;
	for (std::size_t i = 0; i < entries.size(); ++i) {
		const std::size_t from = entries[i].offset / BLOCK_SIZE;
		const std::size_t exclusive_to = from + BytesToBlocks(entries[i].compressed_size);
		for (std::size_t n = from; n < exclusive_to; ++n)
			blocks[n] = i;
	}
}


std::size_t
Archive::FindFreeSpot(const std::size_t bytes) const
{
	const std::size_t blocks_needed = BytesToBlocks(bytes);
	std::size_t index = 1;
	std::size_t so_far = 0;
	for (; index <= blocks.size(); ++index) {
		if (UNUSED_BLOCK == blocks[index - 1])
			so_far++;
		else
			so_far = 0;
		if (blocks_needed <= so_far)
			break;
	}
	if (blocks.size() < index)
		index = blocks.size();
	return index - so_far;
}


void
Archive::ForEachEntry(std::function<void(const char*)> func) const
{
	for (const auto& entry : entries)
		func(entry.name);
}


void
Archive::ForEachBlock(std::function<void(const char*)> func) const
{
	for (const int content : blocks) {
		switch (content) {
		case UNUSED_BLOCK:
			func("(unused)");
			break;
		case DIRECTORY_BLOCK:
			func("(directory)");
			break;
		default:
			func(entries[content].name);
			break;
		}
	}
}


std::size_t
Archive::DirectoryBlocks() const
{
	const std::size_t blocks = BytesToBlocks(entries.size() * sizeof(Entry));
	return blocks == 0 ? 1 : blocks;
}


std::size_t
BytesToBlocks(const std::size_t bytes)
{
	const std::size_t full = bytes / BLOCK_SIZE;
	const std::size_t partial = (bytes % BLOCK_SIZE) > 0;
	return full + partial;
}


std::size_t
NormalizePath(const char* src, char* dest)
{
	if ('.' == src[0] && ('/' == src[1] || '\\' == src[1]))
		return NormalizePath(&src[2], dest);
	std::size_t i;
	for (i = 0; 0 != src[i]; ++i) {
		if ('\\' == src[i])
			dest[i] = '/';
		else
			dest[i] = src[i];
	}
	return i;
}


}  // namespace ArchiveEx
