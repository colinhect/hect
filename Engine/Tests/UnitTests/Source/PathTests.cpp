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
#include <Hect/IO/Path.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct an empty path", "[Path]")
{
    Path path;

    REQUIRE(path.as_string() == "");
    REQUIRE(path.empty());
}

TEST_CASE("Construct a path with single part", "[Path]")
{
    Path path("Data");

    REQUIRE(path.as_string() == "Data");
    REQUIRE(!path.empty());
}

TEST_CASE("Construct a path with multiple parts", "[Path]")
{
    Path path("Data/Internal/Fail.log");

    REQUIRE(path.as_string() == "Data/Internal/Fail.log");
}

TEST_CASE("Construct a path with leading slash", "[Path]")
{
    Path path("/Data/Internal/Fail.log");

    REQUIRE(path.as_string() == "/Data/Internal/Fail.log");
}

TEST_CASE("Construct a path with trailing slash", "[Path]")
{
    Path path("Data/Internal/");

    REQUIRE(path.as_string() == "Data/Internal");
}

TEST_CASE("Construct a path with trailing and leading slash", "[Path]")
{
    Path path("/Data/Internal/");

    REQUIRE(path.as_string() == "/Data/Internal");
}

TEST_CASE("Get extension of a path with an extension", "[Path]")
{
    Path path("Data/Internal/Fail.log");

    REQUIRE(path.extension() == "log");
}

TEST_CASE("Get the extension of a path without an extension", "[Path]")
{
    Path path("Data/Internal/Fail");

    REQUIRE(path.extension() == "");
}

TEST_CASE("Add two paths into a single path", "[Path]")
{
    Path path = Path("Data") + Path("Internal/Fail.log");

    REQUIRE(path.as_string() == "Data/Internal/Fail.log");
}

TEST_CASE("Add an empty path with a non-empty path", "[Path]")
{
    Path path = Path() + Path("Internal/Fail.log");

    REQUIRE(path.as_string() == "Internal/Fail.log");
}

TEST_CASE("Add a path to an empty path", "[Path]")
{
    Path path = Path("Data/Internal") + Path();

    REQUIRE(path.as_string() == "Data/Internal");
}

TEST_CASE("Append a path to an existing path", "[Path]")
{
    Path path("Data");
    path += "Internal/Fail.log";

    REQUIRE(path.as_string() == "Data/Internal/Fail.log");
}

TEST_CASE("Append paths to an empty path", "[Path]")
{
    Path path;
    path += "Data";
    path += "Internal/Fail.log";

    REQUIRE(path.as_string() == "Data/Internal/Fail.log");
}

TEST_CASE("Get the parent directory of a non-empty path", "[Path]")
{
    Path path("/Data/Internal/Fail.log");

    Path parent_directory = path.parent_directory();
    REQUIRE(parent_directory.as_string() == "/Data/Internal");
    REQUIRE(!parent_directory.empty());

    parent_directory = parent_directory.parent_directory();
    REQUIRE(parent_directory.as_string() == "/Data");
    REQUIRE(!parent_directory.empty());

    parent_directory = parent_directory.parent_directory();
    REQUIRE(parent_directory.as_string() == "");
    REQUIRE(parent_directory.empty());
}
