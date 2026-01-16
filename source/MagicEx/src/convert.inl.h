/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include <memory>
#include <string>

#include <Clock.h>
#include <DataLoader.h>
#include <Solid.h>

#include <MagicLoad.h>


namespace starshatter
{


template <typename Input, typename Output>
int
convert(const std::string& input, const std::string& output)
{
	Clock::Init();
	DataLoader::Initialize();
	auto solid = std::make_unique<Solid>();
	if (!ImportInto<Input>(input.c_str(), solid.get()))
		return 1;
	Output exporter {output.c_str()};
	auto* model = solid->GetModel();
	if (model == nullptr)
		return 1;
	if (!exporter.Save(model))
		return 1;
	DataLoader::Close();
	Clock::Close();
	return 0;
}


}  // namespace starshatter
