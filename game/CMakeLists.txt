add_executable(StrongholdRoyale source/main.cpp source/StrongholdRoyale.cpp include/StrongholdRoyale.h)

target_include_directories(StrongholdRoyale
	PUBLIC
	include/)

target_link_libraries(StrongholdRoyale PUBLIC
	Utils
	Core-Wrappers
	Graphics
	Input-Devices
	Camera
	Socket
	App
	Base-Game
	SR-World
)

add_custom_command(TARGET StrongholdRoyale PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/VaKon3D/assets
	${CMAKE_CURRENT_BINARY_DIR}/assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/assets
	${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_custom_target(
	Copy-Assets
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/VaKon3D/assets
	${CMAKE_CURRENT_BINARY_DIR}/assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/assets
	${CMAKE_CURRENT_BINARY_DIR}/assets
)