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

TEST_CASE("Create and remove directories", "[FileSystem]")
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

TEST_CASE("Open non-existing file for write", "[FileSystem]")
{
    FileSystem& fileSystem = engine->fileSystem();

    Path baseDirectory = fileSystem.baseDirectory();
    fileSystem.mountArchive(baseDirectory);
    fileSystem.setWriteDirectory(baseDirectory);

    Path path("File.txt");

    REQUIRE(!fileSystem.exists(path));
    {
        auto stream = fileSystem.openFileForWrite(path);
    }
    REQUIRE(fileSystem.exists(path));
    fileSystem.remove(path);
    REQUIRE(!fileSystem.exists(path));
}

TEST_CASE("Open existing file for write", "[FileSystem]")
{
    FileSystem& fileSystem = engine->fileSystem();

    Path baseDirectory = fileSystem.baseDirectory();
    fileSystem.mountArchive(baseDirectory);
    fileSystem.setWriteDirectory(baseDirectory);

    Path path("File.txt");

    REQUIRE(!fileSystem.exists(path));
    {
        auto stream = fileSystem.openFileForWrite(path);
    }
    REQUIRE(fileSystem.exists(path));
    {
        auto stream = fileSystem.openFileForWrite(path);
    }
    REQUIRE(fileSystem.exists(path));
    fileSystem.remove(path);
    REQUIRE(!fileSystem.exists(path));
}

TEST_CASE("Open existing file for read", "[FileSystem]")
{
    FileSystem& fileSystem = engine->fileSystem();

    Path baseDirectory = fileSystem.baseDirectory();
    fileSystem.mountArchive(baseDirectory);
    fileSystem.setWriteDirectory(baseDirectory);

    Path path("File.txt");

    REQUIRE(!fileSystem.exists(path));
    {
        auto stream = fileSystem.openFileForWrite(path);
    }
    REQUIRE(fileSystem.exists(path));
    {
        auto stream = fileSystem.openFileForRead(path);
    }

    fileSystem.remove(path);
    REQUIRE(!fileSystem.exists(path));
}

TEST_CASE("Open non-existing file for read", "[FileSystem]")
{
    FileSystem& fileSystem = engine->fileSystem();

    Path baseDirectory = fileSystem.baseDirectory();
    fileSystem.mountArchive(baseDirectory);
    fileSystem.setWriteDirectory(baseDirectory);

    Path path("DoesNotExist.txt");

    REQUIRE_THROWS_AS(fileSystem.openFileForRead(path), Error);
}
