cmake_minimum_required(VERSION 3.0...3.5)

project(Terraria-2.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(DEPENDENCIES_DIR_NAME dependencies)
set(DEPENDENCIES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${DEPENDENCIES_DIR_NAME})

if (DEFINED CMAKE_BUILD_TYPE)
	if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		message(STATUS "Project build type: DEBUG")
		add_compile_definitions(__DEBUG)
		add_compile_definitions(OPENGL_DEBUG)
	endif ()
endif ()

add_subdirectory(VaKon2D)
add_subdirectory(game)

add_subdirectory(lib/camera)
add_subdirectory(lib/entity)
add_subdirectory(lib/terraria-world)
add_subdirectory(lib/terraria-game)
add_subdirectory(lib/map)
add_subdirectory(lib/texture-manager)

add_subdirectory(dependencies/simplex-noise)