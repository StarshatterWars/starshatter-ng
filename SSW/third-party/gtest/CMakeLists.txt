set(BUILD_GMOCK OFF CACHE INTERNAL "")
set(INSTALL_GTEST OFF CACHE INTERNAL "")
FetchContent_Declare(
	GTest
	URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.tar.gz
	URL_HASH SHA1=bfa4b5131b6eaac06962c251742c96aab3f7aa78
	DOWNLOAD_EXTRACT_TIMESTAMP No)
