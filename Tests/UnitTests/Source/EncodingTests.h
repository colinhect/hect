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

void testEncodeAndDecode(std::function<void(Encoder&)> encode, std::function<void(Decoder&)> decode)
{
    // JSON
    {
        JsonValue jsonValue;
        {
            JsonEncoder encoder;
            encode(encoder);
            jsonValue = encoder.jsonValues()[0];
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

SUITE(Encoding)
{
    TEST(SingleObject)
    {
        testEncodeAndDecode([] (Encoder& encoder)
        {
            ObjectEncoder object = encoder.encodeObject();
            object.encodeString("String", "Testing");
        }, [] (Decoder& decoder)
        {
            ObjectDecoder object = decoder.decodeObject();

            CHECK(object.hasMember("String"));
            CHECK_EQUAL("Testing", object.decodeString("String"));
        });
    }

    TEST(SingleArray)
    {
        testEncodeAndDecode([] (Encoder& encoder)
        {
            ArrayEncoder array = encoder.encodeArray();
            array.encodeString("Zero");
            array.encodeString("One");
            array.encodeString("Two");
        }, [] (Decoder& decoder)
        {
            ArrayDecoder array = decoder.decodeArray();

            std::vector<std::string> strings;
            while (array.hasMoreElements())
            {
                strings.push_back(array.decodeString());
            }

            CHECK_EQUAL("Zero", strings[0]);
            CHECK_EQUAL("One", strings[1]);
            CHECK_EQUAL("Two", strings[2]);
        });
    }

    TEST(ArrayInObject)
    {
        testEncodeAndDecode([] (Encoder& encoder)
        {
            ObjectEncoder object = encoder.encodeObject();
            ArrayEncoder array = object.encodeArray("Array");
            array.encodeString("Zero");
            array.encodeString("One");
            array.encodeString("Two");
        }, [] (Decoder& decoder)
        {
            ObjectDecoder object = decoder.decodeObject();

            CHECK(object.hasMember("Array"));

            ArrayDecoder array = object.decodeArray("Array");

            std::vector<std::string> strings;
            while (array.hasMoreElements())
            {
                strings.push_back(array.decodeString());
            }

            CHECK_EQUAL("Zero", strings[0]);
            CHECK_EQUAL("One", strings[1]);
            CHECK_EQUAL("Two", strings[2]);
        });
    }

    TEST(ArrayInArray)
    {
        testEncodeAndDecode([] (Encoder& encoder)
        {
            ArrayEncoder array = encoder.encodeArray();

            for (int i = 0; i < 3; ++i)
            {
                ArrayEncoder nested = array.encodeArray();
                nested.encodeString("Zero");
                nested.encodeString("One");
                nested.encodeString("Two");
            }
        }, [] (Decoder& decoder)
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
        testEncodeAndDecode([] (Encoder& encoder)
        {
            ArrayEncoder array = encoder.encodeArray();

            for (int i = 0; i < 3; ++i)
            {
                ObjectEncoder object = array.encodeObject();
                object.encodeString("String", "Testing");
            }
        }, [] (Decoder& decoder)
        {
            ArrayDecoder array = decoder.decodeArray();

            int objectCount = 0;
            while (array.hasMoreElements())
            {
                ObjectDecoder object = array.decodeObject();

                CHECK(object.hasMember("String"));
                CHECK_EQUAL("Testing", object.decodeString("String"));

                ++objectCount;
            }

            CHECK_EQUAL(3, objectCount);
        });
    }

    TEST(AllInArray)
    {
        testEncodeAndDecode([] (Encoder& encoder)
        {
            ArrayEncoder array = encoder.encodeArray();

            array.encodeString("Test");
            array.encodeByte(12);
            array.encodeUnsignedByte(12);
            array.encodeShort(12);
            array.encodeUnsignedShort(12);
            array.encodeInt(12);
            array.encodeUnsignedInt(12);
            array.encodeLong(12);
            array.encodeUnsignedLong(12);
            array.encodeFloat(123.0f);
            array.encodeDouble(123.0);
            array.encodeReal(123);
            array.encodeBool(true);
            array.encodeVector2(Vector2(1, 2));
            array.encodeVector3(Vector3(1, 2, 3));
            array.encodeVector4(Vector4(1, 2, 3, 4));
        }, [] (Decoder& decoder)
        {
            ArrayDecoder array = decoder.decodeArray();

            CHECK_EQUAL("Test", array.decodeString());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12, array.decodeByte());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12u, array.decodeUnsignedByte());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12, array.decodeShort());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12u, array.decodeUnsignedShort());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12, array.decodeInt());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12u, array.decodeUnsignedInt());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12, array.decodeLong());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(12u, array.decodeUnsignedLong());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(123.0f, array.decodeFloat());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(123.0, array.decodeDouble());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(123, array.decodeReal());
            CHECK(array.hasMoreElements());
            CHECK_EQUAL(true, array.decodeBool());
            CHECK(array.hasMoreElements());
            Vector2 v2 = array.decodeVector2();
            CHECK_EQUAL(1, v2.x);
            CHECK_EQUAL(2, v2.y);
            CHECK(array.hasMoreElements());
            Vector3 v3 = array.decodeVector3();
            CHECK_EQUAL(1, v3.x);
            CHECK_EQUAL(2, v3.y);
            CHECK_EQUAL(3, v3.z);
            CHECK(array.hasMoreElements());
            Vector4 v4 = array.decodeVector4();
            CHECK_EQUAL(1, v4.x);
            CHECK_EQUAL(2, v4.y);
            CHECK_EQUAL(3, v4.z);
            CHECK_EQUAL(4, v4.w);
            CHECK(!array.hasMoreElements());
        });
    }

    TEST(AllInObject)
    {
        testEncodeAndDecode([] (Encoder& encoder)
        {
            ObjectEncoder object = encoder.encodeObject();

            object.encodeString("String", "Test");
            object.encodeByte("Byte", 12);
            object.encodeUnsignedByte("UnsignedByte", 12);
            object.encodeShort("Short", 12);
            object.encodeUnsignedShort("UnsignedShort", 12);
            object.encodeInt("Int", 12);
            object.encodeUnsignedInt("UnsignedInt", 12);
            object.encodeLong("Long", 12);
            object.encodeUnsignedLong("UnsignedLong", 12);
            object.encodeFloat("Float", 123.0f);
            object.encodeDouble("Double", 123.0);
            object.encodeReal("Real", 123);
            object.encodeBool("Bool", true);
            object.encodeVector2("Vector2", Vector2(1, 2));
            object.encodeVector3("Vector3", Vector3(1, 2, 3));
            object.encodeVector4("Vector4", Vector4(1, 2, 3, 4));
        }, [] (Decoder& decoder)
        {
            ObjectDecoder object = decoder.decodeObject();

            CHECK(object.hasMember("String"));
            CHECK_EQUAL("Test", object.decodeString("String"));
            CHECK(object.hasMember("Byte"));
            CHECK_EQUAL(12, object.decodeByte("Byte"));
            CHECK(object.hasMember("UnsignedByte"));
            CHECK_EQUAL(12u, object.decodeUnsignedByte("UnsignedByte"));
            CHECK(object.hasMember("Short"));
            CHECK_EQUAL(12, object.decodeShort("Short"));
            CHECK(object.hasMember("UnsignedShort"));
            CHECK_EQUAL(12u, object.decodeUnsignedShort("UnsignedShort"));
            CHECK(object.hasMember("Int"));
            CHECK_EQUAL(12, object.decodeInt("Int"));
            CHECK(object.hasMember("UnsignedInt"));
            CHECK_EQUAL(12u, object.decodeUnsignedInt("UnsignedInt"));
            CHECK(object.hasMember("Long"));
            CHECK_EQUAL(12, object.decodeLong("Long"));
            CHECK(object.hasMember("UnsignedLong"));
            CHECK_EQUAL(12u, object.decodeUnsignedLong("UnsignedLong"));
            CHECK(object.hasMember("Float"));
            CHECK_EQUAL(123.0f, object.decodeFloat("Float"));
            CHECK(object.hasMember("Double"));
            CHECK_EQUAL(123.0, object.decodeDouble("Double"));
            CHECK(object.hasMember("Real"));
            CHECK_EQUAL(123, object.decodeReal("Real"));
            CHECK(object.hasMember("Bool"));
            CHECK_EQUAL(true, object.decodeBool("Bool"));
            CHECK(object.hasMember("Vector2"));
            Vector2 v2 = object.decodeVector2("Vector2");
            CHECK_EQUAL(1, v2.x);
            CHECK_EQUAL(2, v2.y);
            CHECK(object.hasMember("Vector3"));
            Vector3 v3 = object.decodeVector3("Vector3");
            CHECK_EQUAL(1, v3.x);
            CHECK_EQUAL(2, v3.y);
            CHECK_EQUAL(3, v3.z);
            CHECK(object.hasMember("Vector4"));
            Vector4 v4 = object.decodeVector4("Vector4");
            CHECK_EQUAL(1, v4.x);
            CHECK_EQUAL(2, v4.y);
            CHECK_EQUAL(3, v4.z);
            CHECK_EQUAL(4, v4.w);
        });
    }
}