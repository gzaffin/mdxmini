if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(mdxmini)

if(MSVC OR MINGW)
	cmake_minimum_required(VERSION 3.10)
endif()

if(UNIX)
	cmake_minimum_required(VERSION 3.0.2)
endif()

if(MSVC)
	find_package(SDL2 CONFIG REQUIRED)
endif()
if(UNIX OR MINGW)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(SDL2 REQUIRED sdl2)
	set(CMAKE_C_FLAGS "-O3 -Wall -idirafter ${CMAKE_CURRENT_SOURCE_DIR}/src")
endif()

add_subdirectory(src)

file(GLOB SOURCES nlg.c sdlplay.c)

if(MSVC OR MINGW)
	include_directories(src)
endif()

add_executable(mdxplay EXCLUDE_FROM_ALL ${SOURCES})

if(MSVC)
	target_link_libraries(mdxplay PRIVATE mdxmini SDL2::SDL2 SDL2::SDL2main)
endif()

if(UNIX OR MINGW)
	target_link_libraries(mdxplay PRIVATE mdxmini m ${SDL2_LIBRARIES})
	target_include_directories(mdxplay PUBLIC ${SDL2_INCLUDE_DIRS})
	target_compile_options(mdxplay PUBLIC ${SDL2_FLAGS} ${SDL2_FLAGS_OTHERS})
	install(TARGETS mdxplay DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
