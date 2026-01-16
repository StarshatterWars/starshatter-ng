#!/bin/sh
# Lists interesting files in package in $1. Output can be modified by setting $2 to "-print0".  By default it lists all
# files that are usually not part of the repository. This can be reverted by setting $3 to "!". For example:
#
#     $ ./files.sh start -print !
#
# Will list all "non-binary" files that are below 'start/' directory. In case of unmodified state of the repository this
# means a couple of *.def files.

test -d "$1" || exit 1
exec find "$1" -type f -a $3 \( \
	-iname '*.pcx' -o \
	-iname '*.jpg' -o \
	-iname '*.mag' -o \
	-iname '*.wav' -o \
	-iname '*.pal' -o \
	-iname '*.ogg' -o \
	-iname '*.ipl' \) ${2:--print}
