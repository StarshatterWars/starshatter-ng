# Creates a wrapper for patch(1) utility that discards exit status in case the patch gets reapplied. This happens
# because CMake applies patch step frequently with ExternalProject and FetchContent. Moreover, GNU patch does not follow
# strict POSIX description of -N option making CMake think patching failed.
#
# For compatibility use with cmake(1) itself:
#
#     COMMAND ${CMAKE_COMMAND} -D PATCH_FILE=something.patch -P ${PATCH_SCRIPT}

find_program(PATCH_COMMAND patch DOC "POSIX-compliant patch utility")
set(PATCH_SCRIPT ${CMAKE_BINARY_DIR}/apply_patch.cmake CACHE FILEPATH "CMake script wrapper for patch command")
configure_file(${CMAKE_CURRENT_LIST_DIR}/apply_patch.cmake.in ${PATCH_SCRIPT} @ONLY)
