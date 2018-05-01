cmake_minimum_required(VERSION 3.10)

if(MSVC)
  set(CMAKE_C_FLAGS "/Ox /W3 /wd4996")
else()
  set(CMAKE_C_FLAGS "-O3 -Wall")
endif()

add_definitions(/DUSE_NLG)

include_directories(./..)

file(GLOB SOURCES *.c)

add_library(mdxmini STATIC ${SOURCES})
