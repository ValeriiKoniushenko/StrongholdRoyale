add_executable(StrongholdRoyale source/main.cpp source/StrongholdRoyale.cpp include/StrongholdRoyale.h)

target_include_directories(StrongholdRoyale
	PUBLIC
	include/)

target_link_libraries(StrongholdRoyale PUBLIC
	Utils
	)
