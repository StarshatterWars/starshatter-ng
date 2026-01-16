project(Starserver)
add_executable(
	Starserver
	Main.cpp
	NetAdminChat.cpp
	NetAdminServer.cpp
	NetFileServlet.cpp
	StarServer.cpp
	)
target_include_directories(
	Starserver
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
	)
target_link_libraries(
	Starserver
	PRIVATE StarsEx InfoEx
	)
install(
	TARGETS Starserver
	RUNTIME
	COMPONENT Runtime
	DESTINATION ${CMAKE_INSTALL_PREFIX}
	)
