project(DefinitionEx)

add_library(
	${PROJECT_NAME} STATIC
	src/Parser.cpp
	src/Term.cpp
	src/Token.cpp
	)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC FoundationEx)

add_executable(${PROJECT_NAME}_test test/parse_utils_like.cpp)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME} GTest::gtest_main)
generate_emulator(${PROJECT_NAME}_test)
gtest_discover_tests(${PROJECT_NAME}_test DISCOVERY_TIMEOUT 60)
