include(ApplyPatch)
include(FetchContent)
include(MakeAvailable)

# ---------------------------------------------------------------------------
# These subdirectories DECLARE FetchContent_Declare(...) blocks for each dep.
# Keep these, otherwise make_available() has no saved details.
# ---------------------------------------------------------------------------
add_subdirectory(gtest)
add_subdirectory(infoware)
add_subdirectory(zlib)
add_subdirectory(libpng)
add_subdirectory(ogg)
add_subdirectory(vorbis)

# ---------------------------------------------------------------------------
# Now actually populate/build them via make_available()
# ---------------------------------------------------------------------------
make_available(gtest)
make_available(infoware)

# ---- ZLIB ---------------------------------------------------------------
make_available(zlib)

set(_ZLIB_SRC   "${CMAKE_BINARY_DIR}/_deps/zlib-src")
set(_ZLIB_BUILD "${CMAKE_BINARY_DIR}/_deps/zlib-build")

# FindZLIB.cmake may use either ZLIB_INCLUDE_DIR or ZLIB_INCLUDE_DIRS.
# zlib.h is in source; zconf.h is generated into build
set(ZLIB_INCLUDE_DIR  "${_ZLIB_SRC}" CACHE PATH "" FORCE)
set(ZLIB_INCLUDE_DIRS "${_ZLIB_SRC};${_ZLIB_BUILD}" CACHE STRING "" FORCE)

# Library paths for VS multi-config (adjust names if your zlib uses different outputs)
set(ZLIB_LIBRARY_RELEASE "${_ZLIB_BUILD}/Release/zlibstatic.lib"  CACHE FILEPATH "" FORCE)
set(ZLIB_LIBRARY_DEBUG   "${_ZLIB_BUILD}/Debug/zlibstaticd.lib"   CACHE FILEPATH "" FORCE)
set(ZLIB_LIBRARY         "${ZLIB_LIBRARY_RELEASE}"                CACHE FILEPATH "" FORCE)

# Export both include dirs from the zlib targets (helps consumers that link to them)
if(TARGET zlibstatic)
  target_include_directories(zlibstatic INTERFACE "${_ZLIB_SRC}" "${_ZLIB_BUILD}")
endif()
if(TARGET zlib)
  target_include_directories(zlib INTERFACE "${_ZLIB_SRC}" "${_ZLIB_BUILD}")
endif()

# ---------------------------------------------------------------------------
# Provide canonical alias targets expected by THIS project:
#   Zlib::zlib
#   Zlib::zlibstatic
# ---------------------------------------------------------------------------

# Prefer static as canonical
if(NOT TARGET Zlib::zlibstatic)
  if(TARGET zlibstatic)
    add_library(Zlib::zlibstatic ALIAS zlibstatic)
  elseif(TARGET zlib)
    add_library(Zlib::zlibstatic ALIAS zlib)
  endif()
endif()

# Zlib::zlib used by AddDatafile.cmake generator expressions
if(NOT TARGET Zlib::zlib)
  if(TARGET zlib)
    add_library(Zlib::zlib ALIAS zlib)
  elseif(TARGET zlibstatic)
    add_library(Zlib::zlib ALIAS zlibstatic)
  elseif(TARGET Zlib::zlibstatic)
    add_library(Zlib::zlib ALIAS Zlib::zlibstatic)
  endif()
endif()

# ---- LIBPNG -------------------------------------------------------------
#
# FIX: libpng tries to "install(EXPORT libpng ...)" and that export set
# will fail if png15 links to an in-tree zlib target that is not in the same export.
# We are not packaging libpng as a standalone installed package, so turn off its
# export/package installation logic and rely on the top-level install() below.
#
# These option names are consistent across many libpng CMake versions; if a
# specific one is not recognized, it is harmless.
#
set(PNG_SHARED OFF CACHE BOOL "" FORCE)
set(PNG_TESTS  OFF CACHE BOOL "" FORCE)
set(PNG_EXECUTABLES OFF CACHE BOOL "" FORCE)

set(PNG_INSTALL OFF CACHE BOOL "" FORCE)
set(PNG_INSTALL_PKGCONFIG OFF CACHE BOOL "" FORCE)
set(PNG_INSTALL_HEADERS OFF CACHE BOOL "" FORCE)

