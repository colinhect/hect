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
#include <Hect/IO/Encoder.h>
#include <Hect/IO/Decoder.h>
#include <Hect/IO/JsonEncoder.h>
#include <Hect/IO/JsonDecoder.h>
#include <Hect/IO/BinaryEncoder.h>
#include <Hect/IO/BinaryDecoder.h>
#include <Hect/IO/MemoryWriteStream.h>
#include <Hect/IO/MemoryReadStream.h>
using namespace hect;

#include <catch.hpp>
#include <functional>

void testEncodeAndDecode(std::function<void(Encoder&)> encode, std::function<void(Decoder&)> decode)
{
    // JSON
    {
        JsonValue jsonValue;
        {
            JsonEncoder encoder;
            encode(encoder);
            jsonValue = encoder.jsonValues()[0];
        }
        {
            JsonDecoder decoder(jsonValue);
            decode(decoder);
        }
    }

    // Binary
    {
        std::vector<uint8_t> data;
        {
            MemoryWriteStream stream(data);
            BinaryEncoder encoder(stream);
            encode(encoder);
        }
        {
            MemoryReadStream stream(data);
            BinaryDecoder decoder(stream);
            decode(decoder);
        }
    }
}

TEST_CASE("Encoding_SingleObject")
{
    testEncodeAndDecode([](Encoder& encoder)
    {
        ObjectEncoder object = encoder.encodeObject();
        object.encodeString("String", "Testing");
    }, [](Decoder& decoder)
    {
        ObjectDecoder object = decoder.decodeObject();

        REQUIRE(object.hasMember("String"));
        REQUIRE(object.decodeString("String") == "Testing");
    });
}

TEST_CASE("Encoding_SingleArray")
{
    testEncodeAndDecode([](Encoder& encoder)
    {
        ArrayEncoder array = encoder.encodeArray();
        array.encodeString("Zero");
        array.encodeString("One");
        array.encodeString("Two");
    }, [](Decoder& decoder)
    {
        ArrayDecoder array = decoder.decodeArray();

        std::vector<std::string> strings;
        while (array.hasMoreElements())
        {
            strings.push_back(array.decodeString());
        }

        REQUIRE(strings[0] == "Zero");
        REQUIRE(strings[1] == "One");
        REQUIRE(strings[2] == "Two");
    });
}

TEST_CASE("Encoding_ArrayInObject")
{
    testEncodeAndDecode([](Encoder& encoder)
    {
        ObjectEncoder object = encoder.encodeObject();
        ArrayEncoder array = object.encodeArray("Array");
        array.encodeString("Zero");
        array.encodeString("One");
        array.encodeString("Two");
    }, [](Decoder& decoder)
    {
        ObjectDecoder object = decoder.decodeObject();

        REQUIRE(object.hasMember("Array"));

        ArrayDecoder array = object.decodeArray("Array");

        std::vector<std::string> strings;
        while (array.hasMoreElements())
        {
            strings.push_back(array.decodeString());
        }

        REQUIRE(strings[0] == "Zero");
        REQUIRE(strings[1] == "One");
        REQUIRE(strings[2] == "Two");
    });
}

TEST_CASE("Encoding_ArrayInArray")
{
    testEncodeAndDecode([](Encoder& encoder)
    {
        ArrayEncoder array = encoder.encodeArray();

        for (int i = 0; i < 3; ++i)
        {
            ArrayEncoder nested = array.encodeArray();
            nested.encodeString("Zero");
            nested.encodeString("One");
            nested.encodeString("Two");
        }
    }, [](Decoder& decoder)
    {
        ArrayDecoder array = decoder.decodeArray();

        int arrayCount = 0;
        while (array.hasMoreElements())
        {
            ArrayDecoder nested = array.decodeArray();

            std::vector<std::string> strings;
            while (nested.hasMoreElements())
            {
                strings.push_back(nested.decodeString());
            }

            REQUIRE(strings[0] == "Zero");
            REQUIRE(strings[1] == "One");
            REQUIRE(strings[2] == "Two");

            ++arrayCount;
        }

        REQUIRE(arrayCount == 3);
    });
}

TEST_CASE("Encoding_ObjectInArray")
{
    testEncodeAndDecode([](Encoder& encoder)
    {
        ArrayEncoder array = encoder.encodeArray();

        for (int i = 0; i < 3; ++i)
        {
            ObjectEncoder object = array.encodeObject();
            object.encodeString("String", "Testing");
        }
    }, [](Decoder& decoder)
    {
        ArrayDecoder array = decoder.decodeArray();

        int objectCount = 0;
        while (array.hasMoreElements())
        {
            ObjectDecoder object = array.decodeObject();

            REQUIRE(object.hasMember("String"));
            REQUIRE(object.decodeString("String") == "Testing");

            ++objectCount;
        }

        REQUIRE(objectCount == 3);
    });
}

