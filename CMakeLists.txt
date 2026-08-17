#### mdxmini ####
if(MSVC OR MINGW)
	cmake_minimum_required(VERSION 3.12)
endif()

if(UNIX)
	cmake_minimum_required(VERSION 3.0.2)
endif()

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif()

# Define option here
option(USE_LIBICONV "Iconv external dependency" FALSE)

project(mdxmini LANGUAGES C)

if(MSVC)
	find_package(SDL2 CONFIG REQUIRED)
	if(USE_LIBICONV)
		# Iconv library is used for converting from SHIFT-JIS to UTF-8
		find_package(Iconv REQUIRED)
		add_definitions("/DUSE_ICONV")
		message(STATUS "Iconv library is used for converting from SHIFT-JIS to UTF-8.")
	else()
		# Hand-made conversion from SHIFT-JIS to UTF-8
		message(STATUS "Hand-made conversion from SHIFT-JIS to UTF-8.")
	endif()

	if(NOT MSVC_VERSION LESS 1400)
		# Note: CMake 3.12 and up have add_compile_options()
		add_definitions("/D_CRT_SECURE_NO_WARNINGS")
	endif()

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wall")
endif()
if(UNIX AND NOT APPLE)
	find_package(PkgConfig REQUIRED)

	pkg_check_modules(SDL2 REQUIRED sdl2)

	if(USE_LIBICONV)
		# Iconv library is used for converting from SHIFT-JIS to UTF-8
		add_definitions("-D USE_ICONV")
		message(STATUS "Iconv library is used for converting from SHIFT-JIS to UTF-8.")
	else()
		# Hand-made conversion from SHIFT-JIS to UTF-8
		message(STATUS "Hand-made conversion from SHIFT-JIS to UTF-8.")
	endif()
endif()
if(MINGW OR APPLE)
	find_package(PkgConfig REQUIRED)

	pkg_check_modules(SDL2 REQUIRED sdl2)

	if(USE_LIBICONV)
		# Iconv library is used for converting from SHIFT-JIS to UTF-8
		pkg_check_modules(ICONV REQUIRED iconv)
		add_definitions("-D USE_ICONV")
		message(STATUS "Iconv library is used for converting from SHIFT-JIS to UTF-8.")
	else()
		# Hand-made conversion from SHIFT-JIS to UTF-8
		message(STATUS "Hand-made conversion from SHIFT-JIS to UTF-8.")
	endif()
endif()

if(MSVC OR UNIX OR MINGW)
	include_directories(src)
endif()

add_subdirectory(src)

add_executable(mdxplay nlg.c sdlplay.c)

if(MSVC)
	if(USE_LIBICONV)
		# Iconv library is used for converting from SHIFT-JIS to UTF-8
		target_link_libraries(mdxplay PRIVATE mdxmini SDL2::SDL2 SDL2::SDL2main Iconv::Iconv)
		message(STATUS "Iconv library is used for converting from SHIFT-JIS to UTF-8.")
	else()
		# Hand-made conversion from SHIFT-JIS to UTF-8
		target_link_libraries(mdxplay PRIVATE mdxmini SDL2::SDL2 SDL2::SDL2main)
		message(STATUS "Hand-made conversion from SHIFT-JIS to UTF-8.")
	endif()
endif()
if(UNIX AND NOT APPLE)
	target_link_libraries(mdxplay PRIVATE mdxmini m ${SDL2_LIBRARIES})
	target_include_directories(mdxplay PUBLIC ${SDL2_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src)
	target_compile_options(mdxplay PUBLIC ${SDL2_FLAGS} ${SDL2_FLAGS_OTHER})
	target_link_options(mdxplay PUBLIC ${SDL2_LDFLAGS} ${SDL2_LDFLAGS_OTHER})
	install(TARGETS mdxplay DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
if(MINGW OR APPLE)
	if(USE_LIBICONV)
		# Iconv library is used for converting from SHIFT-JIS to UTF-8
		target_link_libraries(mdxplay PRIVATE mdxmini m ${SDL2_LIBRARIES} ${ICONV_LIBRARIES})
		target_include_directories(mdxplay PUBLIC ${SDL2_INCLUDE_DIRS} ${ICONV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src)
		target_compile_options(mdxplay PUBLIC ${SDL2_FLAGS} ${SDL2_FLAGS_OTHER} ${ICONV_CFLAGS} ${ICONV_FLAGS_OTHER})
		target_link_options(mdxplay PUBLIC ${SDL2_LDFLAGS} ${SDL2_LDFLAGS_OTHER} ${ICONV_LDFLAGS} ${ICONV_LDFLAGS_OTHER})
		message(STATUS "Iconv library is used for converting from SHIFT-JIS to UTF-8.")
	else()
		# Hand-made conversion from SHIFT-JIS to UTF-8
		target_link_libraries(mdxplay PRIVATE mdxmini m ${SDL2_LIBRARIES})
		target_include_directories(mdxplay PUBLIC ${SDL2_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src)
		target_compile_options(mdxplay PUBLIC ${SDL2_FLAGS} ${SDL2_FLAGS_OTHER} ${ICONV_FLAGS_OTHER})
		target_link_options(mdxplay PUBLIC ${SDL2_LDFLAGS} ${SDL2_LDFLAGS_OTHER} ${ICONV_LDFLAGS_OTHER})
		message(STATUS "Hand-made conversion from SHIFT-JIS to UTF-8.")
	endif()
	install(TARGETS mdxplay DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
