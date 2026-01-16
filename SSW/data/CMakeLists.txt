include(AddDatafile)
include(AddDownloadableResource)
include(InstallResource)
add_downloadable_resource(
	resource_content ALL
	SHA1 3b4ffe85a9445e39366c5b503ad192388fab0d55
	ARCHIVE content.tar.xz
	)
add_downloadable_resource(
	resource_shatter ALL
	SHA1 bbed14649213a45499e0a158142f19fd619a54af
	ARCHIVE shatter.tar.xz
	)
add_downloadable_resource(
	resource_start ALL
	SHA1 71e5dba8b5d52e5875391a65a6d28b39ac84fedc
	ARCHIVE start.tar.xz
	)
add_downloadable_resource(
	resource_vox_campaign ALL
	SHA1 bdeb71b2ece4d14fa586ce44147497781120fa8c
	ARCHIVE vox_campaign.tar.xz
	)
add_downloadable_resource(
	resource_vox_music ALL
	SHA1 563f05cf62b2eb029ec56efc7c91992d972d20ff
	ARCHIVE vox_music.tar.xz
	)
add_downloadable_resource(
	resource_vox_original ALL
	SHA1 9fd36681c7a51d39f91aae84ffc3e69cfe2b80ce
	ARCHIVE vox_original.tar.xz
	)
add_datafile(content RESOURCES resource_content)
add_datafile(shatter RESOURCES resource_shatter)
add_datafile(start RESOURCES resource_start)
add_datafile(vox SKIP_SOURCE RESOURCES resource_vox_original)
install(
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/content.dat
		${CMAKE_CURRENT_BINARY_DIR}/shatter.dat
		${CMAKE_CURRENT_BINARY_DIR}/start.dat
		${CMAKE_CURRENT_BINARY_DIR}/vox.dat
		${CMAKE_CURRENT_SOURCE_DIR}/client.cfg
	COMPONENT Resources
	DESTINATION ${CMAKE_INSTALL_PREFIX}
	)
install_resource(
	resource_vox_campaign
	resource_vox_music
	COMPONENT Resources
	DESTINATION ${CMAKE_INSTALL_PREFIX}/
	)
