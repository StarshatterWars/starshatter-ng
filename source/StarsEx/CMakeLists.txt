project(StarsEx)

add_library(
	StarsEx STATIC
	ActiveWindow.cpp
	Asteroid.cpp
	AudDlg.cpp
	AudioConfig.cpp
	AwardDlg.cpp
	AwardShowDlg.cpp
	Bitmap.cpp
	Bmp.cpp
	Bolt.cpp
	Button.cpp
	Callsign.cpp
	Camera.cpp
	CameraDirector.cpp
	CameraView.cpp
	Campaign.cpp
	CampaignMissionFighter.cpp
	CampaignMissionRequest.cpp
	CampaignMissionStarship.cpp
	CampaignPlanAssignment.cpp
	CampaignPlanEvent.cpp
	CampaignPlanMission.cpp
	CampaignPlanMovement.cpp
	CampaignPlanStrategic.cpp
	CampaignSaveGame.cpp
	CampaignSituationReport.cpp
	CarrierAI.cpp
	Clock.cpp
	CmdDlg.cpp
	CmdForceDlg.cpp
	CmdIntelDlg.cpp
	CmdMissionsDlg.cpp
	CmdMsgDlg.cpp
	CmdOrdersDlg.cpp
	CmdTheaterDlg.cpp
	CmdTitleDlg.cpp
	CmpCompleteDlg.cpp
	CmpFileDlg.cpp
	CmpLoadDlg.cpp
	CmpnScreen.cpp
	CmpSceneDlg.cpp
	CmpSelectDlg.cpp
	Color.cpp
	ColorPalette.cpp
	CombatAction.cpp
	Combatant.cpp
	CombatAssignment.cpp
	CombatEvent.cpp
	CombatGroup.cpp
	CombatRoster.cpp
	CombatUnit.cpp
	CombatZone.cpp
	ComboBox.cpp
	ComboList.cpp
	Component.cpp
	Computer.cpp
	ConfirmDlg.cpp
	Contact.cpp
	ContentBundle.cpp
	CtlDlg.cpp
	D3DXImage.cpp
	DataLoader.cpp
	DebriefDlg.cpp
	Debris.cpp
	DetailSet.cpp
	DisplayView.cpp
	Drive.cpp
	DriveSprite.cpp
	Drone.cpp
	DropShipAI.cpp
	DumpMissions.cpp
	EditBox.cpp
	Element.cpp
	Encrypt.cpp
	EngDlg.cpp
	ExceptionHandler.cpp
	ExitDlg.cpp
	Explosion.cpp
	FadeView.cpp
	Farcaster.cpp
	FighterAI.cpp
	FighterTacticalAI.cpp
	FirstTimeDlg.cpp
	FlightComp.cpp
	FlightDeck.cpp
	FlightPlanner.cpp
	FltDlg.cpp
	Font.cpp
	FontMgr.cpp
	FormatUtil.cpp
	FormDef.cpp
	FormWindow.cpp
	Galaxy.cpp
	Game.cpp
	GameScreen.cpp
	GameWinDX9.cpp
	Geometry.cpp
	Graphic.cpp
	Grid.cpp
	GroundAI.cpp
	Hangar.cpp
	HardPoint.cpp
	Hoop.cpp
	HUDSounds.cpp
	HUDView.cpp
	ImageBox.cpp
	ImgView.cpp
	Instruction.cpp
	Intel.cpp
	JoyDlg.cpp
	Joystick.cpp
	Keyboard.cpp
	KeyDlg.cpp
	KeyMap.cpp
	LandingGear.cpp
	Layout.cpp
	Light.cpp
	ListBox.cpp
	LoadDlg.cpp
	LoadScreen.cpp
	Locale_ss.cpp
	MapView.cpp
	MCIWave.cpp
	Menu.cpp
	MenuDlg.cpp
	MenuScreen.cpp
	MenuView.cpp
	Mfd.cpp
	Mission.cpp
	MissionEvent.cpp
	MissionTemplate.cpp
	ModConfig.cpp
	ModDlg.cpp
	ModInfo.cpp
	ModInfoDlg.cpp
	MouseController.cpp
	Mouse.cpp
	MsnDlg.cpp
	MsnEditDlg.cpp
	MsnEditNavDlg.cpp
	MsnElemDlg.cpp
	MsnEventDlg.cpp
	MsnNavDlg.cpp
	MsnObjDlg.cpp
	MsnPkgDlg.cpp
	MsnSelectDlg.cpp
	MsnWepDlg.cpp
	MultiController.cpp
	MusicDirector.cpp
	MusicTrack.cpp
	NavAI.cpp
	NavDlg.cpp
	NavLight.cpp
	NavSystem.cpp
	NetAddrDlg.cpp
	NetAuth.cpp
	NetBrokerClient.cpp
	NetChat.cpp
	NetClientConfig.cpp
	NetClientDlg.cpp
	NetData.cpp
	NetGameClient.cpp
	NetGame.cpp
	NetGameServer.cpp
	NetLobbyClient.cpp
	NetLobby.cpp
	NetLobbyDlg.cpp
	NetLobbyServer.cpp
	NetPacket.cpp
	NetPassDlg.cpp
	NetPlayer.cpp
	NetServerConfig.cpp
	NetServerDlg.cpp
	NetUnitDlg.cpp
	NetUser.cpp
	NetUtil.cpp
	NPClientWraps.cpp
	OptDlg.cpp
	Panic.cpp
	ParseUtil.cpp
	Particles.cpp
	Pcx.cpp
	Physical.cpp
	PlanScreen.cpp
	Player.cpp
	PlayerDlg.cpp
	PngImage.cpp
	Polygon.cpp
	Power.cpp
	Projector.cpp
	QuantumDrive.cpp
	QuantumFlash.cpp
	QuantumView.cpp
	QuitView.cpp
	RadioHandler.cpp
	RadioMessage.cpp
	RadioTraffic.cpp
	RadioView.cpp
	RadioVox.cpp
	Random.cpp
	RichTextBox.cpp
	RLoc.cpp
	Scene.cpp
	Screen.cpp
	ScrollWindow.cpp
	SeekerAI.cpp
	Sensor.cpp
	Shadow.cpp
	Shield.cpp
	ShieldRep.cpp
	ShipAI.cpp
	Ship.cpp
	ShipCtrl.cpp
	ShipDesign.cpp
	ShipKiller.cpp
	ShipSolid.cpp
	Shot.cpp
	Sim.cpp
	SimEvent.cpp
	SimObject.cpp
	Skin.cpp
	Sky.cpp
	Slider.cpp
	Solid.cpp
	SoundCard.cpp
	Sound.cpp
	SoundD3D.cpp
	Sprite.cpp
	Stardate.cpp
	Starshatter.cpp
	StarshipAI.cpp
	StarshipTacticalAI.cpp
	StarSystem.cpp
	SteerAI.cpp
	System.cpp
	SystemDesign.cpp
	TacRefDlg.cpp
	TacticalAI.cpp
	TacticalView.cpp
	TerrainApron.cpp
	TerrainClouds.cpp
	Terrain.cpp
	TerrainHaze.cpp
	TerrainPatch.cpp
	TerrainRegion.cpp
	TexCubeDX9.cpp
	TexDX9.cpp
	Thruster.cpp
	TrackIR.cpp
	Trail.cpp
	UIEventDispatch.cpp
	VidDlg.cpp
	Video.cpp
	VideoDX9.cpp
	VideoDX9Enum.cpp
	VideoDX9VertexBuffer.cpp
	VideoFactory.cpp
	VideoSettings.cpp
	Water.cpp
	Weapon.cpp
	WeaponDesign.cpp
	WeaponGroup.cpp
	Weather.cpp
	WepView.cpp
	Window.cpp
	WndProc.cpp
	)
