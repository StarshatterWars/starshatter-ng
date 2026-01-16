# You need to have MinGW installed in /usr/i686-w64-mingw32 to use this toolchain without changes.
# Use it with e.g. command line option:
#
#     $ cmake .. --toolchain=../cmake/toolchains/i686-w64-mingw32.cmake
#
# Or use CMAKE_TOOLCHAIN_FILE variable.

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR i686)
set(TOOLCHAIN_DIR "${CMAKE_CURRENT_LIST_DIR}")

set(TOOLCHAIN_PREFIX i686-w64-mingw32)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)
set(CMAKE_MC_COMPILER ${TOOLCHAIN_PREFIX}-windmc)

set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