TEST_CASE("Encoding_AllInArray")
{
    testEncodeAndDecode([](Encoder& encoder)
    {
        ArrayEncoder array = encoder.encodeArray();

        array.encodeString("Test");
        array.encodeByte(12);
        array.encodeUnsignedByte(12);
        array.encodeShort(12);
        array.encodeUnsignedShort(12);
        array.encodeInt(12);
        array.encodeUnsignedInt(12);
        array.encodeLong(12);
        array.encodeUnsignedLong(12);
        array.encodeFloat(123.0f);
        array.encodeDouble(123.0);
        array.encodeReal(123);
        array.encodeBool(true);
        array.encodeVector2(Vector2(1, 2));
        array.encodeVector3(Vector3(1, 2, 3));
        array.encodeVector4(Vector4(1, 2, 3, 4));
    }, [](Decoder& decoder)
    {
        ArrayDecoder array = decoder.decodeArray();

        REQUIRE(array.decodeString() == "Test");
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeByte() == 12);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeUnsignedByte() == 12u);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeShort() == 12);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeUnsignedShort() == 12u);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeInt() == 12);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeUnsignedInt() == 12u);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeLong() == 12);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeUnsignedLong() == 12u);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeFloat() == 123.0f);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeDouble() == 123.0);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeReal() == 123);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeBool() == true);
        REQUIRE(array.hasMoreElements());
        Vector2 v2 = array.decodeVector2();
        REQUIRE(v2.x == 1);
        REQUIRE(v2.y == 2);
        REQUIRE(array.hasMoreElements());
        Vector3 v3 = array.decodeVector3();
        REQUIRE(v3.x == 1);
        REQUIRE(v3.y == 2);
        REQUIRE(v3.z == 3);
        REQUIRE(array.hasMoreElements());
        Vector4 v4 = array.decodeVector4();
        REQUIRE(v4.x == 1);
        REQUIRE(v4.y == 2);
        REQUIRE(v4.z == 3);
        REQUIRE(v4.w == 4);
        REQUIRE(!array.hasMoreElements());
    });
}

TEST_CASE("Encoding_AllInObject")
{
    testEncodeAndDecode([](Encoder& encoder)
    {
        ObjectEncoder object = encoder.encodeObject();

        object.encodeString("String", "Test");
        object.encodeByte("Byte", 12);
        object.encodeUnsignedByte("UnsignedByte", 12);
        object.encodeShort("Short", 12);
        object.encodeUnsignedShort("UnsignedShort", 12);
        object.encodeInt("Int", 12);
        object.encodeUnsignedInt("UnsignedInt", 12);
        object.encodeLong("Long", 12);
        object.encodeUnsignedLong("UnsignedLong", 12);
        object.encodeFloat("Float", 123.0f);
        object.encodeDouble("Double", 123.0);
        object.encodeReal("Real", 123);
        object.encodeBool("Bool", true);
        object.encodeVector2("Vector2", Vector2(1, 2));
        object.encodeVector3("Vector3", Vector3(1, 2, 3));
        object.encodeVector4("Vector4", Vector4(1, 2, 3, 4));
    }, [](Decoder& decoder)
    {
        ObjectDecoder object = decoder.decodeObject();

        REQUIRE(object.hasMember("String"));
        REQUIRE(object.decodeString("String") == "Test");
        REQUIRE(object.hasMember("Byte"));
        REQUIRE(object.decodeByte("Byte") == 12);
        REQUIRE(object.hasMember("UnsignedByte"));
        REQUIRE(object.decodeUnsignedByte("UnsignedByte") == 12u);
        REQUIRE(object.hasMember("Short"));
        REQUIRE(object.decodeShort("Short") == 12);
        REQUIRE(object.hasMember("UnsignedShort"));
        REQUIRE(object.decodeUnsignedShort("UnsignedShort") == 12u);
        REQUIRE(object.hasMember("Int"));
        REQUIRE(object.decodeInt("Int") == 12);
        REQUIRE(object.hasMember("UnsignedInt"));
        REQUIRE(object.decodeUnsignedInt("UnsignedInt") == 12u);
        REQUIRE(object.hasMember("Long"));
        REQUIRE(object.decodeLong("Long") == 12);
        REQUIRE(object.hasMember("UnsignedLong"));
        REQUIRE(object.decodeUnsignedLong("UnsignedLong") == 12u);
        REQUIRE(object.hasMember("Float"));
        REQUIRE(object.decodeFloat("Float") == 123.0f);
        REQUIRE(object.hasMember("Double"));
        REQUIRE(object.decodeDouble("Double") == 123.0);
        REQUIRE(object.hasMember("Real"));
        REQUIRE(object.decodeReal("Real") == 123);
        REQUIRE(object.hasMember("Bool"));
        REQUIRE(object.decodeBool("Bool") == true);
        REQUIRE(object.hasMember("Vector2"));
        Vector2 v2 = object.decodeVector2("Vector2");
        REQUIRE(v2.x == 1);
        REQUIRE(v2.y == 2);
        REQUIRE(object.hasMember("Vector3"));
        Vector3 v3 = object.decodeVector3("Vector3");
        REQUIRE(v3.x == 1);
        REQUIRE(v3.y == 2);
        REQUIRE(v3.z == 3);
        REQUIRE(object.hasMember("Vector4"));
        Vector4 v4 = object.decodeVector4("Vector4");
        REQUIRE(v4.x == 1);
        REQUIRE(v4.y == 2);
        REQUIRE(v4.z == 3);
        REQUIRE(v4.w == 4);
    });
}