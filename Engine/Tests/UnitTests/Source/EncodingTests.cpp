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
#include <Hect/IO/Encoder.h>
#include <Hect/IO/Decoder.h>
using namespace hect;

#include <catch.hpp>
#include <functional>

#include "Utilities.h"

TEST_CASE("Encode single object", "[Encoding]")
{
    test_encode_and_decode([](Encoder& encoder)
    {
        encoder << begin_object()
                << encode_value("String", std::string("Testing"))
                << end_object();
    }, [](Decoder& decoder)
    {
        std::string string;

        decoder >> begin_object()
                >> decode_value("String", string)
                >> end_object();

        REQUIRE(string == "Testing");
    });
}

TEST_CASE("Encode single array", "[Encoding]")
{
    test_encode_and_decode([](Encoder& encoder)
    {
        encoder << begin_array()
                << encode_value(std::string("Zero"))
                << encode_value(std::string("One"))
                << encode_value(std::string("Two"))
                << end_array();
    }, [](Decoder& decoder)
    {
        std::vector<std::string> strings;

        decoder >> begin_array();
        while (decoder.has_more_elements())
        {
            std::string string;
            decoder >> decode_value(string);
            strings.push_back(string);
        }
        decoder >> end_array();

        REQUIRE(strings[0] == "Zero");
        REQUIRE(strings[1] == "One");
        REQUIRE(strings[2] == "Two");
    });
}

TEST_CASE("Encode array in object", "[Encoding]")
{
    test_encode_and_decode([](Encoder& encoder)
    {
        encoder << begin_object()
                << begin_array("Array")
                << encode_value(std::string("Zero"))
                << encode_value(std::string("One"))
                << encode_value(std::string("Two"))
                << end_array()
                << end_object();
    }, [](Decoder& decoder)
    {
        decoder >> begin_object()
                >> begin_array("Array");

        std::vector<std::string> strings;
        while (decoder.has_more_elements())
        {
            std::string string;
            decoder >> decode_value(string);
            strings.push_back(string);
        }
        decoder >> end_array()
                >> end_object();

        REQUIRE(strings[0] == "Zero");
        REQUIRE(strings[1] == "One");
        REQUIRE(strings[2] == "Two");
    });
}

TEST_CASE("Encode array in array", "[Encoding]")
{
    test_encode_and_decode([](Encoder& encoder)
    {
        encoder << begin_array();
        for (int i = 0; i < 3; ++i)
        {
            encoder << begin_array()
                    << encode_value(std::string("Zero"))
                    << encode_value(std::string("One"))
                    << encode_value(std::string("Two"))
                    << end_array();
        }
        encoder << end_array();
    }, [](Decoder& decoder)
    {
        decoder >> begin_array();

        int array_count = 0;
        while (decoder.has_more_elements())
        {
            decoder >> begin_array();

            std::vector<std::string> strings;
            while (decoder.has_more_elements())
            {
                std::string string;
                decoder >> decode_value(string);
                strings.push_back(string);
            }

            REQUIRE(strings[0] == "Zero");
            REQUIRE(strings[1] == "One");
            REQUIRE(strings[2] == "Two");

            ++array_count;
            decoder >> end_array();
        }
        decoder >> end_array();

        REQUIRE(array_count == 3);
    });
}

TEST_CASE("Encode object in array", "[Encoding]")
{
    test_encode_and_decode([](Encoder& encoder)
    {
        encoder << begin_array();
        for (int i = 0; i < 3; ++i)
        {
            encoder << begin_object()
                    << encode_value("String", std::string("Testing"))
                    << end_object();
        }
        encoder << end_array();
    }, [](Decoder& decoder)
    {
        decoder >> begin_array();

        int object_count = 0;
        while (decoder.has_more_elements())
        {
            std::string string;

            decoder >> begin_object()
                    >> decode_value("String", string)
                    >> end_object();

            REQUIRE(string== "Testing");

            ++object_count;
        }

        decoder >> end_array();

        REQUIRE(object_count == 3);
    });
}

