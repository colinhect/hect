Main Page {#mainpage}
=========

[TOC]

Hect Engine is a 3D game engine written in modern C++.  It strives to be flexable, clean, consistent, and powerful.  The engine is in the very early stages of development and should not be used for any serious projects.

# Requirements {#mainpage_requirements}
* [CMake 2.8.x](http://www.cmake.org)
* [Python 2.x or 3.x](https://www.python.org)
* [Doxygen](https://www.doxygen.org)

## Linux Requirements {#mainpage_linuxrequirements}
* [Clang 3.4](https://clang.llvm.org)
* [GCC 4.8](https://gcc.gnu.org)
* [Make](http://www.gnu.org/software/make)
* [SDL2](https://www.libsdl.org)
* [OpenGL](https://www.opengl.org)

## Windows Requirements {#mainpage_windowsrequirements}
* [Microsoft Visual Studio 2013 (Express or Professional)](http://www.visualstudio.com)

# Building {#mainpage_building}

Hect Engine and its tests are built using CMake.  The structure of the source tree encourages the use of CMake within the same source tree for projects using Hect Engine.

## Building on Linux {#mainpage_linuxbuilding}

The following sequence of shell commands demonstrate how to clone the Hect Engine repository and perform a clean build.

    git clone https://github.com/colinhect/hect.git
    cd hect
    cmake -H./ -BBuild -DCMAKE_BUILD_TYPE=Release
    cd Build
    make

If the build was successful, the Hect Engine binaries, tests, and samples will be in the `Build/Output` directory.