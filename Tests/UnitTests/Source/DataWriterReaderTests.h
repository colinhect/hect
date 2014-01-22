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

void testWriteAndRead(std::function<void(DataWriter&)> save, std::function<void(DataReader&)> load)
{
    // DataValueWriter/DataReader
    {
        DataValue dataValue;
        {
            DataValueWriter writer;
            save(writer);
            dataValue = writer.dataValues()[0];
        }
        {
            DataValueReader reader(dataValue);
            load(reader);
        }
    }

    // BinaryWriter/DataReader
    {
        std::vector<uint8_t> data;
        {
            MemoryWriteStream stream(data);
            BinaryWriter writer(stream);
            save(writer);
        }
        {
            MemoryReadStream stream(data);
            BinaryReader reader(stream);
            load(reader);
        }
    }
}

SUITE(WriterTests)
{
    TEST(SingleObject)
    {
        testWriteAndRead([] (DataWriter& writer)
        {
            ObjectWriter object = writer.writeObject();
            object.writeString("String", "Testing");
        }, [] (DataReader& reader)
        {
            ObjectReader object = reader.readObject();

            CHECK(object.hasMember("String"));
            CHECK_EQUAL("Testing", object.readString("String"));
        });
    }

    TEST(SingleArray)
    {
        testWriteAndRead([] (DataWriter& writer)
        {
            ArrayWriter array = writer.writeArray();
            array.writeString("Zero");
            array.writeString("One");
            array.writeString("Two");
        }, [] (DataReader& reader)
        {
            ArrayReader array = reader.readArray();

            std::vector<std::string> strings;
            while (array.hasMoreElements())
            {
                strings.push_back(array.readString());
            }

            CHECK_EQUAL("Zero", strings[0]);
            CHECK_EQUAL("One", strings[1]);
            CHECK_EQUAL("Two", strings[2]);
        });
    }

    TEST(ArrayInObject)
    {
        testWriteAndRead([] (DataWriter& writer)
        {
            ObjectWriter object = writer.writeObject();
            ArrayWriter array = object.writeArray("Array");
            array.writeString("Zero");
            array.writeString("One");
            array.writeString("Two");
        }, [] (DataReader& reader)
        {
            ObjectReader object = reader.readObject();

            CHECK(object.hasMember("Array"));

            ArrayReader array = object.readArray("Array");

            std::vector<std::string> strings;
            while (array.hasMoreElements())
            {
                strings.push_back(array.readString());
            }

            CHECK_EQUAL("Zero", strings[0]);
            CHECK_EQUAL("One", strings[1]);
            CHECK_EQUAL("Two", strings[2]);
        });
    }

    TEST(ArrayInArray)
    {
        testWriteAndRead([] (DataWriter& writer)
        {
            ArrayWriter array = writer.writeArray();

            for (int i = 0; i < 3; ++i)
            {
                ArrayWriter nested = array.writeArray();
                nested.writeString("Zero");
                nested.writeString("One");
                nested.writeString("Two");
            }
        }, [] (DataReader& reader)
        {
            ArrayReader array = reader.readArray();

            int arrayCount = 0;
            while (array.hasMoreElements())
            {
                ArrayReader nested = array.readArray();

                std::vector<std::string> strings;
                while (nested.hasMoreElements())
                {
                    strings.push_back(nested.readString());
                }

                CHECK_EQUAL("Zero", strings[0]);
                CHECK_EQUAL("One", strings[1]);
                CHECK_EQUAL("Two", strings[2]);

                ++arrayCount;
            }

            CHECK_EQUAL(3, arrayCount);
        });
    }

    TEST(ObjectInArray)
    {
        testWriteAndRead([] (DataWriter& writer)
        {
            ArrayWriter array = writer.writeArray();

            for (int i = 0; i < 3; ++i)
            {
                ObjectWriter object = array.writeObject();
                object.writeString("String", "Testing");
            }
        }, [] (DataReader& reader)
        {
            ArrayReader array = reader.readArray();

            int objectCount = 0;
            while (array.hasMoreElements())
            {
                ObjectReader object = array.readObject();

                CHECK(object.hasMember("String"));
                CHECK_EQUAL("Testing", object.readString("String"));

                ++objectCount;
            }

            CHECK_EQUAL(3, objectCount);
        });
    }

    TEST(AllInArray)
    {
        testWriteAndRead([] (DataWriter& writer)
        {
            ArrayWriter array = writer.writeArray();

            array.writeString("Test");
            array.writeByte(12);
            array.writeUnsignedByte(12);
            array.writeShort(12);
            array.writeUnsignedShort(12);
            array.writeInt(12);
            array.writeUnsignedInt(12);
            array.writeLong(12);
            array.writeUnsignedLong(12);
            array.writeFloat(123.0f);
            array.writeDouble(123.0);
            array.writeReal(123);
            array.writeBool(true);
            array.writeVector2(Vector2(1, 2));
            array.writeVector3(Vector3(1, 2, 3));
            array.writeVector4(Vector4(1, 2, 3, 4));
        }, [] (DataReader& reader)
        {
            ArrayReader array = reader.readArray();

            CHECK_EQUAL("Test", array.readString());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12, array.readByte());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12u, array.readUnsignedByte());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12, array.readShort());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12u, array.readUnsignedShort());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12, array.readInt());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12u, array.readUnsignedInt());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12, array.readLong());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12u, array.readUnsignedLong());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(123.0f, array.readFloat());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(123.0, array.readDouble());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(123, array.readReal());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(true, array.readBool());
            CHECK(array.hasMoreElements());
            Vector2 v2 = array.readVector2();
            CHECK_EQUAL(1, v2.x);
            CHECK_EQUAL(2, v2.y);
            CHECK(array.hasMoreElements());
            Vector3 v3 = array.readVector3();
            CHECK_EQUAL(1, v3.x);
            CHECK_EQUAL(2, v3.y);
            CHECK_EQUAL(3, v3.z);
            CHECK(array.hasMoreElements());
            Vector4 v4 = array.readVector4();
            CHECK_EQUAL(1, v4.x);
            CHECK_EQUAL(2, v4.y);
            CHECK_EQUAL(3, v4.z);
            CHECK_EQUAL(4, v4.w);
            CHECK(!array.hasMoreElements());
        });
    }

    TEST(AllInObject)
    {
        testWriteAndRead([] (DataWriter& writer)
        {
            ObjectWriter object = writer.writeObject();

            object.writeString("String", "Test");
            object.writeByte("Byte", 12);
            object.writeUnsignedByte("UnsignedByte", 12);
            object.writeShort("Short", 12);
            object.writeUnsignedShort("UnsignedShort", 12);
            object.writeInt("Int", 12);
            object.writeUnsignedInt("UnsignedInt", 12);
            object.writeLong("Long", 12);
            object.writeUnsignedLong("UnsignedLong", 12);
            object.writeFloat("Float", 123.0f);
            object.writeDouble("Double", 123.0);
            object.writeReal("Real", 123);
            object.writeBool("Bool", true);
            object.writeVector2("Vector2", Vector2(1, 2));
            object.writeVector3("Vector3", Vector3(1, 2, 3));
            object.writeVector4("Vector4", Vector4(1, 2, 3, 4));
        }, [] (DataReader& reader)
        {
            ObjectReader object = reader.readObject();

            CHECK(object.hasMember("String"));
            CHECK_EQUAL("Test", object.readString("String"));
            CHECK(object.hasMember("Byte"));
            CHECK_EQUAL(12, object.readByte("Byte"));
            CHECK(object.hasMember("UnsignedByte"));
            CHECK_EQUAL(12u, object.readUnsignedByte("UnsignedByte"));
            CHECK(object.hasMember("Short"));
            CHECK_EQUAL(12, object.readShort("Short"));
            CHECK(object.hasMember("UnsignedShort"));
            CHECK_EQUAL(12u, object.readUnsignedShort("UnsignedShort"));
            CHECK(object.hasMember("Int"));
            CHECK_EQUAL(12, object.readInt("Int"));
            CHECK(object.hasMember("UnsignedInt"));
            CHECK_EQUAL(12u, object.readUnsignedInt("UnsignedInt"));
            CHECK(object.hasMember("Long"));
            CHECK_EQUAL(12, object.readLong("Long"));
            CHECK(object.hasMember("UnsignedLong"));
            CHECK_EQUAL(12u, object.readUnsignedLong("UnsignedLong"));
            CHECK(object.hasMember("Float"));
            CHECK_EQUAL(123.0f, object.readFloat("Float"));
            CHECK(object.hasMember("Double"));
            CHECK_EQUAL(123.0, object.readDouble("Double"));
            CHECK(object.hasMember("Real"));
            CHECK_EQUAL(123, object.readReal("Real"));
            CHECK(object.hasMember("Bool"));
            CHECK_EQUAL(true, object.readBool("Bool"));
            CHECK(object.hasMember("Vector2"));
            Vector2 v2 = object.readVector2("Vector2");
            CHECK_EQUAL(1, v2.x);
            CHECK_EQUAL(2, v2.y);
            CHECK(object.hasMember("Vector3"));
            Vector3 v3 = object.readVector3("Vector3");
            CHECK_EQUAL(1, v3.x);
            CHECK_EQUAL(2, v3.y);
            CHECK_EQUAL(3, v3.z);
            CHECK(object.hasMember("Vector4"));
            Vector4 v4 = object.readVector4("Vector4");
            CHECK_EQUAL(1, v4.x);
            CHECK_EQUAL(2, v4.y);
            CHECK_EQUAL(3, v4.z);
            CHECK_EQUAL(4, v4.w);
        });
    }
}