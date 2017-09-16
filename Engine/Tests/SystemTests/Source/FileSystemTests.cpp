///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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
#include <Hect.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Create and remove directories", "[FileSystem]")
{
    Engine& engine = Engine::instance();
    FileSystem& file_system = engine.file_system();

    Path base_directory = file_system.base_directory();
    file_system.mount_archive(base_directory);
    file_system.set_write_directory(base_directory);

    Path path("Directory");

    REQUIRE(!file_system.exists(path));
    file_system.create_directory(path);
    REQUIRE(file_system.exists(path));
    file_system.remove(path);
    REQUIRE(!file_system.exists(path));
}

TEST_CASE("Open non-existing file for write", "[FileSystem]")
{
    Engine& engine = Engine::instance();
    FileSystem& file_system = engine.file_system();

    Path base_directory = file_system.base_directory();
    file_system.mount_archive(base_directory);
    file_system.set_write_directory(base_directory);

    Path path("File.txt");

    REQUIRE(!file_system.exists(path));
    {
        auto stream = file_system.open_file_for_write(path);
    }
    REQUIRE(file_system.exists(path));
    file_system.remove(path);
    REQUIRE(!file_system.exists(path));
}

TEST_CASE("Open existing file for write", "[FileSystem]")
{
    Engine& engine = Engine::instance();
    FileSystem& file_system = engine.file_system();

    Path base_directory = file_system.base_directory();
    file_system.mount_archive(base_directory);
    file_system.set_write_directory(base_directory);

    Path path("File.txt");

    REQUIRE(!file_system.exists(path));
    {
        auto stream = file_system.open_file_for_write(path);
    }
    REQUIRE(file_system.exists(path));
    {
        auto stream = file_system.open_file_for_write(path);
    }
    REQUIRE(file_system.exists(path));
    file_system.remove(path);
    REQUIRE(!file_system.exists(path));
}

TEST_CASE("Open existing file for read", "[FileSystem]")
{
    Engine& engine = Engine::instance();
    FileSystem& file_system = engine.file_system();

    Path base_directory = file_system.base_directory();
    file_system.mount_archive(base_directory);
    file_system.set_write_directory(base_directory);

    Path path("File.txt");

    REQUIRE(!file_system.exists(path));
    {
        auto stream = file_system.open_file_for_write(path);
    }
    REQUIRE(file_system.exists(path));
    {
        auto stream = file_system.open_file_for_read(path);
    }

    file_system.remove(path);
    REQUIRE(!file_system.exists(path));
}

TEST_CASE("Open non-existing file for read", "[FileSystem]")
{
    Engine& engine = Engine::instance();
    FileSystem& file_system = engine.file_system();

    Path base_directory = file_system.base_directory();
    file_system.mount_archive(base_directory);
    file_system.set_write_directory(base_directory);

    Path path("DoesNotExist.txt");

    REQUIRE_THROWS_AS(file_system.open_file_for_read(path), IOError);
}
