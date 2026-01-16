FetchContent_Declare(
	infoware
	URL https://github.com/ThePhD/infoware/archive/e469b86.tar.gz
	URL_HASH SHA1=e844ae80c5ca3f6b944571130ac3795c96a4e9ab
	PATCH_COMMAND ${CMAKE_COMMAND} -D "PATCH_FILE=${CMAKE_CURRENT_SOURCE_DIR}/1.current.patch" -P ${PATCH_SCRIPT}
	DOWNLOAD_EXTRACT_TIMESTAMP No)
