# Converts git version name acquired by GitDescribe to a format that can be embedded into rc file.

function(git_to_resource VAR VERSION)
	string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)-?([0-9]*)" OUTPUT ${VERSION})
	set(RC_VERSION "${CMAKE_MATCH_1},${CMAKE_MATCH_2},${CMAKE_MATCH_3},${CMAKE_MATCH_4}")
	if(NOT DEFINED CMAKE_MATCH_4)
		string(APPEND RC_VERSION "0")
	endif()
	set(${VAR} ${RC_VERSION} PARENT_SCOPE)
endfunction()
