## mdxmini
Music Driver X (MDXDRV) format player using Simple DirectMedia Layer (SDL) version 2.0.x

This is actually a BouKiCHi project (https://github.com/BouKiCHi), but since I cannot find its repostory anymore, I would like to share here.

Music Driver X (MDXDRV) is a music driver developed by milk, K.MAEKAWA, Yatsube and Missy.M for SHARP X68000 home computers, it utilizes MML (Music Macro Language) to create music files capable of FM synthesis and ADPCM data wave replaying.

# How to use mdxplay player

Call `mdxplay` with no arguments for a quick list of examples about how play a .MDX module.

Folder `${HOME}/.mdxplay/` on Linux o.s. or `%USERPROFILE%\.mdxplay\` on Windows o.s. can be used to have .PDX files in a common folder.

# How to build mdxplay player

The following steps build build `mdxplay.exe` on a MSYS2/MinGW-w64 box, or `mdxplay` on a GNU/Linux box, using SDL2 library (sdl2-config) and make.

```shell/bash shell
$ git clone https://github.com/gzaffin/mdxmini.git
$ cd mdxmini
$ make mdxplay
```

The following steps build `mdxplay` on a Ubuntu/Debian/GNU/Linux box, using SDL2 library (pck-config) and cmake.

```GNU/linux bash
$ git clone https://github.com/gzaffin/mdxmini.git
$ cd mdxmini
$ mkdir build
$ cd build
$ cmake -D CMAKE_BUILD_TYPE=Release ..
$ make mdxplay
```

The following steps build `mdxplay.exe` on a MSYS2/MinGW-w64 Windows o.s. box with SDL2 and cmake.

```msys2/mingw bash
$ git clone https://github.com/gzaffin/mdxmini.git
$ cd mdxmini
$ mkdir build
$ cd build
$ cmake -G "MSYS Makefiles" -D CMAKE_BUILD_TYPE=Release ..
$ make mdxplay
```

If MSYS Makefiles generator set with `-G "MSYS Makefiles"` cannot properly set make-utility,
then add `-D CMAKE_MAKE_PROGRAM=<[PATH]/make-utility>` PATH of make-utility (see [1])

```windows command-line interface
$ git clone https://github.com/gzaffin/mdxmini.git
$ cd mdxmini
$ mkdir build
$ cd build
$ cmake -G "MSYS Makefiles" -D CMAKE_MAKE_PROGRAM=mingw32-make ..
$ cmake --build . --config Release --target mdxplay
```

The following steps build `mdxplay.exe` on a Windows o.s. box with MSVC, vcpkg, SDL2 installed with vcpkg.

```windows command-line interface
C:\>clone https://github.com/gzaffin/mdxmini.git
C:\>cd mdxmini
C:\mdxmini>mkdir build
C:\mdxmini>cd build
C:\mdxmini\build>cmake -G "Visual Studio 18 2026" -A x64 -T host=x64 -D CMAKE_TOOLCHAIN_FILE=C:/Users/gzaff/vcpkg/scripts/buildsystems/vcpkg.cmake ..
C:\mdxmini\build>cmake --build . --config Release --target mdxplay
```

[1]
it is make-utility name e.g. `mingw32-make` with specified PATH if make is not within search PATH as it should be

# Links
Reference information pages about how to install and how to use Vcpkg

[GitHub Microsoft vcpkg](https://github.com/Microsoft/vcpkg)

[vcpkg: A C++ package manager for Windows, Linux and MacOS](https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=vs-2019)

[Eric Mittelette's blog](https://devblogs.microsoft.com/cppblog/vcpkg-a-tool-to-acquire-and-build-c-open-source-libraries-on-windows/)

# License

This code is available open source under the terms of the [GNU General Public License version 2](https://opensource.org/licenses/GPL-2.0).
