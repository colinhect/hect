Hect
====

A game engine in very early development.

Build status: [![Build status](https://travis-ci.org/colinhect/hect.png)](https://travis-ci.org/colinhect/hect)

## Requirements

### Windows
* [CMake 2.8.x](http://www.cmake.org)
* [Python 2.x or 3.x](https://www.python.org)
* [Microsoft Visual Studio 2013 (Express or Professional)](http://www.visualstudio.com)
* [Microsoft DirectX SDK](http://www.microsoft.com/en-us/download/details.aspx?id=6812)

### Linux
* [CMake 2.8.x](http://www.cmake.org)
* [Python 2.x or 3.x](https://www.python.org)
* [GCC 4.9](https://gcc.gnu.org)
* [Make](http://www.gnu.org/software/make)
* OpenGL/GLU headers and libraries

## Building

### Platform Independent
* Invoke `python build.py debug` or `python build.py release`
* Resulting binaries and assets can be found in Build/Output

### Visual Studio
* Invoke `python prebuild.py` to generate the Visual Studio solution files using CMake
* Open Build/Hect.sln using Visual Studio 2013
* Build the solution
* Resulting binaries and assets can be found in Build/Output
