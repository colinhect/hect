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

void testSerializeDeserialize(std::function<void(Serializer&)> serialize, std::function<void(Deserializer&)> deserialize)
{
    // DataValueSerializer/Deserializer
    {
        DataValue dataValue;
        {
            DataValueSerializer serializer;
            serialize(serializer);
            dataValue = serializer.serializedDataValues()[0];
        }
        {
            DataValueDeserializer deserializer(dataValue);
            deserialize(deserializer);
        }
    }

    // BinarySerializer/Deserializer
    {
        std::vector<uint8_t> data;
        {
            MemoryWriteStream stream(data);
            BinarySerializer serializer(stream);
            serialize(serializer);
        }
        {
            MemoryReadStream stream(data);
            BinaryDeserializer deserializer(stream);
            deserialize(deserializer);
        }
    }
}

SUITE(SerializerTests)
{
    TEST(SingleObjectDataValue)
    {
        testSerializeDeserialize([] (Serializer& serializer)
        {
            ObjectSerializer object = serializer.writeObject();
            object.writeString("String", "Testing");
        }, [] (Deserializer& deserializer)
        {
            ObjectDeserializer object = deserializer.readObject();

            bool hasObject = object;
            CHECK(hasObject);

            CHECK(object.hasMember("String"));
            CHECK_EQUAL("Testing", object.readString("String"));
        });
    }

    TEST(SingleArrayDataValue)
    {
        testSerializeDeserialize([] (Serializer& serializer)
        {
            ArraySerializer array = serializer.writeArray();
            array.writeString("Zero");
            array.writeString("One");
            array.writeString("Two");
        }, [] (Deserializer& deserializer)
        {
            ArrayDeserializer array = deserializer.readArray();

            bool hasArray = array;
            CHECK(hasArray);

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

    TEST(ArrayInObjectDataValue)
    {
        testSerializeDeserialize([] (Serializer& serializer)
        {
            ObjectSerializer object = serializer.writeObject();
            ArraySerializer array = object.writeArray("Array");
            array.writeString("Zero");
            array.writeString("One");
            array.writeString("Two");
        }, [] (Deserializer& deserializer)
        {
            ObjectDeserializer object = deserializer.readObject();

            bool hasObject = object;
            CHECK(hasObject);

            CHECK(object.hasMember("Array"));

            ArrayDeserializer array = object.readArray("Array");

            bool hasArray = array;
            CHECK(hasArray);

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

    TEST(ArrayInArrayDataValue)
    {
        testSerializeDeserialize([] (Serializer& serializer)
        {
            ArraySerializer array = serializer.writeArray();

            for (int i = 0; i < 3; ++i)
            {
                ArraySerializer nested = array.writeArray();
                nested.writeString("Zero");
                nested.writeString("One");
                nested.writeString("Two");
            }
        }, [] (Deserializer& deserializer)
        {
            ArrayDeserializer array = deserializer.readArray();

            bool hasArray = array;
            CHECK(hasArray);

            int arrayCount = 0;
            while (array.hasMoreElements())
            {
                ArrayDeserializer nested = array.readArray();

                bool hasArray = nested;
                CHECK(hasArray);

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

    TEST(ObjectInArrayDataValue)
    {
        testSerializeDeserialize([] (Serializer& serializer)
        {
            ArraySerializer array = serializer.writeArray();

            for (int i = 0; i < 3; ++i)
            {
                ObjectSerializer object = array.writeObject();
                object.writeString("String", "Testing");
            }
        }, [] (Deserializer& deserializer)
        {
            ArrayDeserializer array = deserializer.readArray();

            bool hasArray = array;
            CHECK(hasArray);

            int objectCount = 0;
            while (array.hasMoreElements())
            {
                ObjectDeserializer object = array.readObject();

                bool hasObject = object;
                CHECK(hasObject);

                CHECK(object.hasMember("String"));
                CHECK_EQUAL("Testing", object.readString("String"));

                ++objectCount;
            }

            CHECK_EQUAL(3, objectCount);
        });
    }
}