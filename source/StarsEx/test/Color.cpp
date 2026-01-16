#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

#include <Color.h>
#include <Types.h>


static void SetPalette();


TEST(StarsEx, IndexedColorToRGB)
{
	SetPalette();

	Color color {};
	EXPECT_NEAR(0, color.Index(), 5);
	EXPECT_EQ(0, color.Red());
	EXPECT_EQ(0, color.Green());
	EXPECT_EQ(0, color.Blue());
	// EXPECT_EQ(255, color.Alpha());  // Indexed colors do not have alpha channel.

	color = Color{16};
	EXPECT_NEAR(16, color.Index(), 5);
	EXPECT_EQ(16, color.Red());
	EXPECT_EQ(16, color.Green());
	EXPECT_EQ(16, color.Blue());
	// EXPECT_EQ(255, color.Alpha());
}


TEST(StarsEx, RGBColorToIndexed)
{
	SetPalette();

	const Color color {50, 50, 50};
	EXPECT_NEAR(50, color.Index(), 5);  // Inverse table is questionable with this palette.
	EXPECT_EQ(50, color.Red());
	EXPECT_EQ(50, color.Green());
	EXPECT_EQ(50, color.Blue());
	EXPECT_EQ(255, color.Alpha());
}


TEST(StarsEx, IndexedColorNotFaded)
{
	SetPalette();

	const Color color {50, 50, 50};
	EXPECT_EQ(50, color.Red());
	EXPECT_EQ(50, color.Green());
	EXPECT_EQ(50, color.Blue());
	Color::SetFade(1.0);
	const auto faded = color.Faded();
	EXPECT_EQ(50, faded.Red());
	EXPECT_EQ(50, faded.Green());
	EXPECT_EQ(50, faded.Blue());
	EXPECT_EQ(color.Alpha(), faded.Alpha());
}


TEST(StarsEx, IndexedColorHalfFaded)
{
	SetPalette();

	const Color color {50, 50, 50};
	EXPECT_EQ(50, color.Red());
	EXPECT_EQ(50, color.Green());
	EXPECT_EQ(50, color.Blue());
	Color::SetFade(0.5);
	const auto faded = color.Faded();
	EXPECT_NEAR(25, faded.Red(), 3);
	EXPECT_NEAR(25, faded.Green(), 3);
	EXPECT_NEAR(25, faded.Blue(), 3);
	EXPECT_EQ(color.Alpha(), faded.Alpha());
}


TEST(StarsEx, DefaultFormattedColorBlend)
{
	SetPalette();

	// Is ColorFormat ever mutated at all?
	const auto index = static_cast<std::uint8_t>(Color::FormattedBlend(20, 80));
	// Why does this seem to do everything?
	const Color blend {index};
	EXPECT_NEAR(100, blend.Red(), 5);
	EXPECT_NEAR(100, blend.Green(), 5);
	EXPECT_NEAR(100, blend.Blue(), 5);
}


void
SetPalette()
{
	std::vector<PALETTEENTRY> palette;
	for (int i = 0; i < 256; ++i) {
		palette.push_back({
			static_cast<std::uint8_t>(i),
			static_cast<std::uint8_t>(i),
			static_cast<std::uint8_t>(i),
			0
		});
	}
	Color::SetPalette(palette.data(), palette.size(), nullptr);
}
