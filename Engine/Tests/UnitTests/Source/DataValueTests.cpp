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
#include <Hect/IO/DataValue.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct a null data value", "[DataValue]")
{
    DataValue value;
    REQUIRE(value.is_null());
}

TEST_CASE("Construct a boolean data value", "[DataValue]")
{
    DataValue value(true);
    REQUIRE(value.is_bool());
    REQUIRE(value.as_bool());
}

TEST_CASE("Construct a number data value", "[DataValue]")
{
    DataValue value(5.0);
    REQUIRE(value.is_number());
    REQUIRE(value.as_double() == 5.0);
}

TEST_CASE("Construct a string data value", "[DataValue]")
{
    DataValue value("Testing");
    REQUIRE(value.is_string());
    REQUIRE(value.as_string() == "Testing");
}

TEST_CASE("Construct an array data value", "[DataValue]")
{
    DataValue value(DataValueType::Array);
    value.add_element(true);
    value.add_element(5.0);
    value.add_element("Testing");

    REQUIRE(value.is_array());
    REQUIRE(value.size() == 3u);

    for (unsigned i = 0; i < value.size(); ++i)
    {
        if (i == 0)
        {
            REQUIRE(value[i].is_bool());
            REQUIRE(value[i].as_bool());
        }
        else if (i == 1)
        {
            REQUIRE(value[i].is_number());
            REQUIRE(value[i].as_double() == 5.0);
        }
        else if (i == 2)
        {
            REQUIRE(value[i].is_string());
            REQUIRE(value[i].as_string() == "Testing");
        }
    }
}

TEST_CASE("Construct an object data value", "[DataValue]")
{
    DataValue value(DataValueType::Object);
    value.add_member("some_bool", true);
    value.add_member("some_number", 5.0);
    value.add_member("some_string", "Testing");

    REQUIRE(value.is_object());
    REQUIRE(value.size() == 3u);

    REQUIRE(value["some_bool"].is_bool());
    REQUIRE(value["some_bool"].as_bool());
    REQUIRE(value["some_number"].is_number());
    REQUIRE(value["some_number"].as_double() == 5.0);
    REQUIRE(value["some_string"].is_string());
    REQUIRE(value["some_string"].as_string() == "Testing");
}

TEST_CASE("Construct a 2-dimensional vector data value", "[DataValue]")
{
    DataValue value(Vector2(1, 2));
    REQUIRE(value.is_array());

    Vector2 v = value.as_vector2();
    REQUIRE(v.x == 1);
    REQUIRE(v.y == 2);
}

TEST_CASE("Construct a 3-dimensional vector data value", "[DataValue]")
{
    DataValue value(Vector3(1, 2, 3));
    REQUIRE(value.is_array());

    Vector3 v = value.as_vector3();
    REQUIRE(v.x == 1);
    REQUIRE(v.y == 2);
    REQUIRE(v.z == 3);
}

TEST_CASE("Construct a 4-dimensional vector data value", "[DataValue]")
{
    DataValue value(Vector4(1, 2, 3, 4));
    REQUIRE(value.is_array());

    Vector4 v = value.as_vector4();
    REQUIRE(v.x == 1);
    REQUIRE(v.y == 2);
    REQUIRE(v.z == 3);
    REQUIRE(v.w == 4);
}

TEST_CASE("Get the value of a data value as a name", "[DataValue]")
{
    static Name testing("Testing");

    DataValue value(testing);
    REQUIRE(value.is_string());
    REQUIRE(value.as_string() == "Testing");
    REQUIRE(value.as_name() == testing);
}

TEST_CASE("Get the member names of an object data value", "[DataValue]")
{
    DataValue value(DataValueType::Object);
    value.add_member("some_bool", true);
    value.add_member("some_number", 5.0);
    value.add_member("some_string", "Testing");

    auto member_names = value.member_names();

    REQUIRE(member_names.size() == 3u);
    REQUIRE(std::find(member_names.begin(), member_names.end(), "some_bool") != member_names.end());
    REQUIRE(std::find(member_names.begin(), member_names.end(), "some_number") != member_names.end());
    REQUIRE(std::find(member_names.begin(), member_names.end(), "some_string") != member_names.end());
}

TEST_CASE("Null switch on a null data value", "[DataValue]")
{
    DataValue value;
    REQUIRE(value.or_default(true).as_bool());
}

TEST_CASE("Null switch on a non-null data value", "[DataValue]")
{
    DataValue value(true);
    REQUIRE(value.or_default(false).as_bool());
}

TEST_CASE("Iterate over array data value", "[DataValue]")
{
    DataValue value(DataValueType::Array);
    for (int i = 0; i < 10; ++i)
    {
        value.add_element(static_cast<double>(i));
    }

    REQUIRE(value.is_array());
    REQUIRE(value.size() == 10u);

    int i = 0;
    for (const DataValue& element : value)
    {
        REQUIRE(element.is_number());
        REQUIRE(element.as_double() == i++);
    }
}

TEST_CASE("Iterate over null data value", "[DataValue]")
{
    DataValue value;
    for (const DataValue& element : value)
    {
        (void)element;
        REQUIRE(false);
    }
}

TEST_CASE("Perform member and array accesses on null data value", "[DataValue]")
{
    DataValue value;
    REQUIRE(value["root"][10]["something"].is_null());
}

TEST_CASE("Decode data value from a YAML string", "[DataValue][Yaml]")
{
    std::string yaml = "---\nsome_bool: true\nsome_number: 1.0\nsome_array: [ 0, 1, 2 ]\nsome_string: Testing 1 2 3";
    DataValue value;
    value.decode_from_yaml(yaml);

    REQUIRE(value.is_object());
    REQUIRE(value.size() == 4u);
    REQUIRE(value["some_bool"].is_bool());
    REQUIRE(value["some_bool"].as_bool());
    REQUIRE(value["some_number"].is_number());
    REQUIRE(value["some_number"].as_double() == 1.0);
    REQUIRE(value["some_string"].is_string());
    REQUIRE(value["some_string"].as_string() == "Testing 1 2 3");
    REQUIRE(value["some_array"].is_array());
    REQUIRE(value["some_array"].size() == 3u);
    REQUIRE(value["some_array"][0].as_int() == 0);
    REQUIRE(value["some_array"][1].as_int() == 1);
    REQUIRE(value["some_array"][2].as_int() == 2);
}
