#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>

#include <FilesystemDataSource.h>


namespace fs = std::filesystem;


TEST(FoundationEx, OpenRelativePathOnFilesystem)
{
	fs::current_path(fs::temp_directory_path());
	{
		std::fstream tmp("sample", tmp.out | tmp.binary);
		tmp << "Hello, there!\n";
	}
	FileSystemDataSource src {};
	auto file = src.Open("", "sample");
	ASSERT_TRUE(file.valid());
	ASSERT_LT(0, file.available());
	const auto text = file.more();
	ASSERT_STREQ("Hello, there!\n", text);
}