TEST_CASE("Encode everything in array", "[Encoding]")
{
    test_encode_and_decode([](Encoder& encoder)
    {
        encoder << begin_array()
                << encode_value(std::string("Test"))
                << encode_value(static_cast<int8_t>(12))
                << encode_value(static_cast<uint8_t>(12))
                << encode_value(static_cast<int16_t>(12))
                << encode_value(static_cast<uint16_t>(12))
                << encode_value(static_cast<int32_t>(12))
                << encode_value(static_cast<uint32_t>(12))
                << encode_value(static_cast<int64_t>(12))
                << encode_value(static_cast<uint64_t>(12))
                << encode_value(123.0f)
                << encode_value(123.0)
                << encode_value(true)
                << end_array();
    }, [](Decoder& decoder)
    {
        decoder >> begin_array();

        REQUIRE(decoder.decode_string() == "Test");
        REQUIRE(decoder.has_more_elements());
        REQUIRE(decoder.decode_int8() == 12);
        REQUIRE(decoder.has_more_elements());
        REQUIRE(decoder.decode_uint8() == 12u);
        REQUIRE(decoder.has_more_elements());
        REQUIRE(decoder.decode_int16() == 12);
        REQUIRE(decoder.has_more_elements());
        REQUIRE(decoder.decode_uint16() == 12u);
        REQUIRE(decoder.has_more_elements());
        REQUIRE(decoder.decode_int32() == 12);
        REQUIRE(decoder.has_more_elements());
        REQUIRE(decoder.decode_uint32() == 12u);
        REQUIRE(decoder.has_more_elements());
        REQUIRE(decoder.decode_int64() == 12);
        REQUIRE(decoder.has_more_elements());
        REQUIRE(decoder.decode_uint64() == 12u);
        REQUIRE(decoder.has_more_elements());
        REQUIRE(decoder.decode_float32() == 123.0f);
        REQUIRE(decoder.has_more_elements());
        REQUIRE(decoder.decode_float64() == 123.0);
        REQUIRE(decoder.has_more_elements());
        REQUIRE(decoder.decode_bool() == true);
        REQUIRE(!decoder.has_more_elements());
    });
}

TEST_CASE("Encode everything in object", "[Encoding]")
{
    test_encode_and_decode([](Encoder& encoder)
    {
        encoder << begin_object()
                << encode_value("String", std::string("Test"))
                << encode_value("Int8", static_cast<int8_t>(12))
                << encode_value("UInt8", static_cast<uint8_t>(12))
                << encode_value("Int16", static_cast<int16_t>(12))
                << encode_value("UInt16", static_cast<uint16_t>(12))
                << encode_value("Int32", static_cast<int32_t>(12))
                << encode_value("UInt32", static_cast<uint32_t>(12))
                << encode_value("Int64", static_cast<int64_t>(12))
                << encode_value("UInt64", static_cast<uint64_t>(12))
                << encode_value("Float32", 123.0f)
                << encode_value("Float64", 123.0)
                << encode_value("Bool", true)
                << end_object();
    }, [](Decoder& decoder)
    {
        decoder >> begin_object();

        {
            std::string value;
            decoder >> decode_value("String", value);
            REQUIRE(value == "Test");
        }

        {
            int8_t value;
            decoder >> decode_value("Int8", value);
            REQUIRE(value == 12);
        }

        {
            uint8_t value;
            decoder >> decode_value("UInt8", value);
            REQUIRE(value == 12u);
        }

        {
            int16_t value;
            decoder >> decode_value("Int16", value);
            REQUIRE(value == 12);
        }

        {
            uint16_t value;
            decoder >> decode_value("UInt16", value);
            REQUIRE(value == 12u);
        }

        {
            int32_t value;
            decoder >> decode_value("Int32", value);
            REQUIRE(value == 12);
        }

        {
            uint32_t value;
            decoder >> decode_value("UInt32", value);
            REQUIRE(value == 12u);
        }

        {
            int64_t value;
            decoder >> decode_value("Int64", value);
            REQUIRE(value == 12);
        }

        {
            uint64_t value;
            decoder >> decode_value("UInt64", value);
            REQUIRE(value == 12u);
        }

        {
            float value;
            decoder >> decode_value("Float32", value);
            REQUIRE(value == 123.0f);
        }

        {
            double value;
            decoder >> decode_value("Float64", value);
            REQUIRE(value == 123.0);
        }

        {
            bool value;
            decoder >> decode_value("Bool", value);
            REQUIRE(value == true);
        }

        decoder >> end_object();
    });
}