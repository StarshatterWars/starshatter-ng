#include <gtest/gtest.h>

#include <starshatter/ColorFormat.h>


namespace starshatter
{


TEST(EngineEx, DefaultColorFormatIsARGB32)
{
	const ColorFormat format;
	ASSERT_EQ(0, format.pal);
	EXPECT_EQ(32, format.bpp);
	EXPECT_EQ(0x00ff0000, format.rmask);
	EXPECT_EQ(0x0000ff00, format.gmask);
	EXPECT_EQ(0x000000ff, format.bmask);
	EXPECT_EQ(0xff000000, format.amask);
	EXPECT_EQ(16, format.rshift);
	EXPECT_EQ(8, format.gshift);
	EXPECT_EQ(0, format.bshift);
	EXPECT_EQ(24, format.ashift);
	// No expectations about [rgba]down members.
}


TEST(EngineEx, CanCreatePaletteColorFormat)
{
	const ColorFormat format(256);
	ASSERT_EQ(256, format.pal);
	EXPECT_EQ(8, format.bpp);
	// Other members are not intended for palette ColorFormat.
}


TEST(EngineEx, DoNotCareAboutSmallerColorPaletteBpp)
{
	const ColorFormat format(128);
	ASSERT_EQ(128, format.pal);
	// EXPECT_EQ(8, format.bpp);  // Hard-coded by legacy, we do not care even if it may be questionable.
}


}  // namespace starshatter
