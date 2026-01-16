#include <cstring>
#include <memory>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include <starshatter/foundation/reader.h>
#include <Text.h>


using starshatter::foundation::Direction;
using starshatter::foundation::Reader;


TEST(FoundationEx, ReadFromView)
{
	Reader reader("Hello, World!");
	ASSERT_TRUE(reader.valid());
	const auto s = reader.available();
	std::vector<char> buffer(s);
	ASSERT_EQ(14, buffer.size());
	const auto bytes = reader.read(buffer.data());
	ASSERT_EQ(14, bytes);
	ASSERT_EQ(0, reader.available());
	ASSERT_STREQ("Hello, World!", buffer.data());
}


TEST(FoundationEx, ReadChunks)
{
	Reader reader("aaaabbbbccccdddd");
	ASSERT_TRUE(reader.valid());
	const auto s = reader.available();
	std::vector<char> buffer(9);
	const auto bytes = reader.read(buffer.data(), 4, 2);
	ASSERT_EQ(8, bytes);
	ASSERT_EQ(9, reader.available());
	buffer[8] = 0;
	ASSERT_STREQ("aaaabbbb", buffer.data());
}


TEST(FoundationEx, PeekIntoView)
{
	Reader reader("Hello, World!");
	ASSERT_TRUE(reader.valid());
	reader.seek(7);
	std::vector<char> buffer(reader.available());
	ASSERT_EQ(7, buffer.size());
	const auto bytes = reader.peek(buffer.data());
	ASSERT_EQ(7, bytes);
	ASSERT_EQ(7, reader.available());
	ASSERT_STREQ("World!", buffer.data());
}


TEST(FoundationEx, RelativeSeek)
{
	Reader reader("Hello, World!");
	ASSERT_TRUE(reader.valid());
	ASSERT_EQ(11, reader.seek(-3, Direction::End));
	EXPECT_EQ(8, reader.seek(-3, Direction::Current));
	ASSERT_EQ(5, reader.seek(5, Direction::Start));
	EXPECT_EQ(7, reader.seek(2, Direction::Current));
}


TEST(FoundationEx, CreateTextFromReader)
{
	const char* ref = "Hello!";
	auto ptr = std::make_unique<char[]>(std::strlen(ref) + 1);
	std::strcpy(ptr.get(), ref);
	Reader reader(std::move(ptr));
	ASSERT_TRUE(reader.valid());
	const auto text = reader.more();
	ASSERT_STREQ(ref, text);
}
