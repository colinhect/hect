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
    REQUIRE(value.isNull());
}

TEST_CASE("Construct a boolean data value", "[DataValue]")
{
    DataValue value(true);
    REQUIRE(value.isBool());
    REQUIRE(value.asBool());
}

TEST_CASE("Construct a number data value", "[DataValue]")
{
    DataValue value(5.0);
    REQUIRE(value.isNumber());
    REQUIRE(value.asDouble() == 5.0);
}

TEST_CASE("Construct a string data value", "[DataValue]")
{
    DataValue value("Testing");
    REQUIRE(value.isString());
    REQUIRE(value.asString() == "Testing");
}

TEST_CASE("Construct an array data value", "[DataValue]")
{
    DataValue value(DataValueType::Array);
    value.addElement(true);
    value.addElement(5.0);
    value.addElement("Testing");

    REQUIRE(value.isArray());
    REQUIRE(value.size() == 3u);

    for (unsigned i = 0; i < value.size(); ++i)
    {
        if (i == 0)
        {
            REQUIRE(value[i].isBool());
            REQUIRE(value[i].asBool());
        }
        else if (i == 1)
        {
            REQUIRE(value[i].isNumber());
            REQUIRE(value[i].asDouble() == 5.0);
        }
        else if (i == 2)
        {
            REQUIRE(value[i].isString());
            REQUIRE(value[i].asString() == "Testing");
        }
    }
}

TEST_CASE("Construct an object data value", "[DataValue]")
{
    DataValue value(DataValueType::Object);
    value.addMember("someBool", true);
    value.addMember("someNumber", 5.0);
    value.addMember("someString", "Testing");

    REQUIRE(value.isObject());
    REQUIRE(value.size() == 3u);

    REQUIRE(value["someBool"].isBool());
    REQUIRE(value["someBool"].asBool());
    REQUIRE(value["someNumber"].isNumber());
    REQUIRE(value["someNumber"].asDouble() == 5.0);
    REQUIRE(value["someString"].isString());
    REQUIRE(value["someString"].asString() == "Testing");
}

TEST_CASE("Construct a 2-dimensional vector data value", "[DataValue]")
{
    DataValue value(Vector2(1, 2));
    REQUIRE(value.isArray());

    Vector2 v = value.asVector2();
    REQUIRE(v.x == 1);
    REQUIRE(v.y == 2);
}

TEST_CASE("Construct a 3-dimensional vector data value", "[DataValue]")
{
    DataValue value(Vector3(1, 2, 3));
    REQUIRE(value.isArray());

    Vector3 v = value.asVector3();
    REQUIRE(v.x == 1);
    REQUIRE(v.y == 2);
    REQUIRE(v.z == 3);
}

TEST_CASE("Construct a 4-dimensional vector data value", "[DataValue]")
{
    DataValue value(Vector4(1, 2, 3, 4));
    REQUIRE(value.isArray());

    Vector4 v = value.asVector4();
    REQUIRE(v.x == 1);
    REQUIRE(v.y == 2);
    REQUIRE(v.z == 3);
    REQUIRE(v.w == 4);
}

TEST_CASE("Get the member names of an object data value", "[DataValue]")
{
    DataValue value(DataValueType::Object);
    value.addMember("someBool", true);
    value.addMember("someNumber", 5.0);
    value.addMember("someString", "Testing");

    auto memberNames = value.memberNames();

    REQUIRE(memberNames.size() == 3u);
    REQUIRE(std::find(memberNames.begin(), memberNames.end(), "someBool") != memberNames.end());
    REQUIRE(std::find(memberNames.begin(), memberNames.end(), "someNumber") != memberNames.end());
    REQUIRE(std::find(memberNames.begin(), memberNames.end(), "someString") != memberNames.end());
}

TEST_CASE("Null switch on a null data value", "[DataValue]")
{
    DataValue value;
    REQUIRE(value.orDefault(true).asBool());
}

TEST_CASE("Null switch on a non-null data value", "[DataValue]")
{
    DataValue value(true);
    REQUIRE(value.orDefault(false).asBool());
}

TEST_CASE("Iterate over array data value", "[DataValue]")
{
    DataValue value(DataValueType::Array);
    for (int i = 0; i < 10; ++i)
    {
        value.addElement(static_cast<double>(i));
    }

    REQUIRE(value.isArray());
    REQUIRE(value.size() == 10u);

    int i = 0;
    for (const DataValue& element : value)
    {
        REQUIRE(element.isNumber());
        REQUIRE(element.asDouble() == i++);
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
    REQUIRE(value["root"][10]["something"].isNull());
}

TEST_CASE("Decode data value from a YAML string", "[DataValue][Yaml]")
{
    std::string yaml = "---\nsomeBool: true\nsomeNumber: 1.0\nsomeArray: [ 0, 1, 2 ]\nsomeString: Testing 1 2 3";
    DataValue value;
    value.decodeFromYaml(yaml);

    REQUIRE(value.isObject());
    REQUIRE(value.size() == 4u);
    REQUIRE(value["someBool"].isBool());
    REQUIRE(value["someBool"].asBool());
    REQUIRE(value["someNumber"].isNumber());
    REQUIRE(value["someNumber"].asDouble() == 1.0);
    REQUIRE(value["someString"].isString());
    REQUIRE(value["someString"].asString() == "Testing 1 2 3");
    REQUIRE(value["someArray"].isArray());
    REQUIRE(value["someArray"].size() == 3u);
    REQUIRE(value["someArray"][0].asInt() == 0);
    REQUIRE(value["someArray"][1].asInt() == 1);
    REQUIRE(value["someArray"][2].asInt() == 2);
}