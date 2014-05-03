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
#pragma once

SUITE(Json)
{
    TEST(Null)
    {
        JsonValue value;
        CHECK(value.isNull());
    }

    TEST(Bool)
    {
        JsonValue value(true);
        CHECK(value.isBool());
        CHECK(value.asBool());
    }

    TEST(Number)
    {
        JsonValue value(5.0);
        CHECK(value.isNumber());
        CHECK_EQUAL(5.0, value.asDouble());
    }

    TEST(String)
    {
        JsonValue value("Testing");
        CHECK(value.isString());
        CHECK_EQUAL("Testing", value.asString());
    }

    TEST(Array)
    {
        JsonValue value(JsonValueType::Array);
        value.addElement(true);
        value.addElement(5.0);
        value.addElement("Testing");

        CHECK(value.isArray());
        CHECK_EQUAL(3u, value.size());

        for (unsigned i = 0; i < value.size(); ++i)
        {
            if (i == 0)
            {
                CHECK(value[i].isBool());
                CHECK(value[i].asBool());
            }
            else if (i == 1)
            {
                CHECK(value[i].isNumber());
                CHECK_EQUAL(5.0, value[i].asDouble());
            }
            else if (i == 2)
            {
                CHECK(value[i].isString());
                CHECK_EQUAL("Testing", value[i].asString());
            }
        }
    }

    TEST(Object)
    {
        JsonValue value(JsonValueType::Object);
        value.addMember("someBool", true);
        value.addMember("someNumber", 5.0);
        value.addMember("someString", "Testing");

        CHECK(value.isObject());
        CHECK_EQUAL(3u, value.size());

        CHECK(value["someBool"].isBool());
        CHECK(value["someBool"].asBool());
        CHECK(value["someNumber"].isNumber());
        CHECK_EQUAL(5.0, value["someNumber"].asDouble());
        CHECK(value["someString"].isString());
        CHECK_EQUAL("Testing", value["someString"].asString());
    }

    TEST(Vector2)
    {
        JsonValue value(Vector2(1, 2));
        CHECK(value.isArray());

        Vector2 v = value.asVector2();
        CHECK_EQUAL(1, v.x);
        CHECK_EQUAL(2, v.y);
    }

    TEST(Vector3)
    {
        JsonValue value(Vector3(1, 2, 3));
        CHECK(value.isArray());

        Vector3 v = value.asVector3();
        CHECK_EQUAL(1, v.x);
        CHECK_EQUAL(2, v.y);
        CHECK_EQUAL(3, v.z);
    }

    TEST(Vector4)
    {
        JsonValue value(Vector4(1, 2, 3, 4));
        CHECK(value.isArray());

        Vector4 v = value.asVector4();
        CHECK_EQUAL(1, v.x);
        CHECK_EQUAL(2, v.y);
        CHECK_EQUAL(3, v.z);
        CHECK_EQUAL(4, v.w);
    }

    TEST(MemberNames)
    {
        JsonValue value(JsonValueType::Object);
        value.addMember("someBool", true);
        value.addMember("someNumber", 5.0);
        value.addMember("someString", "Testing");

        auto memberNames = value.memberNames();

        CHECK_EQUAL(3u, memberNames.size());
        CHECK(std::find(memberNames.begin(), memberNames.end(), "someBool") != memberNames.end());
        CHECK(std::find(memberNames.begin(), memberNames.end(), "someNumber") != memberNames.end());
        CHECK(std::find(memberNames.begin(), memberNames.end(), "someString") != memberNames.end());
    }

    TEST(OrFromNull)
    {
        JsonValue value;
        CHECK(value.or(true).asBool());
    }

    TEST(OrFromNotNull)
    {
        JsonValue value(true);
        CHECK(value.or(false).asBool());
    }

    TEST(Iterate)
    {
        JsonValue value(JsonValueType::Array);
        for (int i = 0; i < 10; ++i)
        {
            value.addElement((double)i);
        }

        CHECK(value.isArray());
        CHECK_EQUAL(10u, value.size());

        int i = 0;
        for (const JsonValue& element : value)
        {
            CHECK(element.isNumber());
            CHECK_EQUAL(i++, element.asDouble());
        }
    }

    TEST(IterateOnNull)
    {
        JsonValue value;
        for (const JsonValue& element : value)
        {
            element;
            CHECK(false);
        }
    }

    TEST(DeepNull)
    {
        JsonValue value;
        CHECK(value["root"][10]["something"].isNull());
    }

    TEST(DecodeFromJson)
    {
        std::string json = "{ \"someBool\" : true, \"someArray\" : [ 0, 1, 2 ] }";
        JsonValue value;
        value.decodeFromJson(json);

        CHECK(value.isObject());
        CHECK_EQUAL(2u, value.size());
        CHECK(value["someBool"].isBool());
        CHECK(value["someBool"].asBool());
        CHECK(value["someArray"].isArray());
        CHECK_EQUAL(3u, value["someArray"].size());
        CHECK_EQUAL(0, value["someArray"][0].asInt());
        CHECK_EQUAL(1, value["someArray"][1].asInt());
        CHECK_EQUAL(2, value["someArray"][2].asInt());
    }
}