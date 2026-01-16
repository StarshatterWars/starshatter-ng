cmake_minimum_required(VERSION 3.20)
project(Sha1)
add_library(${PROJECT_NAME} STATIC Sha1.cpp)
target_include_directories(
	${PROJECT_NAME}
	PUBLIC include)
