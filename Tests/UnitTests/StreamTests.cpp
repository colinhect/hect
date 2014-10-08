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
#include <Hect/Core/Error.h>
#include <Hect/IO/MemoryReadStream.h>
#include <Hect/IO/MemoryWriteStream.h>
#include <Hect/Math/Constants.h>
#include "Hect/Platform/FileSystem.h"
using namespace hect;

#include <functional>

#include <catch.hpp>

void testWriteAndReadStream(std::function<void(WriteStream&)> write, std::function<void(ReadStream&)> read)
{
    // Memory streams
    {
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

    // File streams
    {
        FileSystem::initialize();

        Path workingDirectory = FileSystem::workingDirectory();
        FileSystem::mountArchive(workingDirectory);
        FileSystem::setWriteDirectory(workingDirectory);

        Path path("File");

        REQUIRE(!FileSystem::exists(path));
        {
            WriteStream stream = FileSystem::openFileForWrite(path);
            write(stream);
        }
        REQUIRE(FileSystem::exists(path));
        {
            ReadStream stream = FileSystem::openFileForRead(path);
            read(stream);
        }
        FileSystem::remove(path);
        REQUIRE(!FileSystem::exists(path));

        FileSystem::deinitialize();
    }
}

TEST_CASE("Stream_WriteAndReadPastEndOfStream")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream << "Testing";
    }, [](ReadStream& stream)
    {
        bool errorOccurred = false;

        try
        {
            uint8_t data[16];
            stream.read(data, 16);
        }
        catch (Error&)
        {
            errorOccurred = true;
        }

        REQUIRE(errorOccurred);
    });
}

TEST_CASE("Stream_WriteAndReadString")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream << "Testing";
    }, [](ReadStream& stream)
    {
        std::string value;
        stream >> value;
        REQUIRE(value == "Testing");
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadInt8")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        int8_t value = 123;
        stream << value;
    }, [](ReadStream& stream)
    {
        int8_t value;
        stream >> value;
        REQUIRE(value == 123);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadUInt8")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        uint8_t value = 123;
        stream << value;
    }, [](ReadStream& stream)
    {
        uint8_t value;
        stream >> value;
        REQUIRE(value == 123);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadInt16")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        int16_t value = 123;
        stream << value;
    }, [](ReadStream& stream)
    {
        int16_t value;
        stream >> value;
        REQUIRE(value == 123);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadUInt16")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        uint16_t value = 123;
        stream << value;
    }, [](ReadStream& stream)
    {
        uint16_t value;
        stream >> value;
        REQUIRE(value == 123);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadInt32")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        int32_t value = 123;
        stream << value;
    }, [](ReadStream& stream)
    {
        int32_t value;
        stream >> value;
        REQUIRE(value == 123);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadUInt32")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        uint32_t value = 123;
        stream << value;
    }, [](ReadStream& stream)
    {
        uint32_t value;
        stream >> value;
        REQUIRE(value == 123u);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadInt64")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        int64_t value = 123456789;
        stream << value;
    }, [](ReadStream& stream)
    {
        int64_t value;
        stream >> value;
        REQUIRE(value == 123456789);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadUInt64")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        uint64_t value = 123456789;
        stream << value;
    }, [](ReadStream& stream)
    {
        uint64_t value;
        stream >> value;
        REQUIRE(value == 123456789);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadFloat32")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        float value = static_cast<float>(pi);
        stream << value;
    }, [](ReadStream& stream)
    {
        float value;
        stream >> value;
        REQUIRE(std::abs(value - pi) < 0.01);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadFloat64")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        double value = pi;
        stream << value;
    }, [](ReadStream& stream)
    {
        double value;
        stream >> value;
        REQUIRE(std::abs(value - pi) < 0.01);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadBool")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream << true << false;
    }, [](ReadStream& stream)
    {
        bool trueValue;
        bool falseValue;
        stream >> trueValue >> falseValue;
        REQUIRE(trueValue);
        REQUIRE(!falseValue);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_SeekOnWrite")
{
    testWriteAndReadStream([](WriteStream& stream)
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
        uint32_t firstValue;
        uint32_t secondValue;
        stream >> firstValue >> secondValue;

        REQUIRE(firstValue == 1u);
        REQUIRE(secondValue == 2u);
        REQUIRE(stream.endOfStream());
    });
}