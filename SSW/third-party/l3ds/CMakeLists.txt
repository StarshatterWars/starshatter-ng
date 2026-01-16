project(l3ds)
add_library(${PROJECT_NAME} SHARED l3ds.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)
install(TARGETS ${PROJECT_NAME} RUNTIME COMPONENT Runtime DESTINATION ${CMAKE_INSTALL_PREFIX})
