project(MagicEx)
add_library(
	${PROJECT_NAME} STATIC
	src/MagicLoad.cpp
	src/ModelFile3DS.cpp
	src/ModelFileMAG.cpp
	src/ModelFileOBJ.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include/)
target_link_libraries(${PROJECT_NAME} PUBLIC StarsEx PRIVATE l3ds)


add_executable(${PROJECT_NAME}_3ds2mag src/3ds2mag.cpp)
target_link_libraries(${PROJECT_NAME}_3ds2mag PRIVATE ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}_3ds2mag PROPERTIES OUTPUT_NAME 3ds2mag)


add_executable(${PROJECT_NAME}_mag2obj src/mag2obj.cpp)
target_link_libraries(${PROJECT_NAME}_mag2obj PRIVATE ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}_mag2obj PROPERTIES OUTPUT_NAME mag2obj)


add_executable(${PROJECT_NAME}_obj2mag src/obj2mag.cpp)
target_link_libraries(${PROJECT_NAME}_obj2mag PRIVATE ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}_obj2mag PROPERTIES OUTPUT_NAME obj2mag)


install(
	TARGETS ${PROJECT_NAME}_3ds2mag ${PROJECT_NAME}_mag2obj ${PROJECT_NAME}_obj2mag
	RUNTIME COMPONENT Tools DESTINATION ${CMAKE_INSTALL_PREFIX})
