/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include <memory>

#include <Solid.h>


template <typename ModelLoader>
bool
ImportInto(const char* pathname, Solid* target)
{
	ModelLoader loader {pathname};
	if (auto* original = target->GetModel()) {
		auto solid = std::make_unique<Solid>();
		if (!solid->Load(&loader))
			return false;
		auto* imported = solid->GetModel();
		original->GetMaterials().append(imported->GetMaterials());
		original->GetSurfaces().append(imported->GetSurfaces());
		original->OptimizeMaterials();
		imported->GetMaterials().clear();
		imported->GetSurfaces().clear();
		return true;
	}
	return target->Load(&loader);
}


