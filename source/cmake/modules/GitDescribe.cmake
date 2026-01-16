# Finds a human readable version name for current repository revision based on available tags. Provided variable VAR
# will be set to contain found value.

function(git_describe VAR)
	execute_process(
		COMMAND git describe --tags --dirty --broken
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		RESULT_VARIABLE DESCRIBE_RESULT
		OUTPUT_VARIABLE DESCRIBE_OUTPUT
		OUTPUT_STRIP_TRAILING_WHITESPACE
		)
	if(NOT DESCRIBE_RESULT)
		set(${VAR} ${DESCRIBE_OUTPUT} PARENT_SCOPE)
	else()
		execute_process(
			COMMAND git rev-parse --short HEAD
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			RESULT_VARIABLE PARSE_RESULT
			OUTPUT_VARIABLE PARSE_OUTPUT
			OUTPUT_STRIP_TRAILING_WHITESPACE
			)
		if(NOT PARSE_RESULT)
			set(${VAR} "Unknown-g${PARSE_OUTPUT}" PARENT_SCOPE)
		else()
			set(${VAR} "Unknown" PARENT_SCOPE)
		endif()
	endif()
endfunction()
