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
#include <Hect/IO/Path.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Path_DefaultConstructor")
{
    Path path;

    REQUIRE(path.toString() == "");
    REQUIRE(path.empty());
}

TEST_CASE("Path_ConstructorWithOnePart")
{
    Path path("Data");

    REQUIRE(path.toString() == "Data");
    REQUIRE(!path.empty());
}

TEST_CASE("Path_ConstructorWithMultipleParts")
{
    Path path("Data/Internal/Fail.log");

    REQUIRE(path.toString() == "Data/Internal/Fail.log");
}

TEST_CASE("Path_ConstructorWithLeadingSlash")
{
    Path path("/Data/Internal/Fail.log");

    REQUIRE(path.toString() == "Data/Internal/Fail.log");
}

TEST_CASE("Path_ConstructorWithTrailingSlash")
{
    Path path("Data/Internal/");

    REQUIRE(path.toString() == "Data/Internal");
}

TEST_CASE("Path_ConstructorWithLeadingAndTrailingSlash")
{
    Path path("/Data/Internal/");

    REQUIRE(path.toString() == "Data/Internal");
}

TEST_CASE("Path_Extension")
{
    Path path("Data/Internal/Fail.log");

    REQUIRE(path.extension() == "log");
}

TEST_CASE("Path_NoExtension")
{
    Path path("Data/Internal/Fail");

    REQUIRE(path.extension() == "");
}

TEST_CASE("Path_Add")
{
    Path path = Path("Data") + Path("Internal/Fail.log");

    REQUIRE(path.toString() == "Data/Internal/Fail.log");
}

TEST_CASE("Path_AddWithEmptyFirst")
{
    Path path = Path() + Path("Internal/Fail.log");

    REQUIRE(path.toString() == "Internal/Fail.log");
}

TEST_CASE("Path_AddWithEmptySecond")
{
    Path path = Path("Data/Internal") + Path();

    REQUIRE(path.toString() == "Data/Internal");
}

TEST_CASE("Path_AddEquals")
{
    Path path("Data");
    path += "Internal/Fail.log";

    REQUIRE(path.toString() == "Data/Internal/Fail.log");
}

TEST_CASE("Path_AddEqualsFromEmpty")
{
    Path path;
    path += "Data";
    path += "Internal/Fail.log";

    REQUIRE(path.toString() == "Data/Internal/Fail.log");
}

TEST_CASE("Path_ParentDirectory")
{
    Path path("/Data/Internal/Fail.log");

    Path parentDirectory = path.parentDirectory();
    REQUIRE(parentDirectory.toString() == "Data/Internal");
    REQUIRE(!parentDirectory.empty());

    parentDirectory = parentDirectory.parentDirectory();
    REQUIRE(parentDirectory.toString() == "Data");
    REQUIRE(!parentDirectory.empty());

    parentDirectory = parentDirectory.parentDirectory();
    REQUIRE(parentDirectory.toString() == "");
    REQUIRE(parentDirectory.empty());
}

TEST_CASE("Path_StreamOutput")
{
    Path path("/Data/Internal/Fail.log");

    std::stringstream ss;
    ss << path;

    REQUIRE(ss.str() == "Data/Internal/Fail.log");
}