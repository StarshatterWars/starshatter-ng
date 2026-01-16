#!/bin/sh
cmake -B $builddir -S $srcdir --toolchain=$srcdir/cmake/toolchains/i686-w64-mingw32.cmake --install-prefix=/starshatter
