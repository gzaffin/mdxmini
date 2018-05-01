cmake_minimum_required(VERSION 3.10)

if(MSVC)
  set(CMAKE_C_FLAGS "/Ox /W3 /wd4996")
else()
  set(CMAKE_C_FLAGS "-O3 -Wall")
endif()

add_subdirectory(src)

file(GLOB SOURCES nlg.c sdlplay.c)

include_directories(src)

add_executable(mdxplay EXCLUDE_FROM_ALL ${SOURCES})

target_link_libraries(mdxplay mdxmini)

