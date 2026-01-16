project(TypesEx)
add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
  target_include_directories(${PROJECT_NAME} INTERFACE
    "$ENV{DXSDK_DIR}/Include"
   # "${WINDOWSSDK_PATH}/um"
   # "${WINDOWSSDK_PATH}/shared"
  )
endif()