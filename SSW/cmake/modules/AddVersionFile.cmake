# Adds a target that generates file with version information:
#
#     add_version_file(version.txt version.txt.conf ALL)
#
# Where 'version.txt.conf' is a template file which has referenced like "@VARIABLE@" substituted with appropriate
# values.

set(ADD_VERSION_FILE_DIR "${CMAKE_CURRENT_LIST_DIR}")
function(add_version_file OUTPUT TEMPLATE)
	cmake_parse_arguments(PARSE_ARGV 2 VERSION_FILE "ALL" "" "")
	if(VERSION_FILE_ALL)
		set(VERSION_FILE_ALL_ARG ALL)
	endif()
	add_custom_target(
		${OUTPUT}
		${VERSION_FILE_ALL_ARG}
		BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT}"
		DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${TEMPLATE}"
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		COMMAND
			cmake
			-DCMAKE_MODULE_PATH='${ADD_VERSION_FILE_DIR}'
			-DOUTPUT='${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT}'
			-DTEMPLATE='${CMAKE_CURRENT_SOURCE_DIR}/${TEMPLATE}'
			-P "${ADD_VERSION_FILE_DIR}/generate_version_file.cmake")
endfunction()
