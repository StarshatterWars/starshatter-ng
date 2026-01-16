/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "starshatter/ColorFormat.h"

#include <cstdint>


namespace starshatter
{


ColorFormat::ColorFormat() :
	ColorFormat(32, 8, 16, 8, 8, 8, 0, 8, 24)
{
}


ColorFormat::ColorFormat(const int pal_) :
	pal {pal_},
	bpp {8}
{
}


ColorFormat::ColorFormat(
		const int bpp_,
		const std::uint8_t r, const std::uint8_t rs,
		const std::uint8_t g, const std::uint8_t gs,
		const std::uint8_t b, const std::uint8_t bs,
		const std::uint8_t a, const std::uint8_t as) :
	pal {0},
	bpp {bpp_},
	rdown (8 - r), rshift {rs}, rmask {((1u << r) - 1) << rs},
	gdown (8 - g), gshift {gs}, gmask {((1u << g) - 1) << gs},
	bdown (8 - b), bshift {bs}, bmask {((1u << b) - 1) << bs},
	adown (8 - a), ashift {as}, amask {((1u << a) - 1) << as}
{
}


}  // namespace starshatter
