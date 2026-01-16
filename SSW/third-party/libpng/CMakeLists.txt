FetchContent_Declare(
	libpng
	URL
		https://download.sourceforge.net/libpng/libpng-1.5.1.tar.gz
		https://ftp.ignore.pl/starshatter/resources/mirror/libpng-1.5.1.tar.gz
	URL_HASH SHA1=29afc7422d4d8af1066f8783ae148e298520d5ba
	PATCH_COMMAND ${CMAKE_COMMAND} -D "PATCH_FILE=${CMAKE_CURRENT_SOURCE_DIR}/1.zlib.patch" -P ${PATCH_SCRIPT}
	PATCH_COMMAND ${CMAKE_COMMAND} -D "PATCH_FILE=${CMAKE_CURRENT_SOURCE_DIR}/2.export.patch" -P ${PATCH_SCRIPT}
	PATCH_COMMAND ${CMAKE_COMMAND} -D "PATCH_FILE=${CMAKE_CURRENT_SOURCE_DIR}/3.includes.patch" -P ${PATCH_SCRIPT}
	PATCH_COMMAND ${CMAKE_COMMAND} -D "PATCH_FILE=${CMAKE_CURRENT_SOURCE_DIR}/4.aliases.patch" -P ${PATCH_SCRIPT}
	DOWNLOAD_EXTRACT_TIMESTAMP No)
