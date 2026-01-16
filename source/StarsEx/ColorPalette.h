/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#pragma once

#include <cstdint>
#include <vector>

// TODO: Remove Types.h, PALETTEENTRY
#include <Types.h>

// TODO: Refactor so that an include is doable.
class Color;
class ColorIndex;


namespace starshatter
{


class ColorPalette
{
public:
	ColorPalette();
	ColorPalette(const std::vector<PALETTEENTRY>& forward);
	ColorPalette(const std::vector<PALETTEENTRY>& forward, const std::vector<std::uint8_t>& reverse);

	std::size_t Size() const;
	Color Forward(const ColorIndex& index) const;
	const std::vector<Color>& Forward() const;
	ColorIndex Reverse(const Color& index) const;
	const std::vector<ColorIndex>& Reverse() const;
	ColorIndex Blend(const ColorIndex& lhs, const ColorIndex& rhs) const;

private:
	std::vector<Color> forward;
	std::vector<ColorIndex> reverse;
	std::vector<ColorIndex> blend;
};


}  // namespace starshatter
