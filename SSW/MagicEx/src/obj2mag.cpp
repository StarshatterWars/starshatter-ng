/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include <string>

#include <ModelFileMAG.h>
#include <ModelFileOBJ.h>

#include "convert.h"


int
main(int argc, char* argv[])
{
	if (argc < 2)
		return 2;
	const std::string input {argv[1]};
	const auto output = input + ".mag";
	return starshatter::convert<ModelFileOBJ, ModelFileMAG>(input, output);
}
