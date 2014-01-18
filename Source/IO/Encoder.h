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

#include <stack>

#include "Core/DataValue.h"
#include "IO/WriteStream.h"

namespace hect
{

    ///
    /// Provides access for encoding an array.
    class ArrayEncoder
    {
        friend class Encoder;
        friend class DataValueEncoder;

        friend class ObjectEncoder;
    public:
        ArrayEncoder(const ArrayEncoder& encoder);
        ArrayEncoder(ArrayEncoder&& encoder);
        ~ArrayEncoder();

        ///
        /// Encodes a string.
        ///
        /// \param value The value to encode.
        void encodeString(const std::string& value);

        ///
        /// Encodes an array.
        ///
        /// \returns The encoder.
        ArrayEncoder encodeArray();

        ///
        /// Encodes an object.
        ///
        /// \returns The encoder.
        ObjectEncoder encodeObject();

    private:
        ArrayEncoder();
        ArrayEncoder(Encoder* encoder);

        Encoder* _encoder;
    };

    class ObjectEncoder
    {
        friend class Encoder;
        friend class DataValueEncoder;

        friend class ArrayEncoder;
    public:
        ObjectEncoder(const ObjectEncoder& encoder);
        ObjectEncoder(ObjectEncoder&& encoder);
        ~ObjectEncoder();

        ///
        /// Encodes a string.
        ///
        /// \param name The name of the member to encode to.
        /// \param value The value to encode.
        void encodeString(const char* name, const std::string& value);

        ///
        /// Encodes an array.
        ///
        /// \param name The name of the member to encode to.
        ///
        /// \returns The encoder.
        ArrayEncoder encodeArray(const char* name);

        ///
        /// Encodes an object.
        ///
        /// \param name The name of the member to encode to.
        ///
        /// \returns The encoder.
        ObjectEncoder encodeObject(const char* name);

    private:
        ObjectEncoder();
        ObjectEncoder(Encoder* encoder);

        Encoder* _encoder;
    };

    ///
    /// Provides abstract access for encoding structured data.
    class Encoder
    {
        friend class ArrayEncoder;
        friend class ObjectEncoder;
    public:

        ///
        /// Returns whether the resulting data is human-readable.
        virtual bool isHumanReadable() const = 0;

        ///
        /// Encodes an array.
        ///
        /// \returns The encoder.
        virtual ArrayEncoder encodeArray() = 0;

        ///
        /// Encodes an object.
        ///
        /// \returns The encoder.
        virtual ObjectEncoder encodeObject() = 0;

    protected:
        virtual void beginArray() = 0;
        virtual void beginArray(const char* name) = 0;
        virtual void endArray() = 0;

        virtual void beginObject() = 0;
        virtual void beginObject(const char* name) = 0;
        virtual void endObject() = 0;

        virtual void encodeString(const std::string& value) = 0;
        virtual void encodeString(const char* name, const std::string& value) = 0;
    };

    ///
    /// Provides access for encoding structured data to a data value.
    class DataValueEncoder :
        public Encoder
    {
    public:
        bool isHumanReadable() const;

        ArrayEncoder encodeArray();
        ObjectEncoder encodeObject();

        ///
        /// Returns the encoded data values.
        DataValue::Array& encodedDataValue();

    private:
        void beginArray();
        void beginArray(const char* name);
        void endArray();

        void beginObject();
        void beginObject(const char* name);
        void endObject();

        void encodeString(const std::string& value);
        void encodeString(const char* name, const std::string& value);

        void _encode(const DataValue& value);
        void _encode(const char* name, const DataValue& value);

        std::stack<std::string> _nameStack;
        std::stack<DataValue> _valueStack;

        DataValue::Array _completed;
    };

}