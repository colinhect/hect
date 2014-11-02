///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2014 Colin Hill
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
#include <Hect/Core/Configuration.h>
#include <Hect/Core/Error.h>
#include <Hect/Runtime/Engine.h>
using namespace hect;

#include <catch.hpp>

extern Engine* engine;

// Issue #108
#ifdef HECT_WINDOWS_BUILD

TEST_CASE("FileSystem_CreateAndRemoveDirectories")
{
    FileSystem& fileSystem = engine->fileSystem();

    Path baseDirectory = fileSystem.baseDirectory();
    fileSystem.mountArchive(baseDirectory);
    fileSystem.setWriteDirectory(baseDirectory);

    Path path("Directory");

    REQUIRE(!fileSystem.exists(path));
    fileSystem.createDirectory(path);
    REQUIRE(fileSystem.exists(path));
    fileSystem.remove(path);
    REQUIRE(!fileSystem.exists(path));
}

TEST_CASE("FileSystem_OpenNonExistingFileForWrite")
{
    FileSystem& fileSystem = engine->fileSystem();

    Path baseDirectory = fileSystem.baseDirectory();
    fileSystem.mountArchive(baseDirectory);
    fileSystem.setWriteDirectory(baseDirectory);

    Path path("File.txt");

    REQUIRE(!fileSystem.exists(path));
    {
        WriteStream stream = fileSystem.openFileForWrite(path);
    }
    REQUIRE(fileSystem.exists(path));
    fileSystem.remove(path);
    REQUIRE(!fileSystem.exists(path));
}

TEST_CASE("FileSystem_OpenExistingFileForWrite")
{
    FileSystem& fileSystem = engine->fileSystem();

    Path baseDirectory = fileSystem.baseDirectory();
    fileSystem.mountArchive(baseDirectory);
    fileSystem.setWriteDirectory(baseDirectory);

    Path path("File.txt");

    REQUIRE(!fileSystem.exists(path));
    {
        WriteStream stream = fileSystem.openFileForWrite(path);
    }
    REQUIRE(fileSystem.exists(path));
    {
        WriteStream stream = fileSystem.openFileForWrite(path);
    }
    REQUIRE(fileSystem.exists(path));
    fileSystem.remove(path);
    REQUIRE(!fileSystem.exists(path));
}

TEST_CASE("FileSystem_OpenExistingFileForRead")
{
    FileSystem& fileSystem = engine->fileSystem();

    Path baseDirectory = fileSystem.baseDirectory();
    fileSystem.mountArchive(baseDirectory);
    fileSystem.setWriteDirectory(baseDirectory);

    Path path("File.txt");

    REQUIRE(!fileSystem.exists(path));
    {
        WriteStream stream = fileSystem.openFileForWrite(path);
    }
    REQUIRE(fileSystem.exists(path));
    {
        ReadStream stream = fileSystem.openFileForRead(path);
    }

    fileSystem.remove(path);
    REQUIRE(!fileSystem.exists(path));
}

TEST_CASE("FileSystem_OpenNonExistingFileForRead")
{
    FileSystem& fileSystem = engine->fileSystem();

    Path baseDirectory = fileSystem.baseDirectory();
    fileSystem.mountArchive(baseDirectory);
    fileSystem.setWriteDirectory(baseDirectory);

    Path path("DoesNotExist.txt");

    bool errorOccurred = false;

    try
    {
        ReadStream stream = fileSystem.openFileForRead(path);
    }
    catch (Error&)
    {
        errorOccurred = true;
    }

    REQUIRE(errorOccurred);
}

#endif