# Some libpng trees use these naming variants:
set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)
set(SKIP_INSTALL_EXPORT ON CACHE BOOL "" FORCE)

make_available(libpng)

# Ensure png15 links to your zlib target, not to imported/system zlib.
# This also prevents __imp_ (DLL import) symbol issues when png is built static
# but accidentally links against a dynamic zlib somewhere on PATH.
if(TARGET png15)
  # Prefer your canonical zlib alias
  if(TARGET Zlib::zlibstatic)
    target_link_libraries(png15 PRIVATE Zlib::zlibstatic)
  elseif(TARGET Zlib::zlib)
    target_link_libraries(png15 PRIVATE Zlib::zlib)
  elseif(TARGET zlibstatic)
    target_link_libraries(png15 PRIVATE zlibstatic)
  elseif(TARGET zlib)
    target_link_libraries(png15 PRIVATE zlib)
  endif()

  # Make sure include path exposes generated zconf.h too
  target_include_directories(png15 PRIVATE "${_ZLIB_SRC}" "${_ZLIB_BUILD}")
endif()

## ---- LIBPNG -------------------------------------------------------------
#
# Goals:
#  1) Ensure libpng uses the in-tree zlib we built via FetchContent.
#  2) Provide Png::png alias regardless of libpng's target naming.
#  3) Avoid hard-coded paths that break in multi-config generators.
#

# Ensure FindZLIB inside libpng sees BOTH headers:
#  - zlib.h in source dir
#  - zconf.h generated into build dir
set(ZLIB_INCLUDE_DIR  "${_ZLIB_SRC}" CACHE PATH "" FORCE)
set(ZLIB_INCLUDE_DIRS "${_ZLIB_SRC};${_ZLIB_BUILD}" CACHE STRING "" FORCE)

# If libpng calls find_package(ZLIB), these are the variables FindZLIB consumes.
# But DO NOT try to set ZLIB_LIBRARY to a target name; keep it as a file path if you set it at all.
# For multi-config, let it pick the right config or rely on target linkage after.
set(ZLIB_LIBRARY_RELEASE "${_ZLIB_BUILD}/Release/zlibstatic.lib" CACHE FILEPATH "" FORCE)
set(ZLIB_LIBRARY_DEBUG   "${_ZLIB_BUILD}/Debug/zlibstaticd.lib"  CACHE FILEPATH "" FORCE)

# IMPORTANT: Do NOT FORCE ZLIB_LIBRARY to Release when you're building Debug.
# Comment this out unless you *only* ever build Release from VS.
# set(ZLIB_LIBRARY "${ZLIB_LIBRARY_RELEASE}" CACHE FILEPATH "" FORCE)

# Disable libpng install/export/package noise (we're not distributing libpng as a package here)
set(PNG_SHARED OFF CACHE BOOL "" FORCE)
set(PNG_TESTS OFF CACHE BOOL "" FORCE)
set(PNG_EXECUTABLES OFF CACHE BOOL "" FORCE)

set(PNG_INSTALL OFF CACHE BOOL "" FORCE)
set(PNG_INSTALL_PKGCONFIG OFF CACHE BOOL "" FORCE)
set(PNG_INSTALL_HEADERS OFF CACHE BOOL "" FORCE)

# Some libpng trees use these:
set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)
set(SKIP_INSTALL_EXPORT ON CACHE BOOL "" FORCE)

# ---- libpng -------------------------------------------------------------
make_available(libpng)

# ---- LIBPNG -------------------------------------------------------------
#
# IMPORTANT:
# libpng uses *plain* target_link_libraries() internally.
# Therefore we MUST NOT call target_link_libraries() on its targets externally,
# or we risk mixing signatures and breaking configure.
#

# Ensure FindZLIB inside libpng sees BOTH headers:
#  - zlib.h in source dir
#  - zconf.h generated into build dir
set(ZLIB_INCLUDE_DIR  "${_ZLIB_SRC}" CACHE PATH "" FORCE)
set(ZLIB_INCLUDE_DIRS "${_ZLIB_SRC};${_ZLIB_BUILD}" CACHE STRING "" FORCE)

