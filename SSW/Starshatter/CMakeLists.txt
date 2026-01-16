project(Starshatter)

add_executable(
  Starshatter WIN32
  Main.cpp
)

target_include_directories(
  Starshatter
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
  Starshatter
  PRIVATE StarsEx InfoEx
)

add_version_file(Starshatter.rc Starshatter.rc.conf)

target_sources(
  Starshatter
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/Starshatter.rc
)

install(
  TARGETS Starshatter
  RUNTIME
  COMPONENT Runtime
  DESTINATION ${CMAKE_INSTALL_PREFIX}
)

