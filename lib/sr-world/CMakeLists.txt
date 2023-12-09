file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(
	SR-World STATIC
	${Sources}
)

target_include_directories(
	SR-World PUBLIC
	include
)

target_link_libraries(
	SR-World PUBLIC
	Utils
	glfw
	Glad
	Logger
	Delegate
	Misc
	Window
	FreeType-Wrapper
	Scene-Objects
	Base-Game
	Input-Devices
)