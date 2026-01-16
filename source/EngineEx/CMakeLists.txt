project(EngineEx)

add_library(${PROJECT_NAME} STATIC
	src/ArchiveDataSource.cpp
	src/ColorFormat.cpp
	src/Resource.cpp
	)
target_sources(${PROJECT_NAME}
	PUBLIC FILE_SET HEADERS
	BASE_DIRS include/
	FILES
		include/ArchiveDataSource.h
		include/starshatter/ColorFormat.h
		include/starshatter/Resource.h
	)
target_link_libraries(${PROJECT_NAME}
	PUBLIC ArchiveEx
	PUBLIC FoundationEx
	)

add_executable(${PROJECT_NAME}_test
        test/ColorFormat.cpp
        )
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME} GTest::gtest_main)
generate_emulator(${PROJECT_NAME}_test)
gtest_discover_tests(${PROJECT_NAME}_test DISCOVERY_TIMEOUT 60)
