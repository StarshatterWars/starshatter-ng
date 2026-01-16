# Installs content of a downloadable resource target from AddDownloadableResource as-is. For example:
#
#     install_resource(
#         resource_music
#         DESTINATION ${CMAKE_INSTALL_PREFIX}/
#     )
#
# Will copy the content of the music resource directly into the CMAKE_INSTALL_PREFIX effectively rooting the archive
# content at the prefix itself.

function(install_resource)
	cmake_parse_arguments(
		PARSE_ARGV 0
		INSTALL_RESOURCE
		""
		"DESTINATION;COMPONENT"
		""
		)
	if(NOT DEFINED INSTALL_RESOURCE_DESTINATION)
		set(INSTALL_RESOURCE_DESTINATION ${CMAKE_INSTALL_DATADIR}/)
	endif()
	if(NOT DEFINED INSTALL_RESOURCE_COMPONENT)
		set(INSTALL_RESOURCE_COMPONENT ${CMAKE_INSTALL_DEFAULT_COMPONENT_NAME})
	endif()
	foreach(RESOURCE_TARGET IN ITEMS ${INSTALL_RESOURCE_UNPARSED_ARGUMENTS})
		get_property(RESOURCE_DIRECTORY TARGET ${RESOURCE_TARGET} PROPERTY RESOURCE_DIRECTORY)
		install(
			DIRECTORY ${RESOURCE_DIRECTORY}/
			COMPONENT ${INSTALL_RESOURCE_COMPONENT}
			DESTINATION ${INSTALL_RESOURCE_DESTINATION}
			)
	endforeach()
endfunction()
