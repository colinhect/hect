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
#include "IO/ReadStream.h"

namespace hect
{

    ///
    /// Provides access for deserializing an array.
    class ArrayDeserializer
    {
        friend class Deserializer;
        friend class DataValueDeserializer;
        friend class BinaryDeserializer;

        friend class ObjectDeserializer;
    public:
        ArrayDeserializer(const ArrayDeserializer& deserializer);
        ArrayDeserializer(ArrayDeserializer&& deserializer);
        ~ArrayDeserializer();

        ///
        /// Returns whether the deserializer has reached the end of the array.
        bool hasMoreElements() const;

        ///
        /// Reads a string.
        ///
        /// \returns The read string.
        std::string readString();

        ///
        /// Begins reading an array.
        ///
        /// \returns The deserializer for the array.
        ArrayDeserializer readArray();

        ///
        /// Begins reading an object.
        ///
        /// \returns The deserializer for the object.
        ObjectDeserializer readObject();

        ///
        /// Returns whether the deserializer has an array to deserialize.
        operator bool() const;

    private:
        ArrayDeserializer();
        ArrayDeserializer(Deserializer* deserializer);

        Deserializer* _deserializer;
    };

    ///
    /// Provides access for deserializing an object.
    class ObjectDeserializer
    {
        friend class Deserializer;
        friend class DataValueDeserializer;
        friend class BinaryDeserializer;

        friend class ArrayDeserializer;
    public:
        ObjectDeserializer(const ObjectDeserializer& deserializer);
        ObjectDeserializer(ObjectDeserializer&& deserializer);
        ~ObjectDeserializer();

        ///
        /// Returns whether the object has a member with a specific name.
        ///
        /// \param name The name of the member in question.
        bool hasMember(const char* name) const;
        
        ///
        /// Reads a string.
        ///
        /// \param name The name of the member to read.
        ///
        /// \returns The read string.
        std::string readString(const char* name);
        
        ///
        /// Begins reading an array.
        ///
        /// \param name The name of the member to begin reading.
        ///
        /// \returns The deserializer for the array.
        ArrayDeserializer readArray(const char* name);

        ///
        /// Begins reading an object.
        ///
        /// \param name The name of the member to begin reading.
        ///
        /// \returns The deserializer for the object.
        ObjectDeserializer readObject(const char* name);
        
        ///
        /// Returns whether the deserializer has an object to deserialize.
        operator bool() const;

    private:
        ObjectDeserializer();
        ObjectDeserializer(Deserializer* deserializer);

        Deserializer* _deserializer;
    };

    ///
    /// Provides abstract access for deserializing structured data.
    class Deserializer
    {
        friend class ArrayDeserializer;
        friend class ObjectDeserializer;
    public:

        ///
        /// Returns whether the source data is human-readable.
        virtual bool isHumanReadable() const = 0;

        ///
        /// Begins reading an array.
        ///
        /// \returns The deserializer for the array.
        virtual ArrayDeserializer readArray() = 0;

        ///
        /// Begins reading an object.
        ///
        /// \returns The deserializer for the object.
        virtual ObjectDeserializer readObject() = 0;

    protected:
        virtual bool beginArray() = 0;
        virtual bool beginArray(const char* name) = 0;
        virtual void endArray() = 0;

        virtual bool hasMoreElements() const = 0;

        virtual bool beginObject() = 0;
        virtual bool beginObject(const char* name) = 0;
        virtual void endObject() = 0;

        virtual bool hasMember(const char* name) const = 0;

        virtual std::string readString() = 0;
        virtual std::string readString(const char* name) = 0;
    };

    ///
    /// Provides access for deserializing structured data from a data value.
    class DataValueDeserializer :
        public Deserializer
    {
    public:

        ///
        /// Constructs a deserializer given the data value to deserialize.
        ///
        /// \param dataValue The data value to deserialize.
        DataValueDeserializer(const DataValue& dataValue);

        bool isHumanReadable() const;

        ArrayDeserializer readArray();
        ObjectDeserializer readObject();

    private:
        bool beginArray();
        bool beginArray(const char* name);
        void endArray();

        bool hasMoreElements() const;

        bool beginObject();
        bool beginObject(const char* name);
        void endObject();

        bool hasMember(const char* name) const;

        std::string readString();
        std::string readString(const char* name);

        const DataValue& _deserialize();
        const DataValue& _deserialize(const char* name);

        std::stack<size_t> _indexStack;
        std::stack<DataValue> _valueStack;
    };

    ///
    /// Provides access for deserializing structured data from a binary stream.
    class BinaryDeserializer :
        public Deserializer
    {
    public:

        ///
        /// Constructs a deserializer given the stream to deserialize from.
        ///
        /// \param stream The stream to deserialize from.
        BinaryDeserializer(ReadStream& stream);

        bool isHumanReadable() const;

        ArrayDeserializer readArray();
        ObjectDeserializer readObject();

    private:
        bool beginArray();
        bool beginArray(const char* name);
        void endArray();

        bool hasMoreElements() const;

        bool beginObject();
        bool beginObject(const char* name);
        void endObject();

        bool hasMember(const char* name) const;

        std::string readString();
        std::string readString(const char* name);

        std::stack<unsigned> _indexStack;
        std::stack<unsigned> _countStack;
        ReadStream* _stream;
    };
}