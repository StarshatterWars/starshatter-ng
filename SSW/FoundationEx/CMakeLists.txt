project(FoundationEx)

add_library(
	${PROJECT_NAME} STATIC
	src/DataSource.cpp
	src/FilesystemDataSource.cpp
	src/OverlayDataSource.cpp
	src/reader.cpp
	src/reader/file.cpp
	src/Text.cpp
	src/Utils.cpp
	)
target_sources(${PROJECT_NAME}
	PRIVATE FILE_SET private_headers
	TYPE HEADERS
	BASE_DIRS src/
	FILES
		src/reader/file.h
		src/reader/buffer.h
		src/reader/buffer.inl.h
	PUBLIC FILE_SET HEADERS
	BASE_DIRS include/
	FILES
		include/Text.h
		include/Utils.h
		include/starshatter/foundation/reader.h
		include/List.h
		include/FilesystemDataSource.h
		include/List.inl.h
		include/Dictionary.h
		include/Dictionary.inl.h
		include/OverlayDataSource.h
		include/DataSource.h
	)

add_executable(${PROJECT_NAME}_test
	test/Text.cpp
	test/reader.cpp
	test/FilesystemDataSource.cpp
	)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME} GTest::gtest_main)
generate_emulator(${PROJECT_NAME}_test)
gtest_discover_tests(${PROJECT_NAME}_test DISCOVERY_TIMEOUT 60)
