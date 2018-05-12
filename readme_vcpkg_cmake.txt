for vcpkg and cmake and microsoft visual studio 2017

cmake-gui ..
Configure
fill Optional toolset to use (argument to -T) with
host=x64
OK
Add entry
Add Cache Entry
Name:
CMAKE_TOOLCHAIN_FILE
Type:
STRING
Value:
<PATH>vcpkg/scripts/buildsystems/vcpkg.cmake (see [1])
Description:
cmake entry point for vcpkg
OK
Configure
Generate
Open Project

Name:
SDL2_DIR
Type:
STRING
Value:
<PATH>vcpkg/packages/sdl2_x64-windows/share/sdl2
e.g.
C:/tempGZ/vcpkg/packages/sdl2_x64-windows/share/sdl2
Description:
path to SDL2Config.cmake in vcpkg

[1]
it is absolute or relative path to vcpkg's vcpkg.cmake e.g. C:/tempGZ/vcpkg/scripts/buildsystems/vcpkg.cmake

