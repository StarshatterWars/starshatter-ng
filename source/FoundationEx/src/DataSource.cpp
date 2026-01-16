/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "DataSource.h"


int DataSource::s_next_id {0};


DataSource::DataSource() :
    m_id {s_next_id++}
{
}


DataSource::~DataSource()
{
}


int
DataSource::Id() const
{
    return m_id;
}
