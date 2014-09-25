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
            jsonValue = *encoder.jsonValues().begin();
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
        encoder << beginObject()
            << encodeValue("String", std::string("Testing"))
            << endObject();
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
        encoder << beginArray()
            << encodeValue(std::string("Zero"))
            << encodeValue(std::string("One"))
            << encodeValue(std::string("Two"))
            << endArray();
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
        encoder << beginObject()
            << beginArray("Array")
            << encodeValue(std::string("Zero"))
            << encodeValue(std::string("One"))
            << encodeValue(std::string("Two"))
            << endArray()
            << endObject();
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
        encoder << beginArray();
        for (int i = 0; i < 3; ++i)
        {
            encoder << beginArray()
                << encodeValue(std::string("Zero"))
                << encodeValue(std::string("One"))
                << encodeValue(std::string("Two"))
                << endArray();
        }
        encoder << endArray();
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
        encoder << beginArray();
        for (int i = 0; i < 3; ++i)
        {
            encoder << beginObject()
                << encodeValue("String", std::string("Testing"))
                << endObject();
        }
        encoder << endArray();
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
        encoder << beginArray()
            << encodeValue(std::string("Test"))
            << encodeValue((int8_t)12)
            << encodeValue((uint8_t)12)
            << encodeValue((int16_t)12)
            << encodeValue((uint16_t)12)
            << encodeValue((int32_t)12)
            << encodeValue((uint32_t)12)
            << encodeValue((int64_t)12)
            << encodeValue((uint64_t)12)
            << encodeValue(123.0f)
            << encodeValue(123.0)
            << encodeValue(true)
            << endArray();
    }, [](Decoder& decoder)
    {
        ArrayDecoder array = decoder.decodeArray();

        REQUIRE(array.decodeString() == "Test");
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeInt8() == 12);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeUInt8() == 12u);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeInt16() == 12);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeUInt16() == 12u);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeInt32() == 12);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeUInt32() == 12u);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeInt64() == 12);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeUInt64() == 12u);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeFloat32() == 123.0f);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeFloat64() == 123.0);
        REQUIRE(array.hasMoreElements());
        REQUIRE(array.decodeBool() == true);
        REQUIRE(!array.hasMoreElements());
    });
}

TEST_CASE("Encoding_AllInObject")
{
    testEncodeAndDecode([](Encoder& encoder)
    {
        encoder << beginObject()
            << encodeValue("String", std::string("Test"))
            << encodeValue("Int8", (int8_t)12)
            << encodeValue("UInt8", (uint8_t)12)
            << encodeValue("Int16", (int16_t)12)
            << encodeValue("UInt16", (uint16_t)12)
            << encodeValue("Int32", (int32_t)12)
            << encodeValue("UInt32", (uint32_t)12)
            << encodeValue("Int64", (int64_t)12)
            << encodeValue("UInt64", (uint64_t)12)
            << encodeValue("Float32", 123.0f)
            << encodeValue("Float64", 123.0)
            << encodeValue("Bool", true)
            << endObject();
    }, [](Decoder& decoder)
    {
        ObjectDecoder object = decoder.decodeObject();

        REQUIRE(object.hasMember("String"));
        REQUIRE(object.decodeString("String") == "Test");
        REQUIRE(object.hasMember("Int8"));
        REQUIRE(object.decodeInt8("Int8") == 12);
        REQUIRE(object.hasMember("UInt8"));
        REQUIRE(object.decodeUInt8("UInt8") == 12u);
        REQUIRE(object.hasMember("Int16"));
        REQUIRE(object.decodeInt16("Int16") == 12);
        REQUIRE(object.hasMember("UInt16"));
        REQUIRE(object.decodeUInt16("UInt16") == 12u);
        REQUIRE(object.hasMember("Int32"));
        REQUIRE(object.decodeInt32("Int32") == 12);
        REQUIRE(object.hasMember("UInt32"));
        REQUIRE(object.decodeUInt32("UInt32") == 12u);
        REQUIRE(object.hasMember("Int64"));
        REQUIRE(object.decodeInt64("Int64") == 12);
        REQUIRE(object.hasMember("UInt64"));
        REQUIRE(object.decodeUInt64("UInt64") == 12u);
        REQUIRE(object.hasMember("Float32"));
        REQUIRE(object.decodeFloat32("Float32") == 123.0f);
        REQUIRE(object.hasMember("Float64"));
        REQUIRE(object.decodeFloat64("Float64") == 123.0);
        REQUIRE(object.hasMember("Bool"));
        REQUIRE(object.decodeBool("Bool") == true);
    });
}