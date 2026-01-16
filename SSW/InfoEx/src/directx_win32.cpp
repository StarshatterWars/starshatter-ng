#ifdef _WIN32

#include <InfoEx.h>

#include <windows.h>

#include <Text.h>


namespace InfoEx
{


DXVersion
DirectX()
{
	auto lib = LoadLibrary("D3D9.DLL");
	if (lib == nullptr)
		return DXVersion::None;
	FreeLibrary(lib);
	return DXVersion::Dx9;
}


}  // namespace InfoEx


#endif  // _WIN32
