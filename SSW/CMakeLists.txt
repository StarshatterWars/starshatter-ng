cmake_minimum_required(VERSION 3.24)
project(Starshatter)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS Yes)
set(CMAKE_SYSTEM_VERSION 10.0.22621.0 CACHE STRING "Windows SDK version" FORCE)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
enable_testing()
include(AddVersionFile)
include(GenerateEmulator)
include(GoogleTest)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	if(CMAKE_HOST_SYSTEM_NAME MATCHES Windows)
		set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Starshatter" CACHE PATH "Prefix to install to" FORCE)
	else()
		set(CMAKE_INSTALL_PREFIX "/opt/starshatter" CACHE PATH "Prefix to install to" FORCE)
	endif()
endif()

#----------------------------------------------------------------------------
# DirectX 9 (June 2010 SDK) – required for d3dx9.h
# ---------------------------------------------------------------------------
if(WIN32)
    if(NOT DEFINED DX_DIR)
        set(DX_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)")
    endif()

    set(DX_INCLUDE_DIR "${DX_DIR}/Include")
    set(DX_LIB_DIR     "${DX_DIR}/Lib/x64")   # use x86 if 32-bit

    include_directories(${DX_INCLUDE_DIR})
    link_directories(${DX_LIB_DIR})
endif()

add_subdirectory(third-party)
add_subdirectory(ArchiveEx)
add_subdirectory(data)
add_subdirectory(DefinitionEx)
add_subdirectory(EngineEx)
add_subdirectory(FoundationEx)
add_subdirectory(InfoEx)
add_subdirectory(MagicEx)
if(MSVC)
	add_subdirectory(Magic2)
endif()
add_subdirectory(NetEx)
add_subdirectory(Starserver)
add_subdirectory(StarsEx)
add_subdirectory(Starshatter)
add_subdirectory(TypesEx)
add_version_file(version.txt version.txt.conf ALL)
install(
	FILES NOTICE COPYING
	COMPONENT Copyright
	DESTINATION ${CMAKE_INSTALL_PREFIX}
	)
