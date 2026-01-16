#include <utility>

#include <gtest/gtest.h>

#include <Text.h>


TEST(FoundationEx, DefaultConstructedTextIsEmpty)
{
	Text t;
	ASSERT_EQ(0, t.length());
	ASSERT_TRUE(t.empty());
}


TEST(FoundationEx, TextCanBeInitializedWithLiteral)
{
	Text t {"Hello, there"};
	ASSERT_EQ("Hello, there", t);
	ASSERT_EQ(12, t.length());
}


TEST(FoundationEx, TextCanBeCopied)
{
	Text a {"Hello, there"};
	Text b(a);
	ASSERT_EQ(a, b);
}


TEST(FoundationEx, ConcatenateTextWithLiteralWithoutSideEffects)
{
	Text a {"Hello"};
	const auto b = a + ", there";
	ASSERT_EQ("Hello", a);
	ASSERT_EQ("Hello, there", b);
}


TEST(FoundationEx, ReplaceInTextWithoutSideEffects)
{
	Text a {"Hello, all"};
	const auto b = a.replace("Hello", "Goodbye");
	ASSERT_EQ("Hello, all", a);
	ASSERT_EQ("Goodbye, all", b);
}


TEST(FoundationEx, SubstringFromTextWithoutSideEffects)
{
	Text a {"Hello, all"};
	const auto b = a.substring(7, 3);
	ASSERT_EQ("Hello, all", a);
	ASSERT_EQ("all", b);
}


TEST(FoundationEx, TrimTextWithoutSideEffects)
{
	Text a {"  Hi! "};
	const auto b = a.trim();
	ASSERT_EQ("  Hi! ", a);
	ASSERT_EQ("Hi!", b);
}
