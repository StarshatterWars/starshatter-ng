project(ArchiveEx)

add_library(${PROJECT_NAME} SHARED Archive.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE Zlib::zlibstatic)

# MSVC: ensure an import library (.lib) is generated (and symbols are exported)
if(MSVC)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
  )
endif()

# Install runtime + import library (Windows .dll + .lib)
install(TARGETS ${PROJECT_NAME}
  RUNTIME COMPONENT Runtime DESTINATION ${CMAKE_INSTALL_PREFIX}
  ARCHIVE COMPONENT Runtime DESTINATION ${CMAKE_INSTALL_PREFIX}
  LIBRARY COMPONENT Runtime DESTINATION ${CMAKE_INSTALL_PREFIX}
)

add_executable(dat dat.cpp)
target_link_libraries(dat PRIVATE ${PROJECT_NAME})

if(WIN32)
  # MSVC uses "shlwapi" (no -l prefix)
  target_link_libraries(dat PRIVATE shlwapi)
endif()

generate_emulator(dat LIBS ${PROJECT_NAME})
install(TARGETS dat RUNTIME COMPONENT Tools DESTINATION ${CMAKE_INSTALL_PREFIX})