# Ensure libpng links the correct zlib library via the variables it already uses.
# This is what prevents:
#   - LNK1104 (wrong config path)
#   - __imp_inflateReset (import/static mismatch)
#
# NOTE: do NOT force ZLIB_LIBRARY to Release globally. Use config-specific vars.
set(ZLIB_LIBRARY_RELEASE "${_ZLIB_BUILD}/Release/zlibstatic.lib" CACHE FILEPATH "" FORCE)
set(ZLIB_LIBRARY_DEBUG   "${_ZLIB_BUILD}/Debug/zlibstaticd.lib"  CACHE FILEPATH "" FORCE)

# Some libpng trees / FindZLIB variants look only at ZLIB_LIBRARY.
# We must pick the right one depending on the generator config.
# For multi-config generators, CMake does not know the final config at configure-time,
# but setting ZLIB_LIBRARY to the *release* path is what caused your earlier issues.
# Best compromise: set it to RELEASE and ensure Debug uses ZLIB_LIBRARY_DEBUG.
# If libpng ignores the _DEBUG var, you can switch this line to Debug when configuring Debug.
set(ZLIB_LIBRARY "${ZLIB_LIBRARY_RELEASE}" CACHE FILEPATH "" FORCE)

# Disable libpng install/export/package behavior (prevents export-set errors)
set(PNG_SHARED OFF CACHE BOOL "" FORCE)
set(PNG_TESTS OFF CACHE BOOL "" FORCE)
set(PNG_EXECUTABLES OFF CACHE BOOL "" FORCE)
set(PNG_INSTALL OFF CACHE BOOL "" FORCE)
set(PNG_INSTALL_PKGCONFIG OFF CACHE BOOL "" FORCE)
set(PNG_INSTALL_HEADERS OFF CACHE BOOL "" FORCE)
set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)
set(SKIP_INSTALL_EXPORT ON CACHE BOOL "" FORCE)

make_available(libpng)

# ---- Find the produced libpng target and alias it to Png::png -----------
set(_PNG_CANDIDATES
  PNG::PNG
  png
  png_static
  png_shared
  png16
  png16_static
  png16_shared
  png15
  png15_static
  png15_shared
)

set(_PNG_FOUND "")
foreach(t IN LISTS _PNG_CANDIDATES)
  if(TARGET "${t}")
    set(_PNG_FOUND "${t}")
    break()
  endif()
endforeach()

