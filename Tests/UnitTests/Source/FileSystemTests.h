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
void testWriteAndReadFile(std::function<void(WriteStream*)> writer, std::function<void(ReadStream*)> reader)
{
    FileSystem fileSystem;
    Path workingDirectory = fileSystem.workingDirectory();
    fileSystem.addDataSource(workingDirectory);
    fileSystem.setWriteDirectory(workingDirectory);

    Path path("File.txt");

    CHECK(!fileSystem.exists(path));
    {
        FileWriteStream stream = fileSystem.openFileForWrite(path);
        writer(&stream);
    }
    CHECK(fileSystem.exists(path));
    {
        FileReadStream stream = fileSystem.openFileForRead(path);
        reader(&stream);
    }
    fileSystem.remove(path);
    CHECK(!fileSystem.exists(path));
}

SUITE(FileSystem)
{
    TEST(CreateAndRemoveDirectories)
    {
        FileSystem fileSystem;
        Path workingDirectory = fileSystem.workingDirectory();
        fileSystem.addDataSource(workingDirectory);
        fileSystem.setWriteDirectory(workingDirectory);

        Path path("Directory");

        CHECK(!fileSystem.exists(path));
        fileSystem.createDirectory(path);
        CHECK(fileSystem.exists(path));
        fileSystem.remove(path);
        CHECK(!fileSystem.exists(path));
    }

    TEST(OpenNonExistingFileForWrite)
    {
        FileSystem fileSystem;
        Path workingDirectory = fileSystem.workingDirectory();
        fileSystem.addDataSource(workingDirectory);
        fileSystem.setWriteDirectory(workingDirectory);

        Path path("File.txt");

        CHECK(!fileSystem.exists(path));
        {
            FileWriteStream stream = fileSystem.openFileForWrite(path);
        }
        CHECK(fileSystem.exists(path));
        fileSystem.remove(path);
        CHECK(!fileSystem.exists(path));
    }

    TEST(OpenExistingFileForWrite)
    {
        FileSystem fileSystem;
        Path workingDirectory = fileSystem.workingDirectory();
        fileSystem.addDataSource(workingDirectory);
        fileSystem.setWriteDirectory(workingDirectory);

        Path path("File.txt");

        CHECK(!fileSystem.exists(path));
        {
            FileWriteStream stream = fileSystem.openFileForWrite(path);
        }
        CHECK(fileSystem.exists(path));
        {
            FileWriteStream stream = fileSystem.openFileForWrite(path);
        }
        CHECK(fileSystem.exists(path));
        fileSystem.remove(path);
        CHECK(!fileSystem.exists(path));
    }

    TEST(OpenExistingFileForRead)
    {
        FileSystem fileSystem;
        Path workingDirectory = fileSystem.workingDirectory();
        fileSystem.addDataSource(workingDirectory);
        fileSystem.setWriteDirectory(workingDirectory);

        Path path("File.txt");

        CHECK(!fileSystem.exists(path));
        {
            FileWriteStream stream = fileSystem.openFileForWrite(path);
        }
        CHECK(fileSystem.exists(path));
        {
            FileReadStream stream = fileSystem.openFileForRead(path);
        }

        fileSystem.remove(path);
        CHECK(!fileSystem.exists(path));
    }

    TEST(OpenNonExistingFileForRead)
    {
        FileSystem fileSystem;
        Path workingDirectory = fileSystem.workingDirectory();
        fileSystem.addDataSource(workingDirectory);
        fileSystem.setWriteDirectory(workingDirectory);

        Path path("DoesNotExist.txt");

        bool errorOccurred = false;

        try
        {
            FileReadStream stream = fileSystem.openFileForRead(path);
        }
        catch (Error&)
        {
            errorOccurred = true;
        }

        CHECK(errorOccurred);
    }

    TEST(FileWriteAndReadPastEndOfStream)
    {
        testWriteAndReadFile([] (WriteStream* stream)
        {
            stream->writeString("Testing");
        }, [] (ReadStream* stream)
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

            CHECK(errorOccurred);
        });
    }

    TEST(WriteAndReadString)
    {
        testWriteAndReadFile([] (WriteStream* stream)
        {
            stream->writeString("Testing");
        }, [] (ReadStream* stream)
        {
            std::string string = stream->readString();
            CHECK_EQUAL("Testing", string);
            CHECK(stream->endOfStream());
        });
    }

    TEST(WriteAndReadByte)
    {
        testWriteAndReadFile([] (WriteStream* stream)
        {
            stream->writeByte(123);
        }, [] (ReadStream* stream)
        {
            int8_t value = stream->readByte();
            CHECK_EQUAL(123, value);
            CHECK(stream->endOfStream());
        });
    }

    TEST(WriteAndReadUnsignedByte)
    {
        testWriteAndReadFile([] (WriteStream* stream)
        {
            stream->writeUnsignedByte(123);
        }, [] (ReadStream* stream)
        {
            uint8_t value = stream->readUnsignedByte();
            CHECK_EQUAL(123, value);
            CHECK(stream->endOfStream());
        });
    }

    TEST(WriteAndReadShort)
    {
        testWriteAndReadFile([] (WriteStream* stream)
        {
            stream->writeShort(123);
        }, [] (ReadStream* stream)
        {
            int16_t value = stream->readShort();
            CHECK_EQUAL(123, value);
            CHECK(stream->endOfStream());
        });
    }

    TEST(WriteAndReadUnsignedShort)
    {
        testWriteAndReadFile([] (WriteStream* stream)
        {
            stream->writeUnsignedShort(123);
        }, [] (ReadStream* stream)
        {
            int16_t value = stream->readUnsignedShort();
            CHECK_EQUAL(123, value);
            CHECK(stream->endOfStream());
        });
    }

    TEST(WriteAndReadInt)
    {
        testWriteAndReadFile([] (WriteStream* stream)
        {
            stream->writeInt(123);
        }, [] (ReadStream* stream)
        {
            int32_t value = stream->readInt();
            CHECK_EQUAL(123, value);
            CHECK(stream->endOfStream());
        });
    }

    TEST(WriteAndReadUnsignedInt)
    {
        testWriteAndReadFile([] (WriteStream* stream)
        {
            stream->writeUnsignedInt(123);
        }, [] (ReadStream* stream)
        {
            uint32_t value = stream->readUnsignedInt();
            CHECK_EQUAL(123u, value);
            CHECK(stream->endOfStream());
        });
    }

    TEST(WriteAndReadLong)
    {
        testWriteAndReadFile([] (WriteStream* stream)
        {
            stream->writeLong(123456789);
        }, [] (ReadStream* stream)
        {
            int64_t value = stream->readLong();
            CHECK_EQUAL(123456789, value);
            CHECK(stream->endOfStream());
        });
    }

    TEST(WriteAndReadUnsignedLong)
    {
        testWriteAndReadFile([] (WriteStream* stream)
        {
            stream->writeUnsignedLong(123456789);
        }, [] (ReadStream* stream)
        {
            uint64_t value = stream->readUnsignedLong();
            CHECK_EQUAL(123456789, value);
            CHECK(stream->endOfStream());
        });
    }

    TEST(WriteAndReadFloat)
    {
        testWriteAndReadFile([] (WriteStream* stream)
        {
            stream->writeFloat((float)pi);
        }, [] (ReadStream* stream)
        {
            float value = stream->readFloat();
            CHECK_CLOSE(pi, value, epsilon);
            CHECK(stream->endOfStream());
        });
    }

    TEST(WriteAndReadDouble)
    {
        testWriteAndReadFile([] (WriteStream* stream)
        {
            stream->writeDouble(pi);
        }, [] (ReadStream* stream)
        {
            double value = stream->readDouble();
            CHECK_CLOSE(pi, value, epsilon);
            CHECK(stream->endOfStream());
        });
    }

    TEST(SeekOnWrite)
    {
        testWriteAndReadFile([] (WriteStream* stream)
        {
            size_t position = stream->position();
            stream->writeUnsignedInt(3);
            stream->writeUnsignedInt(2);
            stream->seek(position);
            stream->writeUnsignedInt(1);
        }, [] (ReadStream* stream)
        {
            CHECK_EQUAL(1u, stream->readUnsignedInt());
            CHECK_EQUAL(2u, stream->readUnsignedInt());
            CHECK(stream->endOfStream());
        });
    }
}