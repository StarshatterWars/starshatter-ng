# Populates all declared FetchContent projects. Use this once after everything is set up:
#
#     FetchContent_Declare(zlib ...)
#     FetchContent_Declare(libpng ...)
#     make_available(zlib libpng)

function(make_available)
	foreach(DEPNAME IN LISTS ARGN)
		string(TOLOWER "${DEPNAME}" DEPNAME)
		FetchContent_GetProperties(${DEPNAME})
		if(NOT ${DEPNAME}_POPULATED)
			FetchContent_Populate(${DEPNAME})
			add_subdirectory(${${DEPNAME}_SOURCE_DIR} ${${DEPNAME}_BINARY_DIR} EXCLUDE_FROM_ALL)
		endif()
	endforeach()
endfunction()
