# Adds a target that creates a datafile archive containing all files under selected source directory tree. For example:
#
#     add_datafile(
#         shatter
#         SOURCE shatter
#         OUTPUT shatter.dat
#         RESOURCES resource_shatter
#     )
#
# Will create 'shatter.dat' in the respective binary directory containing all files in 'shatter' subdirectory of current
# source directory. SOURCE option's argument defaults to the target name. OUTPUT option's argument defaults to the
# target name with ".dat" suffix.
#
# Optionally, RESOURCES can be used to specify binary resource targets from AddDownloadableResource that will provide
# additional directories with files that will also get included in the final datafile. In case of conflicts, files from
# source tree will be used. This is to allow working with binary assets located in source directory.
#
# SKIP_SOURCE option can be specified to skip adding the files from SOURCE directory completely.
#
# To create the archive the dat.exe is used - for non-Windows platforms an emulator that can run it is required.

function(add_datafile DATAFILE_TARGET)
	cmake_parse_arguments(
		PARSE_ARGV 1
		DATAFILE
		"SKIP_SOURCE"
		"SOURCE;OUTPUT"
		"RESOURCES"
		)
	if(NOT DEFINED DATAFILE_SOURCE)
		set(DATAFILE_SOURCE ${DATAFILE_TARGET})
	endif()
	if(NOT DEFINED DATAFILE_OUTPUT)
		set(DATAFILE_OUTPUT "${DATAFILE_TARGET}.dat")
	endif()
	cmake_path(ABSOLUTE_PATH DATAFILE_SOURCE NORMALIZE BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
	cmake_path(ABSOLUTE_PATH DATAFILE_OUTPUT NORMALIZE BASE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
	if(CMAKE_HOST_SYSTEM_NAME MATCHES Windows)
		set(DATAFILE_VAR PATH)
		set(DATAFILE_PATHS $ENV{PATH})
	else()
		set(DATAFILE_VAR WINEPATH)
		set(DATAFILE_PATHS $ENV{WINEPATH})
	endif()
	list(PREPEND DATAFILE_PATHS $<TARGET_FILE_DIR:Zlib::zlib>)
	add_custom_command(
		OUTPUT ${DATAFILE_OUTPUT}
		COMMAND ${CMAKE_COMMAND} -E rm -f ${DATAFILE_OUTPUT}
		COMMENT "Creating datafile ${DATAFILE_TARGET}"
		)
	get_target_property(EMULATOR dat CROSSCOMPILING_EMULATOR)
	foreach(RESOURCE_TARGET IN ITEMS ${DATAFILE_RESOURCES})
		get_property(RESOURCE_DIRECTORY TARGET ${RESOURCE_TARGET} PROPERTY RESOURCE_DIRECTORY)
		add_custom_command(
			OUTPUT ${DATAFILE_OUTPUT} APPEND
			DEPENDS ${RESOURCE_TARGET}
			COMMAND
				${CMAKE_COMMAND} -E chdir ${RESOURCE_DIRECTORY}
				${CMAKE_COMMAND} -E env "${DATAFILE_VAR}='${DATAFILE_PATHS}'"
				${EMULATOR} $<TARGET_FILE:dat> -cu $<SHELL_PATH:${DATAFILE_OUTPUT}> .
			)
	endforeach()
	if(NOT DATAFILE_SKIP_SOURCE)
		file(
			GLOB_RECURSE DATAFILE_DEPENDENCIES
			LIST_DIRECTORIES true
			CONFIGURE_DEPENDS
			"${DATAFILE_SOURCE}/*"
			)
		add_custom_command(
			OUTPUT ${DATAFILE_OUTPUT} APPEND
			DEPENDS ${DATAFILE_DEPENDENCIES}
			COMMAND
				${CMAKE_COMMAND} -E chdir ${DATAFILE_SOURCE}
				${CMAKE_COMMAND} -E env "${DATAFILE_VAR}='${DATAFILE_PATHS}'"
				${EMULATOR} $<TARGET_FILE:dat> -cu $<SHELL_PATH:${DATAFILE_OUTPUT}> .
			)
	endif()
	add_custom_target(${DATAFILE_TARGET} ALL DEPENDS ${DATAFILE_OUTPUT})
endfunction()
