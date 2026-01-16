#!/bin/sh
install_component() {
	DESTDIR=$staging cmake --install $builddir --component $1
}
install_component Copyright
install_component Runtime
install_component Tools
butler push $staging $target:$branch-executables-only-windows-x86 --userversion-file $builddir/version.txt
install_component Resources
butler push $staging $target:$branch-full-windows-x86 --userversion-file $builddir/version.txt