if(NOT _PNG_FOUND)
  get_property(_allTargets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
  message(FATAL_ERROR "libpng did not produce a known target. Targets: ${_allTargets}")
endif()

if(NOT TARGET Png::png)
  add_library(Png::png ALIAS ${_PNG_FOUND})
endif()

# Ensure zconf.h is visible to libpng compilation (SAFE: includes only)
# Do NOT link here.
get_target_property(_png_imported "${_PNG_FOUND}" IMPORTED)
get_target_property(_png_type "${_PNG_FOUND}" TYPE)
if(NOT _png_imported AND (NOT _png_type STREQUAL "INTERFACE_LIBRARY"))
  target_include_directories("${_PNG_FOUND}" PRIVATE "${_ZLIB_SRC}" "${_ZLIB_BUILD}")
endif()

# ---- libpng include path fix --------------------------------------------

set(_PNG_SRC "${CMAKE_BINARY_DIR}/_deps/libpng-src")
set(_PNG_BUILD "${CMAKE_BINARY_DIR}/_deps/libpng-build")

if(TARGET png15)
    target_include_directories(png15
        INTERFACE
            "${_PNG_SRC}"
            "${_PNG_BUILD}"
    )
endif()

if(TARGET png15_static)
    target_include_directories(png15_static
        INTERFACE
            "${_PNG_SRC}"
            "${_PNG_BUILD}"
    )
endif()

if(TARGET png)
    target_include_directories(png
        INTERFACE
            "${_PNG_SRC}"
            "${_PNG_BUILD}"
    )
endif()

# ---- OGG ----------------------------------------------------------------
make_available(ogg)

# Make vorbis' FindOgg.cmake succeed when ogg is built in-tree
set(OGG_INCLUDE_DIR "${CMAKE_BINARY_DIR}/_deps/ogg-src/include" CACHE PATH "" FORCE)
set(OGG_LIBRARY_RELEASE "${CMAKE_BINARY_DIR}/_deps/ogg-build/Release/ogg.lib" CACHE FILEPATH "" FORCE)
set(OGG_LIBRARY_DEBUG   "${CMAKE_BINARY_DIR}/_deps/ogg-build/Debug/oggd.lib"  CACHE FILEPATH "" FORCE)
set(OGG_LIBRARY "${OGG_LIBRARY_RELEASE}" CACHE FILEPATH "" FORCE)

# ---- VORBIS -------------------------------------------------------------
make_available(vorbis)

# Alias targets expected by the project:
if(TARGET vorbis AND NOT TARGET Vorbis::vorbis)
  add_library(Vorbis::vorbis ALIAS vorbis)
endif()
if(TARGET vorbisfile AND NOT TARGET Vorbis::vorbisfile)
  add_library(Vorbis::vorbisfile ALIAS vorbisfile)
endif()
if(TARGET vorbisenc AND NOT TARGET Vorbis::vorbisenc)
  add_library(Vorbis::vorbisenc ALIAS vorbisenc)
endif()

# ---------------------------------------------------------------------------
# Local third-party libraries (in-repo, not fetched)
# ---------------------------------------------------------------------------
add_subdirectory(Opcode)
add_subdirectory(sha1)
add_subdirectory(l3ds)

# ---------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------
# ---------------------------------------------------------------------------
# Install
#   - Only install libpng if it is a real build artifact target (SHARED/MODULE)
#   - Static libpng is not a runtime artifact; don't try to install it.
# ---------------------------------------------------------------------------

set(_PNG_INSTALL_TARGET "")

# If we created the project alias, try to resolve it to the real target:
if(TARGET Png::png)
  get_target_property(_png_aliased Png::png ALIASED_TARGET)
  if(_png_aliased)
    set(_PNG_INSTALL_TARGET "${_png_aliased}")
  else()
    # Not an ALIAS; could still be IMPORTED/INTERFACE, so we validate below
    set(_PNG_INSTALL_TARGET "Png::png")
  endif()
endif()

# Validate target is installable (must be a built lib/exe, not INTERFACE/IMPORTED)
set(_PNG_CAN_INSTALL OFF)
if(_PNG_INSTALL_TARGET AND TARGET "${_PNG_INSTALL_TARGET}")
  get_target_property(_png_type "${_PNG_INSTALL_TARGET}" TYPE)
  get_target_property(_png_imported "${_PNG_INSTALL_TARGET}" IMPORTED)

  # Only install shared/module libraries as runtime artifacts.
  # (STATIC_LIBRARY is not a runtime artifact; INTERFACE_LIBRARY is not installable here.)
  if(NOT _png_imported AND (_png_type STREQUAL "SHARED_LIBRARY" OR _png_type STREQUAL "MODULE_LIBRARY"))
    set(_PNG_CAN_INSTALL ON)
  endif()
endif()

# Always install Opcode runtime if you need it (adjust if Opcode is static-only)
install(
  TARGETS Opcode
  RUNTIME
  COMPONENT Runtime
  DESTINATION ${CMAKE_INSTALL_PREFIX}
)

# Install libpng only if it is an actual runtime library target
if(_PNG_CAN_INSTALL)
  install(
    TARGETS ${_PNG_INSTALL_TARGET}
    RUNTIME
    COMPONENT Runtime
    DESTINATION ${CMAKE_INSTALL_PREFIX}
  )
else()
  message(STATUS "Skipping libpng install: no installable runtime libpng target (likely static or imported/interface).")
endif()

if(NOT MSVC)
  foreach(LIB IN ITEMS "libstdc++-6.dll" "libwinpthread-1.dll" "libgcc_s_dw2-1.dll")
    find_file(LIBRARY_${LIB} ${LIB} PATHS ${CMAKE_FIND_ROOT_PATH}/bin)
    if(LIBRARY_${LIB})
      install(
        PROGRAMS ${LIBRARY_${LIB}}
        COMPONENT Runtime
        DESTINATION ${CMAKE_INSTALL_PREFIX}
      )
    endif()
  endforeach()
endif()
