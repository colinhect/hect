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
using namespace hect;

#include <catch.hpp>
#include <functional>

#include "Utilities.h"

TEST_CASE("Encode single object", "[Encoding]")
{
    testEncodeAndDecode([](Encoder& encoder)
    {
        encoder << beginObject()
                << encodeValue("String", std::string("Testing"))
                << endObject();
    }, [](Decoder& decoder)
    {
        std::string string;

        decoder >> beginObject()
                >> decodeValue("String", string)
                >> endObject();

        REQUIRE(string == "Testing");
    });
}

TEST_CASE("Encode single array", "[Encoding]")
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
        std::vector<std::string> strings;

        decoder >> beginArray();
        while (decoder.hasMoreElements())
        {
            std::string string;
            decoder >> decodeValue(string);
            strings.push_back(string);
        }
        decoder >> endArray();

        REQUIRE(strings[0] == "Zero");
        REQUIRE(strings[1] == "One");
        REQUIRE(strings[2] == "Two");
    });
}

TEST_CASE("Encode array in object", "[Encoding]")
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
        decoder >> beginObject()
                >> beginArray("Array");

        std::vector<std::string> strings;
        while (decoder.hasMoreElements())
        {
            std::string string;
            decoder >> decodeValue(string);
            strings.push_back(string);
        }
        decoder >> endArray()
                >> endObject();

        REQUIRE(strings[0] == "Zero");
        REQUIRE(strings[1] == "One");
        REQUIRE(strings[2] == "Two");
    });
}

TEST_CASE("Encode array in array", "[Encoding]")
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
        decoder >> beginArray();

        int arrayCount = 0;
        while (decoder.hasMoreElements())
        {
            decoder >> beginArray();

            std::vector<std::string> strings;
            while (decoder.hasMoreElements())
            {
                std::string string;
                decoder >> decodeValue(string);
                strings.push_back(string);
            }

            REQUIRE(strings[0] == "Zero");
            REQUIRE(strings[1] == "One");
            REQUIRE(strings[2] == "Two");

            ++arrayCount;
            decoder >> endArray();
        }
        decoder >> endArray();

        REQUIRE(arrayCount == 3);
    });
}

TEST_CASE("Encode object in array", "[Encoding]")
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
        decoder >> beginArray();

        int objectCount = 0;
        while (decoder.hasMoreElements())
        {
            std::string string;

            decoder >> beginObject()
                    >> decodeValue("String", string)
                    >> endObject();

            REQUIRE(string== "Testing");

            ++objectCount;
        }

        decoder >> endArray();

        REQUIRE(objectCount == 3);
    });
}

TEST_CASE("Encode everything in array", "[Encoding]")
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
        decoder >> beginArray();

        REQUIRE(decoder.decodeString() == "Test");
        REQUIRE(decoder.hasMoreElements());
        REQUIRE(decoder.decodeInt8() == 12);
        REQUIRE(decoder.hasMoreElements());
        REQUIRE(decoder.decodeUInt8() == 12u);
        REQUIRE(decoder.hasMoreElements());
        REQUIRE(decoder.decodeInt16() == 12);
        REQUIRE(decoder.hasMoreElements());
        REQUIRE(decoder.decodeUInt16() == 12u);
        REQUIRE(decoder.hasMoreElements());
        REQUIRE(decoder.decodeInt32() == 12);
        REQUIRE(decoder.hasMoreElements());
        REQUIRE(decoder.decodeUInt32() == 12u);
        REQUIRE(decoder.hasMoreElements());
        REQUIRE(decoder.decodeInt64() == 12);
        REQUIRE(decoder.hasMoreElements());
        REQUIRE(decoder.decodeUInt64() == 12u);
        REQUIRE(decoder.hasMoreElements());
        REQUIRE(decoder.decodeFloat32() == 123.0f);
        REQUIRE(decoder.hasMoreElements());
        REQUIRE(decoder.decodeFloat64() == 123.0);
        REQUIRE(decoder.hasMoreElements());
        REQUIRE(decoder.decodeBool() == true);
        REQUIRE(!decoder.hasMoreElements());
    });
}

TEST_CASE("Encode everything in object", "[Encoding]")
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
        decoder >> beginObject();

        {
            std::string value;
            decoder >> decodeValue("String", value);
            REQUIRE(value == "Test");
        }

        {
            int8_t value;
            decoder >> decodeValue("Int8", value);
            REQUIRE(value == 12);
        }

        {
            uint8_t value;
            decoder >> decodeValue("UInt8", value);
            REQUIRE(value == 12u);
        }

        {
            int16_t value;
            decoder >> decodeValue("Int16", value);
            REQUIRE(value == 12);
        }

        {
            uint16_t value;
            decoder >> decodeValue("UInt16", value);
            REQUIRE(value == 12u);
        }

        {
            int32_t value;
            decoder >> decodeValue("Int32", value);
            REQUIRE(value == 12);
        }

        {
            uint32_t value;
            decoder >> decodeValue("UInt32", value);
            REQUIRE(value == 12u);
        }

        {
            int64_t value;
            decoder >> decodeValue("Int64", value);
            REQUIRE(value == 12);
        }

        {
            uint64_t value;
            decoder >> decodeValue("UInt64", value);
            REQUIRE(value == 12u);
        }

        {
            float value;
            decoder >> decodeValue("Float32", value);
            REQUIRE(value == 123.0f);
        }

        {
            double value;
            decoder >> decodeValue("Float64", value);
            REQUIRE(value == 123.0);
        }

        {
            bool value;
            decoder >> decodeValue("Bool", value);
            REQUIRE(value == true);
        }

        decoder >> endObject();
    });
}