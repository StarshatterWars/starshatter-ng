#pragma once

#include <Text.h>


namespace InfoEx
{


Text ShortDescription();
Text LongDescription();


enum struct DXVersion : int
{
    None,
    Dx7 = 7,
    Dx8,
    Dx9,
    Future,
};


DXVersion DirectX();


}  // namespace InfoEx
