# mdxmini

Music Driver X (MDXDRV) format player using Simple DirectMedia Layer (SDL) version 2.0.x


This is actually a BouKiCHi project (https://github.com/BouKiCHi), but since I cannot find its repostory anymore, I would like to share here.


Music Driver X (MDXDRV) is a music driver developed by milk, K.MAEKAWA, Yatsube and Missy.M for SHARP X68000 home computers, it utilizes MML (Music Macro Language) to create music files capable of FM synthesis and ADPCM data wave replaying.


# How to build

The following steps build `mdxplay` on a GNU/Linux box, using SDL2 library (sdl2-config) and make.

```
$ git clone https://github.com/gzaffin/mdxmini.git
$ cd mdxmini
$ make -j 2
```

The following steps build `mdxplay` on a GNU/Linux box, using SDL2 library (pck-config) and cmake.

```
$ git clone https://github.com/gzaffin/mdxmini.git
$ cd mdxmini
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make -j 2 mdxplay
```

The following steps build `pmdplay.exe` on a MSYS2/MinGW-w64 Windows o.s. box with SDL2 and cmake.

```
$ git clone https://github.com/gzaffin/mdxmini.git
$ cd mdxmini
$ mkdir build
$ cd build
$ cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release ..
$ make -j 4 mdxplay
```

If MSYS Makefiles generator set with `-G "MSYS Makefiles"` cannot properly set `make-utility`,
such as `make`, `mingw32-make` in the following example,
then add `-DCMAKE_MAKE_PROGRAM= make-utility` (see [1])

```
$ git clone https://github.com/gzaffin/mdxmini.git
$ cd mdxmini
$ mkdir build
$ cd build
$ cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=mingw32-make ..
$ cmake --build . --target mdxplay
```

The following steps build `pmdplay.exe` on a Windows o.s. box
with Visual Studio Community Edition 2017 or 2019, vcpkg, SDL2 and cmake.

```
mkdir build
cd build
cmake-gui ..
```

On CMake GUI

Push `Configure` button

fill Optional toolset to use (argument to -T) with `host=x64` for Windows 64-bit o.s.

Push `OK` button

Push `Add entry` button

Add Cache Entry

Name: `CMAKE_TOOLCHAIN_FILE`

Type: `STRING`

Value: PATH vcpkg/scripts/buildsystems/vcpkg.cmake (see [2])

Description: `cmake entry point for vcpkg`

Push `OK` button

Double check that `SDL2_DIR` `STRING` variable is set with path to find SDL2Config.cmake (see [3]), if not the case, fix it

Push `Configure` button

Push `Generate` button

Push `Open Project` button

Open Microsoft Visual Studio Community, open generated solution and build solution

[1]
it is make-utility name e.g. `mingw32-make` with specified PATH if make is not in configured search PATH as it should be

[2]
it is absolute or relative path to vcpkg's vcpkg.cmake e.g. C:/tempGZ/vcpkg/scripts/buildsystems/vcpkg.cmake

[3]
it is absolute or relative path to vcpkg's SDL2Config.cmake e.g. C:/tempGZ/vcpkg/installed/x64-windows/share/sdl2

links to reference information pages concerning with how install and use Vcpkg
[A]
https://blogs.msdn.microsoft.com/vcblog/2016/09/19/vcpkg-a-tool-to-acquire-and-build-c-open-source-libraries-on-windows/
[B]
https://blogs.msdn.microsoft.com/vcblog/2018/04/24/announcing-a-single-c-library-manager-for-linux-macos-and-windows-vcpkg/

