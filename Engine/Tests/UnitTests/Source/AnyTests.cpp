///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
#include <Hect/Core/Any.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct an empty any value", "[Any]")
{
    Any a;

    REQUIRE(!a);
    REQUIRE(!a.hasValue());
}

TEST_CASE("Construct a non-empty any value", "[Any]")
{
    Any a(5);

    REQUIRE(a);
    REQUIRE(a.hasValue());
}

TEST_CASE("Detect type of an empty any value", "[Any]")
{
    Any a;

    REQUIRE(!a.isType<int>());
    REQUIRE(!a.isType<double>());
}

TEST_CASE("Detect type of a non-empty any value", "[Any]")
{
    Any a(5);

    REQUIRE(a.isType<int>());
    REQUIRE(!a.isType<double>());
}

TEST_CASE("Cast any value to correct type", "[Any]")
{
    Any a(5);

    REQUIRE(a);
    REQUIRE(a.as<int>() == 5);
}

TEST_CASE("Cast any value to incorrect type", "[Any]")
{
    Any a(5);

    REQUIRE(a);
    REQUIRE_THROWS_AS(a.as<double>(), InvalidOperation);
}

TEST_CASE("Copy an any value", "[Any]")
{
    Any a(5);
    REQUIRE(a);
    REQUIRE(a.as<int>() == 5);

    Any b(a);
    REQUIRE(b);
    REQUIRE(b.as<int>() == 5);
}

TEST_CASE("Assign any value to new type", "[Any]")
{
    Any a(5);
    REQUIRE(a);
    REQUIRE(a.as<int>() == 5);

    a = std::string("Testing");
    REQUIRE(a);
    REQUIRE(a.as<std::string>() == "Testing");
}

TEST_CASE("Move an any value", "[Any]")
{
    Any a(5);
    REQUIRE(a);
    REQUIRE(a.as<int>() == 5);

    Any b(std::move(a));
    REQUIRE(!a);
    REQUIRE(!a.hasValue());
    REQUIRE(b.as<int>() == 5);
}

TEST_CASE("Assign any value moved from another any value", "[Any]")
{
    Any a(5);
    REQUIRE(a);
    REQUIRE(a.as<int>() == 5);

    Any b;
    b = std::move(a);
    REQUIRE(!a);
    REQUIRE(!a.hasValue());
    REQUIRE(b);
    REQUIRE(b.as<int>() == 5);
}