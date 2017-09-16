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
#include <Hect/Core/Optional.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct an empty optional container", "[Optional]")
{
    Optional<std::string> optional;

    REQUIRE(!optional.has_value());
    REQUIRE(!optional);
}

TEST_CASE("Construct an non-empty optional container", "[Optional]")
{
    Optional<std::string> optional("Value");

    REQUIRE(optional.has_value());
    REQUIRE(optional);
}

TEST_CASE("Retrieve the value in an optional container", "[Optional]")
{
    Optional<std::string> optional("Value");

    REQUIRE(*optional == "Value");
    REQUIRE(optional.value() == "Value");

    const Optional<std::string>& const_optional = optional;
    REQUIRE(*const_optional == "Value");
    REQUIRE(const_optional.value() == "Value");
}

TEST_CASE("Update the value in an optional container using assignment", "[Optional]")
{
    Optional<std::string> optional("Value");

    REQUIRE(*optional == "Value");

    optional = "NewValue";

    REQUIRE(*optional == "NewValue");
}

TEST_CASE("Update the value in an optional container using movement", "[Optional]")
{
    Optional<std::string> optional("Value");

    REQUIRE(*optional == "Value");

    std::string new_value("NewValue");
    optional = std::move(new_value);

    REQUIRE(*optional == "NewValue");
}

TEST_CASE("Update the value in an optional container using deferencing", "[Optional]")
{
    Optional<std::string> optional("Value");

    REQUIRE(*optional == "Value");
    *optional = "NewValue";
    REQUIRE(*optional == "NewValue");
}

TEST_CASE("Copy an optional container without a value", "[Optional]")
{
    Optional<std::string> optional;

    Optional<std::string> optional_copy = optional;
    REQUIRE(!optional_copy.has_value());
}

TEST_CASE("Copy an optional container with a value", "[Optional]")
{
    Optional<std::string> optional("Value");

    Optional<std::string> optional_copy = optional;
    REQUIRE(*optional_copy == "Value");
}

TEST_CASE("Move an optional container without a value", "[Optional]")
{
    Optional<std::string> optional;

    Optional<std::string> optional_copy = std::move(optional);
    REQUIRE(!optional_copy.has_value());
}

TEST_CASE("Move an optional container with a value", "[Optional]")
{
    Optional<std::string> optional("Value");

    Optional<std::string> optional_copy = std::move(optional);
    REQUIRE(*optional_copy == "Value");
}
