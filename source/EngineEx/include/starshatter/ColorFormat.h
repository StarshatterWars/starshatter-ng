/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#pragma once

#include <cstdint>


namespace starshatter
{


struct ColorFormat
{
	ColorFormat();
	ColorFormat(int pal);
	ColorFormat(
		int bpp,
		std::uint8_t r, std::uint8_t rs,
		std::uint8_t g, std::uint8_t gs,
		std::uint8_t b, std::uint8_t bs,
		std::uint8_t a=0, std::uint8_t as=0);

	int pal;
	int bpp;
	std::uint8_t rdown, rshift;
	std::uint8_t gdown, gshift;
	std::uint8_t bdown, bshift;
	std::uint8_t adown, ashift;
	std::uint32_t rmask;
	std::uint32_t gmask;
	std::uint32_t bmask;
	std::uint32_t amask;
};


}  // namespace starshatter
