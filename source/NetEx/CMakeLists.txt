project(NetEx)
add_library(
	NetEx
	STATIC
	HttpClient.cpp
	HttpParam.cpp
	HttpRequest.cpp
	HttpResponse.cpp
	HttpServer.cpp
	HttpServlet.cpp
	HttpServletExec.cpp
	HttpSession.cpp
	NetAddr.cpp
	NetClient.cpp
	NetGram.cpp
	NetHost.cpp
	NetLayer.cpp
	NetLink.cpp
	NetMsg.cpp
	NetPeer.cpp
	NetServer.cpp
	NetSock.cpp
	)
target_include_directories(
	NetEx
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	)
target_link_libraries(
	NetEx
	PUBLIC FoundationEx
	)

target_link_libraries(
	NetEx PRIVATE
   	Ws2_32
  	)
