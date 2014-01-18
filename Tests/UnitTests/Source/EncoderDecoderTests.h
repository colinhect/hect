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

SUITE(EncoderDecoder)
{
    void encodeSingleObject(Encoder& encoder)
    {
        ObjectEncoder object = encoder.encodeObject();
        object.encodeString("String", "Testing");
    }

    void decodeSingleObject(Decoder& decoder)
    {
        ObjectDecoder object = decoder.decodeObject();

        bool hasObject = object;
        CHECK(hasObject);

        CHECK(object.hasMember("String"));
        CHECK_EQUAL("Testing", object.decodeString("String"));
    }

    TEST(SingleObjectDataValue)
    {
        DataValue dataValue;
        {
            DataValueEncoder encoder;
            encodeSingleObject(encoder);
            dataValue = encoder.encodedDataValue()[0];
        }
        {
            DataValueDecoder decoder(dataValue);
            decodeSingleObject(decoder);
        }
    }

    void encodeSingleArray(Encoder& encoder)
    {
        ArrayEncoder array = encoder.encodeArray();
        array.encodeString("Zero");
        array.encodeString("One");
        array.encodeString("Two");
    }

    void decodeSingleArray(Decoder& decoder)
    {
        ArrayDecoder array = decoder.decodeArray();

        bool hasArray = array;
        CHECK(hasArray);

        std::vector<std::string> strings;
        while (array.hasMoreElements())
        {
            strings.push_back(array.decodeString());
        }
        
        CHECK_EQUAL("Zero", strings[0]);
        CHECK_EQUAL("One", strings[1]);
        CHECK_EQUAL("Two", strings[2]);
    }

    TEST(SingleArrayDataValue)
    {
        DataValue dataValue;
        {
            DataValueEncoder encoder;
            encodeSingleArray(encoder);
            dataValue = encoder.encodedDataValue()[0];
        }
        {
            DataValueDecoder decoder(dataValue);
            decodeSingleArray(decoder);
        }
    }

    void encodeArrayInObject(Encoder& encoder)
    {
        ObjectEncoder object = encoder.encodeObject();
        ArrayEncoder array = object.encodeArray("Array");
        array.encodeString("Zero");
        array.encodeString("One");
        array.encodeString("Two");
    }

    void decodeArrayInObject(Decoder& decoder)
    {
        ObjectDecoder object = decoder.decodeObject();

        bool hasObject = object;
        CHECK(hasObject);

        CHECK(object.hasMember("Array"));

        ArrayDecoder array = object.decodeArray("Array");

        bool hasArray = array;
        CHECK(hasArray);

        std::vector<std::string> strings;
        while (array.hasMoreElements())
        {
            strings.push_back(array.decodeString());
        }
        
        CHECK_EQUAL("Zero", strings[0]);
        CHECK_EQUAL("One", strings[1]);
        CHECK_EQUAL("Two", strings[2]);
    }

    TEST(ArrayInObjectDataValue)
    {
        DataValue dataValue;
        {
            DataValueEncoder encoder;
            encodeArrayInObject(encoder);
            dataValue = encoder.encodedDataValue()[0];
        }
        {
            DataValueDecoder decoder(dataValue);
            decodeArrayInObject(decoder);
        }
    }

    void encodeArrayInArray(Encoder& encoder)
    {
        ArrayEncoder array = encoder.encodeArray();

        for (int i = 0; i < 3; ++i)
        {
            ArrayEncoder nested = array.encodeArray();
            nested.encodeString("Zero");
            nested.encodeString("One");
            nested.encodeString("Two");
        }
    }

    void decodeArrayInArray(Decoder& decoder)
    {
        ArrayDecoder array = decoder.decodeArray();

        bool hasArray = array;
        CHECK(hasArray);

        int arrayCount = 0;
        while (array.hasMoreElements())
        {
            ArrayDecoder nested = array.decodeArray();

            bool hasArray = nested;
            CHECK(hasArray);

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
    }

    TEST(ArrayInArrayDataValue)
    {
        DataValue dataValue;
        {
            DataValueEncoder encoder;
            encodeArrayInArray(encoder);
            dataValue = encoder.encodedDataValue()[0];
        }
        {
            DataValueDecoder decoder(dataValue);
            decodeArrayInArray(decoder);
        }
    }

    void encodeObjectInArray(Encoder& encoder)
    {
        ArrayEncoder array = encoder.encodeArray();

        for (int i = 0; i < 3; ++i)
        {
            ObjectEncoder object = array.encodeObject();
            object.encodeString("String", "Testing");
        }
    }

    void decodeObjectInArray(Decoder& decoder)
    {
        ArrayDecoder array = decoder.decodeArray();

        bool hasArray = array;
        CHECK(hasArray);

        int objectCount = 0;
        while (array.hasMoreElements())
        {
            ObjectDecoder object = array.decodeObject();

            bool hasObject = object;
            CHECK(hasObject);

            CHECK(object.hasMember("String"));
            CHECK_EQUAL("Testing", object.decodeString("String"));

            ++objectCount;
        }

        CHECK_EQUAL(3, objectCount);
    }

    TEST(ObjectInArrayDataValue)
    {
        DataValue dataValue;
        {
            DataValueEncoder encoder;
            encodeObjectInArray(encoder);
            dataValue = encoder.encodedDataValue()[0];
        }
        {
            DataValueDecoder decoder(dataValue);
            decodeObjectInArray(decoder);
        }
    }
}