for MSYS2/minGW(mingw64) and cmake and pkg-config and Microsoft Windows 7 o.s.

    install required packages with pacman

$ pacman -S mingw-w64-x86_64-pkg-config mingw-w64-x86_64-SDL2 mingw-w64-x86_64-cmake

    clone and compile NOTE cmake does not work!!

$ git clone https://github.com/gzaffin/mdxmini.git
$ cd mdxmini
$ mkdir build
$ cd build
$ /c/Users/user/.espressif/tools/cmake/3.16.4/bin/cmake -DCMAKE_MAKE_PROGRAM=mingw32-make.exe -G "MinGW Makefiles" ..
$ /c/Users/user/.espressif/tools/cmake/3.16.4/bin/cmake --build . --config Release --target mdxplay
