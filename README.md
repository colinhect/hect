Hect Engine
===========

[Website](http://colinhect.github.io/hect/)

Build status: [![Build status](https://travis-ci.org/colinhect/hect.png)](https://travis-ci.org/colinhect/hect)

Hect Engine is a 3D game engine written in modern C++.  It strives to be flexible, clean, consistent, and powerful.  The engine is in the very early stages of development and should not be used for any serious projects.

# Requirements
* [CMake 3.2.x](http://www.cmake.org)
* [Python 2.x or 3.x](https://www.python.org)
* [Doxygen](https://www.doxygen.org)

## Linux Requirements
* [GCC 5](https://gcc.gnu.org)
* [Make](http://www.gnu.org/software/make)
* [SDL 2](https://www.libsdl.org)
* [OpenGL](https://www.opengl.org)

## Windows Requirements {#mainpage_windowsrequirements}
* [Microsoft Visual Studio 2015 (Community or Professional)](http://www.visualstudio.com)
* [Visual Leak Detector (Optional)](https://vld.codeplex.com/)

# Building

Hect Engine and its tests are built using CMake.  The structure of the source tree encourages the use of CMake within the same source tree for projects using Hect Engine.

## Building on Linux

The following sequence of shell commands demonstrate how to clone the Hect Engine repository and perform a clean build.

    git clone https://github.com/colinhect/hect.git
    cd hect
    cmake -H./ -BBuild -DCMAKE_BUILD_TYPE=Release
    cd Build
    make

If the build was successful, the Hect Engine binaries, tests, and samples will be in the `Build/Output` directory.
