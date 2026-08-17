#### mdxmini src ####
if(MSVC OR MINGW)
	cmake_minimum_required(VERSION 3.12)
endif()

if(UNIX)
	cmake_minimum_required(VERSION 3.0.2)
endif()

if(MSVC)
	set(CMAKE_C_FLAGS "/Ox /W3 /wd4996")
endif()
if(UNIX OR MINGW)
	set(CMAKE_C_FLAGS "-O3 -Wall")
endif()

if(MSVC)
	if(USE_LIBICONV)
		# Iconv library is used for converting from SHIFT-JIS to UTF-8
		find_package(Iconv REQUIRED)
		add_definitions("/DUSE_ICONV /DUSE_NLG")
		message(STATUS "Iconv library is used for converting from SHIFT-JIS to UTF-8.")
	else()
		# Hand-made conversion from SHIFT-JIS to UTF-8
		add_definitions("/DUSE_NLG")
		message(STATUS "Hand-made conversion from SHIFT-JIS to UTF-8.")
	endif()

	if(NOT MSVC_VERSION LESS 1400)
		# Note: CMake 3.12 and up have add_compile_options()
		add_definitions("/D_CRT_SECURE_NO_WARNINGS")
	endif()
endif()
if(UNIX AND NOT APPLE)
	if(USE_LIBICONV)
		# Iconv library is used for converting from SHIFT-JIS to UTF-8
		find_package(PkgConfig REQUIRED)
		add_definitions("-D USE_ICONV -D USE_NLG")
		message(STATUS "Iconv library is used for converting from SHIFT-JIS to UTF-8.")
	else()
		# Hand-made conversion from SHIFT-JIS to UTF-8
		add_definitions("-D USE_NLG")
		message(STATUS "Hand-made conversion from SHIFT-JIS to UTF-8.")
	endif()
endif()
if(MINGW OR APPLE)
	if(USE_LIBICONV)
		# Iconv library is used for converting from SHIFT-JIS to UTF-8
		find_package(PkgConfig REQUIRED)
		find_package(Iconv REQUIRED)
		add_definitions("-D USE_ICONV -D USE_NLG")
		message(STATUS "Iconv library is used for converting from SHIFT-JIS to UTF-8.")
	else()
		# Hand-made conversion from SHIFT-JIS to UTF-8
		add_definitions("-D USE_NLG")
		message(STATUS "Hand-made conversion from SHIFT-JIS to UTF-8.")
	endif()
endif()

include_directories(./..)

file(GLOB SOURCES *.c)

add_library(mdxmini STATIC ${SOURCES})
if(MSVC)
	if(USE_LIBICONV)
		# Iconv library is used for converting from SHIFT-JIS to UTF-8
		target_link_libraries(mdxmini PRIVATE Iconv::Iconv)
		message(STATUS "Iconv library is used for converting from SHIFT-JIS to UTF-8.")
	else()
		# Hand-made conversion from SHIFT-JIS to UTF-8
		message(STATUS "Hand-made conversion from SHIFT-JIS to UTF-8.")
	endif()
endif()
if(UNIX AND NOT APPLE)
	target_link_libraries(mdxmini PRIVATE m)
endif()
if(MINGW OR APPLE)
	if(USE_LIBICONV)
		# Iconv library is used for converting from SHIFT-JIS to UTF-8
		target_link_libraries(mdxmini PRIVATE m ${ICONV_LIBRARIES})
		message(STATUS "Iconv library is used for converting from SHIFT-JIS to UTF-8.")
	else()
		# Hand-made conversion from SHIFT-JIS to UTF-8
		target_link_libraries(mdxmini PRIVATE m)
		message(STATUS "Hand-made conversion from SHIFT-JIS to UTF-8.")
	endif()
endif()
