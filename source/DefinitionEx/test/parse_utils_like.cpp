#include <gtest/gtest.h>

#include <starshatter/definition.h>


TEST(DefinitionEx, ParseUtilsLike)
{
	std::string content {
		"TEST\n\n"
		"number: 3\n"
		"bool: true\n"
		"short: hello\n"
		"longer: \"hello there\"\n"
		"array: (0.98, -1.0, 0.35)\n"
		"struct: {\n"
		"	name: \"trailing comma\",\n"
		"}\n"
	};
	Parser parser({content.data(), content.size()});
	Term* term = parser.ParseTerm();
	ASSERT_NE(nullptr, term);
	{
		TermText* text = term->isText();
		ASSERT_NE(nullptr, text);
		EXPECT_STREQ("TEST", text->value());
	}
	{
		delete term;
		term = parser.ParseTerm();
		ASSERT_NE(nullptr, term);
		TermDef* def = term->isDef();
		ASSERT_NE(nullptr, def);
		EXPECT_STREQ("number", def->name()->value());
		TermNumber* num = def->term()->isNumber();
		ASSERT_NE(nullptr, num);
		EXPECT_EQ(3, static_cast<int>(num->value()));
	}
	{
		delete term;
		term = parser.ParseTerm();
		ASSERT_NE(nullptr, term);
		TermDef* def = term->isDef();
		ASSERT_NE(nullptr, def);
		EXPECT_STREQ("bool", def->name()->value());
		TermBool* boolean = def->term()->isBool();
		ASSERT_NE(nullptr, boolean);
		EXPECT_TRUE(boolean->value());
	}
	{
		delete term;
		term = parser.ParseTerm();
		ASSERT_NE(nullptr, term);
		TermDef* def = term->isDef();
		ASSERT_NE(nullptr, def);
		EXPECT_STREQ("short", def->name()->value());
		TermText* text = def->term()->isText();
		ASSERT_NE(nullptr, text);
		EXPECT_STREQ("hello", text->value());
	}
	{
		delete term;
		term = parser.ParseTerm();
		ASSERT_NE(nullptr, term);
		TermDef* def = term->isDef();
		ASSERT_NE(nullptr, def);
		EXPECT_STREQ("longer", def->name()->value());
		TermText* text = def->term()->isText();
		ASSERT_NE(nullptr, text);
		EXPECT_STREQ("hello there", text->value());
	}
	{
		delete term;
		term = parser.ParseTerm();
		ASSERT_NE(nullptr, term);
		TermDef* def = term->isDef();
		ASSERT_NE(nullptr, def);
		EXPECT_STREQ("array", def->name()->value());
		TermArray* array = def->term()->isArray();
		ASSERT_NE(nullptr, array);
		ASSERT_EQ(3, array->elements()->size());
		EXPECT_FLOAT_EQ(0.98, array->elements()->at(0)->isNumber()->value());  // what if not a number?
		EXPECT_FLOAT_EQ(-1.0, array->elements()->at(1)->isNumber()->value());
		EXPECT_FLOAT_EQ(0.35, array->elements()->at(2)->isNumber()->value());
	}
	{
		delete term;
		term = parser.ParseTerm();
		ASSERT_NE(nullptr, term);
		TermDef* def = term->isDef();
		ASSERT_NE(nullptr, def);
		EXPECT_STREQ("struct", def->name()->value());
		ASSERT_NE(nullptr, def->term());
		TermStruct* strukt = def->term()->isStruct();
		ASSERT_NE(nullptr, strukt);
		ASSERT_EQ(1, strukt->elements()->size());
		def = strukt->elements()->at(0)->isDef();
		ASSERT_NE(nullptr, def);
		EXPECT_STREQ("name", def->name()->value());
		TermText* text = def->term()->isText();
		ASSERT_NE(nullptr, text);
		EXPECT_STREQ("trailing comma", text->value());
	}
	Token::close();
}
