/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "ColorPalette.h"

#include <cstdint>
#include <vector>

#include <Types.h>

#include "Color.h"


namespace starshatter
{


static std::size_t ReverseKey(const Color& color);
static ColorIndex Closest(const std::vector<Color>& forward, const Color& color);
static std::vector<Color> BuildForwardLookup(const std::vector<PALETTEENTRY>& forward);
static std::vector<ColorIndex> BuildReverseLookup(const std::vector<Color>& forward);
static std::vector<ColorIndex> BuildReverseLookup(const std::vector<std::uint8_t>& reverse);
static std::vector<ColorIndex> BuildBlendLookup(const std::vector<Color>& forward, const std::vector<ColorIndex>& reverse);


ColorPalette::ColorPalette()
{
}


ColorPalette::ColorPalette(const std::vector<PALETTEENTRY>& forward_) :
	forward {BuildForwardLookup(forward_)},
	reverse {BuildReverseLookup(forward)},
	blend {BuildBlendLookup(forward, reverse)}
{
}


ColorPalette::ColorPalette(const std::vector<PALETTEENTRY>& forward_, const std::vector<std::uint8_t>& reverse_) :
	forward {BuildForwardLookup(forward_)},
	reverse {BuildReverseLookup(reverse_)},
	blend {BuildBlendLookup(forward, reverse)}
{
}


std::size_t
ColorPalette::Size() const
{
	return forward.size();
}


Color
ColorPalette::Forward(const ColorIndex& index) const
{
	return forward[index.Index()];
}


const std::vector<Color>&
ColorPalette::Forward() const
{
	return forward;
}


ColorIndex
ColorPalette::Reverse(const Color& color) const
{
	return reverse[ReverseKey(color)];
}


const std::vector<ColorIndex>&
ColorPalette::Reverse() const
{
	return reverse;
}


ColorIndex
ColorPalette::Blend(const ColorIndex& lhs, const ColorIndex& rhs) const
{
	return blend[lhs.Index() * 256 + rhs.Index()];
}


std::size_t
ReverseKey(const Color& color)
{
	return (color.Red() >> 3 << 10) | (color.Green() >> 3 << 5) | (color.Blue() >> 3);
}


ColorIndex
Closest(const std::vector<Color>& forward, const Color& color)
{
	ColorIndex match {};
	double best = 1e+9;
	for (int i = 0; i < forward.size(); ++i) {
		const auto& candidate = forward[i];
		if (candidate == color)
			return i;
		const double dr = candidate.Red() - color.Red();
		const double dg = candidate.Green() - color.Green();
		const double db = candidate.Blue() - color.Blue();
		const double d2 = (dr * dr) + (dg * dg) + (db * db);
		if (d2 < best) {
			best = d2;
			match = i;
		}
	}
	return match;
}


std::vector<Color>
BuildForwardLookup(const std::vector<PALETTEENTRY>& forward_)
{
	std::vector<Color> forward;
	forward.reserve(forward_.size());
	for (const auto& entry : forward_)
		forward.push_back(Color{entry.peRed, entry.peGreen, entry.peBlue});
	return forward;
}


std::vector<ColorIndex>
BuildReverseLookup(const std::vector<Color>& forward)
{
	static constexpr int SIZE {32768};
	std::vector<ColorIndex> reverse;
	reverse.reserve(SIZE);
	for (int i = 0; i < SIZE; ++i) {
		const std::uint8_t r = ((i >> 10) & 0x1f) << 3;
		const std::uint8_t g = ((i >> 5) & 0x1f) << 3;
		const std::uint8_t b = (i & 0x1f) << 3;
		reverse.push_back(Closest(forward, {r, g, b}));  // O(SIZE * n)
	}
	return reverse;
}


std::vector<ColorIndex>
BuildReverseLookup(const std::vector<std::uint8_t>& reverse_)
{
	std::vector<ColorIndex> reverse;
	reverse.reserve(reverse_.size());
	for (const auto& index : reverse_)
		reverse.push_back(ColorIndex{index});
	return reverse;
}


std::vector<ColorIndex>
BuildBlendLookup(const std::vector<Color>& forward, const std::vector<ColorIndex>& reverse)
{
	std::vector<ColorIndex> blend;
	blend.resize(256 * 256);
	for (int lhs = 0; lhs < 256; ++lhs) {
		const auto left = forward[lhs];
		for (int rhs = 0; rhs < 256; ++rhs) {
			const auto right = forward[rhs];
			blend[lhs * 256 + rhs] = reverse[ReverseKey(left + right.Value())];
		}
	}
	return blend;
}


}  // namespace starshatter
