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
    class ArraySerializer
    {
        friend class Serializer;
        friend class DataValueSerializer;

        friend class ObjectSerializer;
    public:
        ArraySerializer(const ArraySerializer& serializer);
        ArraySerializer(ArraySerializer&& serializer);
        ~ArraySerializer();

        ///
        /// Serializes a string.
        ///
        /// \param value The value to serialize.
        void writeString(const std::string& value);

        ///
        /// Serializes an array.
        ///
        /// \returns The serializer.
        ArraySerializer writeArray();

        ///
        /// Serializes an object.
        ///
        /// \returns The serializer.
        ObjectSerializer writeObject();

    private:
        ArraySerializer();
        ArraySerializer(Serializer* serializer);

        Serializer* _serializer;
    };

    class ObjectSerializer
    {
        friend class Serializer;
        friend class DataValueSerializer;

        friend class ArraySerializer;
    public:
        ObjectSerializer(const ObjectSerializer& serializer);
        ObjectSerializer(ObjectSerializer&& serializer);
        ~ObjectSerializer();

        ///
        /// Serializes a string.
        ///
        /// \param name The name of the member to serialize to.
        /// \param value The value to serialize.
        void writeString(const char* name, const std::string& value);

        ///
        /// Serializes an array.
        ///
        /// \param name The name of the member to serialize to.
        ///
        /// \returns The serializer.
        ArraySerializer writeArray(const char* name);

        ///
        /// Serializes an object.
        ///
        /// \param name The name of the member to serialize to.
        ///
        /// \returns The serializer.
        ObjectSerializer writeObject(const char* name);

    private:
        ObjectSerializer();
        ObjectSerializer(Serializer* serializer);

        Serializer* _serializer;
    };

    ///
    /// Provides abstract access for encoding structured data.
    class Serializer
    {
        friend class ArraySerializer;
        friend class ObjectSerializer;
    public:

        ///
        /// Returns whether the resulting data is human-readable.
        virtual bool isHumanReadable() const = 0;

        ///
        /// Serializes an array.
        ///
        /// \returns The serializer.
        virtual ArraySerializer writeArray() = 0;

        ///
        /// Serializes an object.
        ///
        /// \returns The serializer.
        virtual ObjectSerializer writeObject() = 0;

    protected:
        virtual void beginArray() = 0;
        virtual void beginArray(const char* name) = 0;
        virtual void endArray() = 0;

        virtual void beginObject() = 0;
        virtual void beginObject(const char* name) = 0;
        virtual void endObject() = 0;

        virtual void writeString(const std::string& value) = 0;
        virtual void writeString(const char* name, const std::string& value) = 0;
    };

    ///
    /// Provides access for encoding structured data to a data value.
    class DataValueSerializer :
        public Serializer
    {
    public:
        bool isHumanReadable() const;

        ArraySerializer writeArray();
        ObjectSerializer writeObject();

        ///
        /// Returns the serialized data values.
        DataValue::Array& serializedDataValue();

    private:
        void beginArray();
        void beginArray(const char* name);
        void endArray();

        void beginObject();
        void beginObject(const char* name);
        void endObject();

        void writeString(const std::string& value);
        void writeString(const char* name, const std::string& value);

        void _serialize(const DataValue& value);
        void _serialize(const char* name, const DataValue& value);

        std::stack<std::string> _nameStack;
        std::stack<DataValue> _valueStack;

        DataValue::Array _completed;
    };

}