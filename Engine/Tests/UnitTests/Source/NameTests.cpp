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
#include <Hect/Core/Name.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct empty name", "[Name]")
{
    Name name;

    REQUIRE(name.empty());
    REQUIRE(name.as_string() == "");
}

TEST_CASE("Construct non-empty name", "[Name]")
{
    Name name("Test");

    REQUIRE(!name.empty());
    REQUIRE(name.as_string() == "Test");
}

TEST_CASE("Compare two different names", "[Name]")
{
    Name name_a("TestA");
    Name name_b("TestB");

    REQUIRE(name_a != name_b);
    REQUIRE(!(name_a == name_b));
}

TEST_CASE("Compare two equivalent names", "[Name]")
{
    Name name_a("Test");
    Name name_b("Test");

    REQUIRE(name_a == name_b);
    REQUIRE(!(name_a != name_b));
}
