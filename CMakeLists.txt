if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(mdxmini)

if(MSVC OR MINGW)
	cmake_minimum_required(VERSION 3.10)
endif()

if(UNIX OR APPLE)
	cmake_minimum_required(VERSION 3.0.2)
endif()

if(MSVC)
	find_package(SDL2 CONFIG REQUIRED)
	find_package(Iconv REQUIRED)
	add_definitions("-D USE_ICONV")
endif()
if(UNIX OR MINGW)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(SDL2 REQUIRED sdl2)
        pkg_check_modules(ICONV REQUIRED iconv)
        add_definitions("-D USE_ICONV")
endif()
if(APPLE)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(SDL2 REQUIRED sdl2)
endif()

add_subdirectory(src)

file(GLOB SOURCES nlg.c sdlplay.c)

if(MSVC OR UNIX OR MINGW OR APPLE)
	include_directories(src)
endif()

add_executable(mdxplay EXCLUDE_FROM_ALL ${SOURCES})

if(MSVC)
	target_link_libraries(mdxplay PRIVATE mdxmini SDL2::SDL2 SDL2::SDL2main Iconv::Iconv)
endif()

if(UNIX OR MINGW OR APPLE)
    target_link_libraries(mdxplay PRIVATE mdxmini m ${SDL2_LIBRARIES} ${ICONV_LIBRARIES})
    target_include_directories(mdxplay PUBLIC ${SDL2_INCLUDE_DIRS} ${ICONV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_compile_options(mdxplay PUBLIC ${SDL2_FLAGS} ${SDL2_FLAGS_OTHER} ${ICONV_CFLAGS} ${ICONV_FLAGS_OTHER})
        target_link_options(mdxplay PUBLIC ${SDL2_LDFLAGS} ${SDL2_LDFLAGS_OTHER} ${ICONV_LDFLAGS} ${ICONV_LDFLAGS_OTHER})
	install(TARGETS mdxplay DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
