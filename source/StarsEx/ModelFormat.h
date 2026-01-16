/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#pragma once

#include "DataSource.h"
#include "Solid.h"


namespace starshatter {


struct ModelFormat
{
	virtual ~ModelFormat() = default;
	virtual bool Load(Model* model, const DataSource* source, const char* filename, double scale=1.0) const = 0;
	virtual bool Save(const Model* model, const char* filename) const = 0;
};


}  // namespace starshatter
