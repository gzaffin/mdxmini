if(MSVC OR MINGW)
	cmake_minimum_required(VERSION 3.12)
endif()

if(UNIX OR APPLE)
	cmake_minimum_required(VERSION 3.0.2)
endif()

if(MSVC)
	set(CMAKE_C_FLAGS "/Ox /W3 /wd4996")
endif()
if(UNIX OR MINGW OR APPLE)
	set(CMAKE_C_FLAGS "-O3 -Wall")
endif()

if(MSVC)
	find_package(Iconv REQUIRED)
	add_definitions("/DUSE_ICONV /DUSE_NLG")

	if(NOT MSVC_VERSION LESS 1400)
		# Note: CMake 3.12 and up have add_compile_options()
		add_definitions("/D_CRT_SECURE_NO_WARNINGS")
	endif()
endif()
if(UNIX)
	find_package(PkgConfig REQUIRED)
	add_definitions("-D USE_ICONV -D USE_NLG")
endif()
if(APPLE OR MINGW)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(ICONV REQUIRED iconv)
	add_definitions("-D USE_ICONV -D USE_NLG")
endif()

include_directories(./..)

file(GLOB SOURCES *.c)

add_library(mdxmini STATIC ${SOURCES})
if(MSVC)
	target_link_libraries(mdxmini PRIVATE Iconv::Iconv)
endif()
if(APPLE OR MINGW)
	target_link_libraries(mdxmini PRIVATE m ${ICONV_LIBRARIES})
endif()