target_include_directories(
	StarsEx
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	)
target_link_libraries(
	StarsEx
	PUBLIC ArchiveEx
	PUBLIC DefinitionEx
	PUBLIC EngineEx
	PUBLIC FoundationEx
	PUBLIC NetEx
	PUBLIC TypesEx
	PUBLIC Vorbis::vorbis
	PUBLIC Vorbis::vorbisfile
	PUBLIC Ogg::ogg
	PUBLIC Png::png
	PUBLIC Opcode
	PRIVATE Sha1
	PRIVATE InfoEx
	)
target_compile_definitions(
	StarsEx
	PRIVATE _ALLOW_KEYWORD_MACROS
	)
add_version_file(VersionInfo.cpp VersionInfo.cpp.conf)
target_sources(
	StarsEx
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/VersionInfo.cpp
	)

target_link_libraries(StarsEx PRIVATE
    dsound
    dinput8
    dxguid
    winmm
    d3d9
    d3dx9
  )
if(MSVC)
	target_include_directories(
		StarsEx
		PUBLIC $ENV{DXSDK_DIR}/Include
		PUBLIC ${WINDOWSSDK_PATH}/shared
		)
	target_link_libraries(
		StarsEx
		PUBLIC $ENV{DXSDK_DIR}/Lib/x86/dinput8.lib
		PUBLIC $ENV{DXSDK_DIR}/Lib/x86/dsound.lib
		PUBLIC $ENV{DXSDK_DIR}/Lib/x86/d3d9.lib
		PUBLIC $ENV{DXSDK_DIR}/Lib/x86/d3dx9.lib
		PUBLIC $ENV{DXSDK_DIR}/Lib/x86/dxguid.lib
		)
else()
	target_link_libraries(
		StarsEx
		PUBLIC -l:libws2_32.a
		PUBLIC -l:libvfw32.a
		PUBLIC -l:libwsock32.a
		PUBLIC -l:libwinmm.a
		PUBLIC -l:libversion.a
		PUBLIC -l:libdinput8.a
		PUBLIC -l:libdsound.a
		PUBLIC -l:libd3d9.a
		PUBLIC -l:libd3dx9.a
		PUBLIC -l:libdxguid.a
		)
endif()
add_executable(
	${PROJECT_NAME}_test
	test/AudioConfig.cpp
	test/Color.cpp
	test/ShipStats.cpp
	test/Stardate.cpp
	)
generate_emulator(${PROJECT_NAME}_test LIBS Opcode ArchiveEx)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME} GTest::gtest_main)
gtest_discover_tests(${PROJECT_NAME}_test DISCOVERY_TIMEOUT 60)
