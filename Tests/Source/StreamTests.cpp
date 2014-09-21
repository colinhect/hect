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
        FileSystem::mount(workingDirectory);
        FileSystem::setWriteDirectory(workingDirectory);

        Path path("File");

        REQUIRE(!FileSystem::exists(path));
        {
            WriteStream::Pointer stream = FileSystem::openFileForWrite(path);
            write(*stream);
        }
        REQUIRE(FileSystem::exists(path));
        {
            ReadStream::Pointer stream = FileSystem::openFileForRead(path);
            read(*stream);
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
        stream.writeString("Testing");
    }, [](ReadStream& stream)
    {
        bool errorOccurred = false;

        try
        {
            uint8_t data[16];
            stream.readBytes(data, 16);
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
        stream.writeString("Testing");
    }, [](ReadStream& stream)
    {
        std::string string = stream.readString();
        REQUIRE(string == "Testing");
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadInt8")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream.writeInt8(123);
    }, [](ReadStream& stream)
    {
        int8_t value = stream.readInt8();
        REQUIRE(value == 123);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadUInt8")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream.writeUInt8(123);
    }, [](ReadStream& stream)
    {
        uint8_t value = stream.readUInt8();
        REQUIRE(value == 123);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadInt16")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream.writeInt16(123);
    }, [](ReadStream& stream)
    {
        int16_t value = stream.readInt16();
        REQUIRE(value == 123);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadUInt16")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream.writeUInt16(123);
    }, [](ReadStream& stream)
    {
        int16_t value = stream.readUInt16();
        REQUIRE(value == 123);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadInt32")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream.writeInt32(123);
    }, [](ReadStream& stream)
    {
        int32_t value = stream.readInt32();
        REQUIRE(value == 123);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadUInt32")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream.writeUInt32(123);
    }, [](ReadStream& stream)
    {
        uint32_t value = stream.readUInt32();
        REQUIRE(value == 123u);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadInt64")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream.writeInt64(123456789);
    }, [](ReadStream& stream)
    {
        int64_t value = stream.readInt64();
        REQUIRE(value == 123456789);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadUInt64")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream.writeUInt64(123456789);
    }, [](ReadStream& stream)
    {
        uint64_t value = stream.readUInt64();
        REQUIRE(value == 123456789);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadFloat32")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream.writeFloat32((float)pi);
    }, [](ReadStream& stream)
    {
        float value = stream.readFloat32();
        REQUIRE(std::abs(value - pi) < 0.01);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadFloat64")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream.writeFloat64(pi);
    }, [](ReadStream& stream)
    {
        double value = stream.readFloat64();
        REQUIRE(std::abs(value - pi) < 0.01);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadReal")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream.writeReal(pi);
    }, [](ReadStream& stream)
    {
        Real value = stream.readReal();
        REQUIRE(std::abs(value - pi) < 0.01);
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadBool")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        stream.writeBool(true);
        stream.writeBool(false);
    }, [](ReadStream& stream)
    {
        REQUIRE(stream.readBool());
        REQUIRE(!stream.readBool());
        REQUIRE(stream.endOfStream());
    });
}

TEST_CASE("Stream_SeekOnWrite")
{
    testWriteAndReadStream([](WriteStream& stream)
    {
        size_t position = stream.position();
        stream.writeUInt32(3);
        stream.writeUInt32(2);
        stream.seek(position);
        stream.writeUInt32(1);
    }, [](ReadStream& stream)
    {
        REQUIRE(stream.readUInt32() == 1u);
        REQUIRE(stream.readUInt32() == 2u);
        REQUIRE(stream.endOfStream());
    });
}