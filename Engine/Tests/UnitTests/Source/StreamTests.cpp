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
#include <Hect/Core/Exception.h>
#include <Hect/IO/FileSystem.h>
#include <Hect/IO/MemoryReadStream.h>
#include <Hect/IO/MemoryWriteStream.h>
#include <Hect/Math/Constants.h>
using namespace hect;

#include <functional>

#include <catch.hpp>

void test_write_and_read_stream(std::function<void(WriteStream&)> write, std::function<void(ReadStream&)> read)
{
    // Memory streams
    std::vector<uint8_t> data;
    {
        MemoryWriteStream stream(data);
        write(stream);
    }
    {
        MemoryReadStream stream(data);
        read(stream);
    }
}

TEST_CASE("Write and read passed the end of a stream", "[Stream]")
{
    test_write_and_read_stream([](WriteStream& stream)
    {
        stream << "Testing";
    }, [](ReadStream& stream)
    {
        bool error_occurred = false;

        try
        {
            uint8_t data[16];
            stream.read(data, 16);
        }
        catch (Exception&)
        {
            error_occurred = true;
        }

        REQUIRE(error_occurred);
    });
}

TEST_CASE("Write and read a string to and from a stream", "[Stream]")
{
    test_write_and_read_stream([](WriteStream& stream)
    {
        stream << "Testing";
    }, [](ReadStream& stream)
    {
        std::string value;
        stream >> value;
        REQUIRE(value == "Testing");
        REQUIRE(stream.end_of_stream());
    });
}

TEST_CASE("Write and read a signed 8-bit integer to and from a stream", "[Stream]")
{
    test_write_and_read_stream([](WriteStream& stream)
    {
        int8_t value = 123;
        stream << value;
    }, [](ReadStream& stream)
    {
        int8_t value;
        stream >> value;
        REQUIRE(value == 123);
        REQUIRE(stream.end_of_stream());
    });
}

TEST_CASE("Write and read an unsigned 8-bit integer to and from a stream", "[Stream]")
{
    test_write_and_read_stream([](WriteStream& stream)
    {
        uint8_t value = 123;
        stream << value;
    }, [](ReadStream& stream)
    {
        uint8_t value;
        stream >> value;
        REQUIRE(value == 123);
        REQUIRE(stream.end_of_stream());
    });
}

TEST_CASE("Write and read a signed 16-bit integer to and from a stream", "[Stream]")
{
    test_write_and_read_stream([](WriteStream& stream)
    {
        int16_t value = 123;
        stream << value;
    }, [](ReadStream& stream)
    {
        int16_t value;
        stream >> value;
        REQUIRE(value == 123);
        REQUIRE(stream.end_of_stream());
    });
}

TEST_CASE("Write and read an unsigned 16-bit integer to and from a stream", "[Stream]")
{
    test_write_and_read_stream([](WriteStream& stream)
    {
        uint16_t value = 123;
        stream << value;
    }, [](ReadStream& stream)
    {
        uint16_t value;
        stream >> value;
        REQUIRE(value == 123);
        REQUIRE(stream.end_of_stream());
    });
}

TEST_CASE("Write and read a signed 32-bit integer to and from a stream", "[Stream]")
{
    test_write_and_read_stream([](WriteStream& stream)
    {
        int32_t value = 123;
        stream << value;
    }, [](ReadStream& stream)
    {
        int32_t value;
        stream >> value;
        REQUIRE(value == 123);
        REQUIRE(stream.end_of_stream());
    });
}

TEST_CASE("Write and read an unsigned 32-bit integer to and from a stream", "[Stream]")
{
    test_write_and_read_stream([](WriteStream& stream)
    {
        uint32_t value = 123;
        stream << value;
    }, [](ReadStream& stream)
    {
        uint32_t value;
        stream >> value;
        REQUIRE(value == 123u);
        REQUIRE(stream.end_of_stream());
    });
}

TEST_CASE("Write and read a signed 64-bit integer to and from a stream", "[Stream]")
{
    test_write_and_read_stream([](WriteStream& stream)
    {
        int64_t value = 123456789;
        stream << value;
    }, [](ReadStream& stream)
    {
        int64_t value;
        stream >> value;
        REQUIRE(value == 123456789);
        REQUIRE(stream.end_of_stream());
    });
}

TEST_CASE("Write and read an unsigned 64-bit integer to and from a stream", "[Stream]")
{
    test_write_and_read_stream([](WriteStream& stream)
    {
        uint64_t value = 123456789;
        stream << value;
    }, [](ReadStream& stream)
    {
        uint64_t value;
        stream >> value;
        REQUIRE(value == 123456789);
        REQUIRE(stream.end_of_stream());
    });
}

TEST_CASE("Write and read a 32-bit float to and from a stream", "[Stream]")
{
    test_write_and_read_stream([](WriteStream& stream)
    {
        float value = static_cast<float>(Pi);
        stream << value;
    }, [](ReadStream& stream)
    {
        float value;
        stream >> value;
        REQUIRE(std::abs(value - Pi) < 0.01);
        REQUIRE(stream.end_of_stream());
    });
}

TEST_CASE("Write and read a 64-bit float to and from a stream", "[Stream]")
{
    test_write_and_read_stream([](WriteStream& stream)
    {
        double value = Pi;
        stream << value;
    }, [](ReadStream& stream)
    {
        double value;
        stream >> value;
        REQUIRE(std::abs(value - Pi) < 0.01);
        REQUIRE(stream.end_of_stream());
    });
}

TEST_CASE("Write and read a bool to and from a stream", "[Stream]")
{
    test_write_and_read_stream([](WriteStream& stream)
    {
        stream << true << false;
    }, [](ReadStream& stream)
    {
        bool true_value;
        bool false_value;
        stream >> true_value >> false_value;
        REQUIRE(true_value);
        REQUIRE(!false_value);
        REQUIRE(stream.end_of_stream());
    });
}

TEST_CASE("Seek on a write stream", "[Stream]")
{
    test_write_and_read_stream([](WriteStream& stream)
    {
        uint32_t one = 1;
        uint32_t two = 2;
        uint32_t three = 3;

        size_t position = stream.position();
        stream << three;
        stream << two;
        stream.seek(position);
        stream << one;
    }, [](ReadStream& stream)
    {
        uint32_t first_value;
        uint32_t second_value;
        stream >> first_value >> second_value;

        REQUIRE(first_value == 1u);
        REQUIRE(second_value == 2u);
        REQUIRE(stream.end_of_stream());
    });
}