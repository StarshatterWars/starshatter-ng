project(Magic2)
set(CMAKE_MFC_FLAG 1)
add_executable(
	Magic2 WIN32
	AlphaInverse.cpp
	AlphaPalette.cpp
	Command.cpp
	Editor.cpp
	Grid.cpp
	GridProps.cpp
	Magic.cpp
	Magic.rc
	MagicDoc.cpp
	MagicView.cpp
	MainFrm.cpp
	MaterialDialog.cpp
	ModelView.cpp
	Selection.cpp
	Selector.cpp
	StdAfx.cpp
	SurfacePropertiesDialog.cpp
	TextureMapDialog.cpp
	Thumbnail.cpp
	UVMapView.cpp
	)
target_include_directories(
	Magic2
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	)
target_include_directories(
	Magic2 SYSTEM
	PUBLIC ${WINDOWSSDK_PATH}/um
	PUBLIC ${WINDOWSSDK_PATH}/shared
	PUBLIC $ENV{DXSDK_DIR}/Include
	)
target_precompile_headers(
	Magic2
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h
	)
target_link_libraries(Magic2 PUBLIC FoundationEx MagicEx StarsEx)
target_compile_definitions(Magic2 PRIVATE _AFXDLL)
