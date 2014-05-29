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
#include <Hect/IO/FileSystem.h>
#include <Hect/IO/MemoryReadStream.h>
#include <Hect/IO/MemoryWriteStream.h>
#include <Hect/Math/Constants.h>
using namespace hect;

#include <functional>

#include <catch.hpp>

void testWriteAndReadStream(std::function<void(WriteStream*)> write, std::function<void(ReadStream*)> read)
{
    // Memory streams
    {
        std::vector<uint8_t> data;
        {
            MemoryWriteStream stream(data);
            write(&stream);
        }
        {
            MemoryReadStream stream(data);
            read(&stream);
        }
    }

    // File streams
    {
        FileSystem fileSystem;
        Path workingDirectory = fileSystem.workingDirectory();
        fileSystem.addDataSource(workingDirectory);
        fileSystem.setWriteDirectory(workingDirectory);

        Path path("File");

        REQUIRE(!fileSystem.exists(path));
        {
            FileWriteStream stream = fileSystem.openFileForWrite(path);
            write(&stream);
        }
        REQUIRE(fileSystem.exists(path));
        {
            FileReadStream stream = fileSystem.openFileForRead(path);
            read(&stream);
        }
        fileSystem.remove(path);
        REQUIRE(!fileSystem.exists(path));
    }
}

TEST_CASE("Stream_WriteAndReadPastEndOfStream")
{
    testWriteAndReadStream([](WriteStream* stream)
    {
        stream->writeString("Testing");
    }, [](ReadStream* stream)
    {
        bool errorOccurred = false;

        try
        {
            uint8_t data[16];
            stream->readBytes(data, 16);
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
    testWriteAndReadStream([](WriteStream* stream)
    {
        stream->writeString("Testing");
    }, [](ReadStream* stream)
    {
        std::string string = stream->readString();
        REQUIRE(string == "Testing");
        REQUIRE(stream->endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadByte")
{
    testWriteAndReadStream([](WriteStream* stream)
    {
        stream->writeByte(123);
    }, [](ReadStream* stream)
    {
        int8_t value = stream->readByte();
        REQUIRE(value == 123);
        REQUIRE(stream->endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadUnsignedByte")
{
    testWriteAndReadStream([](WriteStream* stream)
    {
        stream->writeUnsignedByte(123);
    }, [](ReadStream* stream)
    {
        uint8_t value = stream->readUnsignedByte();
        REQUIRE(value == 123);
        REQUIRE(stream->endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadShort")
{
    testWriteAndReadStream([](WriteStream* stream)
    {
        stream->writeShort(123);
    }, [](ReadStream* stream)
    {
        int16_t value = stream->readShort();
        REQUIRE(value == 123);
        REQUIRE(stream->endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadUnsignedShort")
{
    testWriteAndReadStream([](WriteStream* stream)
    {
        stream->writeUnsignedShort(123);
    }, [](ReadStream* stream)
    {
        int16_t value = stream->readUnsignedShort();
        REQUIRE(value == 123);
        REQUIRE(stream->endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadInt")
{
    testWriteAndReadStream([](WriteStream* stream)
    {
        stream->writeInt(123);
    }, [](ReadStream* stream)
    {
        int32_t value = stream->readInt();
        REQUIRE(value == 123);
        REQUIRE(stream->endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadUnsignedInt")
{
    testWriteAndReadStream([](WriteStream* stream)
    {
        stream->writeUnsignedInt(123);
    }, [](ReadStream* stream)
    {
        uint32_t value = stream->readUnsignedInt();
        REQUIRE(value == 123u);
        REQUIRE(stream->endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadLong")
{
    testWriteAndReadStream([](WriteStream* stream)
    {
        stream->writeLong(123456789);
    }, [](ReadStream* stream)
    {
        int64_t value = stream->readLong();
        REQUIRE(value == 123456789);
        REQUIRE(stream->endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadUnsignedLong")
{
    testWriteAndReadStream([](WriteStream* stream)
    {
        stream->writeUnsignedLong(123456789);
    }, [](ReadStream* stream)
    {
        uint64_t value = stream->readUnsignedLong();
        REQUIRE(value == 123456789);
        REQUIRE(stream->endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadFloat")
{
    testWriteAndReadStream([](WriteStream* stream)
    {
        stream->writeFloat((float)pi);
    }, [](ReadStream* stream)
    {
        float value = stream->readFloat();
        REQUIRE(std::abs(value - pi) < 0.01);
        REQUIRE(stream->endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadDouble")
{
    testWriteAndReadStream([](WriteStream* stream)
    {
        stream->writeDouble(pi);
    }, [](ReadStream* stream)
    {
        double value = stream->readDouble();
        REQUIRE(std::abs(value - pi) < 0.01);
        REQUIRE(stream->endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadReal")
{
    testWriteAndReadStream([](WriteStream* stream)
    {
        stream->writeReal(pi);
    }, [](ReadStream* stream)
    {
        Real value = stream->readReal();
        REQUIRE(std::abs(value - pi) < 0.01);
        REQUIRE(stream->endOfStream());
    });
}

TEST_CASE("Stream_WriteAndReadBool")
{
    testWriteAndReadStream([](WriteStream* stream)
    {
        stream->writeBool(true);
        stream->writeBool(false);
    }, [](ReadStream* stream)
    {
        REQUIRE(stream->readBool());
        REQUIRE(!stream->readBool());
        REQUIRE(stream->endOfStream());
    });
}

TEST_CASE("Stream_SeekOnWrite")
{
    testWriteAndReadStream([](WriteStream* stream)
    {
        size_t position = stream->position();
        stream->writeUnsignedInt(3);
        stream->writeUnsignedInt(2);
        stream->seek(position);
        stream->writeUnsignedInt(1);
    }, [](ReadStream* stream)
    {
        REQUIRE(stream->readUnsignedInt() == 1u);
        REQUIRE(stream->readUnsignedInt() == 2u);
        REQUIRE(stream->endOfStream());
    });
}