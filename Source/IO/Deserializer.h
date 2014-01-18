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
    /// Provides access for decoding an array.
    class ArrayDeserializer
    {
        friend class Deserializer;
        friend class DataValueDeserializer;

        friend class ObjectDeserializer;
    public:
        ArrayDeserializer(const ArrayDeserializer& deserializer);
        ArrayDeserializer(ArrayDeserializer&& deserializer);
        ~ArrayDeserializer();

        ///
        /// Returns whether the deserializer has reached the end of the array.
        bool hasMoreElements() const;

        ///
        /// Deserializes a string.
        ///
        /// \returns The deserialized string.
        std::string readString();

        ///
        /// Deserializes an array.
        ///
        /// \returns The deserializer.
        ArrayDeserializer readArray();

        ///
        /// Deserializes an object.
        ///
        /// \returns The deserializer.
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
    /// Provides access for decoding an object.
    class ObjectDeserializer
    {
        friend class Deserializer;
        friend class DataValueDeserializer;

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
        /// Deserializes a string.
        ///
        /// \param name The name of the member to deserialize.
        ///
        /// \returns The deserialized string.
        std::string readString(const char* name);
        
        ///
        /// Deserializes an array.
        ///
        /// \param name The name of the member to deserialize.
        ///
        /// \returns The deserializer.
        ArrayDeserializer readArray(const char* name);

        ///
        /// Deserializes an object.
        ///
        /// \param name The name of the member to deserialize.
        ///
        /// \returns The deserializer.
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
    /// Provides abstract access for decoding structured data.
    class Deserializer
    {
        friend class ArrayDeserializer;
        friend class ObjectDeserializer;
    public:

        ///
        /// Returns whether the source data is human-readable.
        virtual bool isHumanReadable() const = 0;

        ///
        /// Deserializes an array.
        ///
        /// \returns The deserializer.
        virtual ArrayDeserializer readArray() = 0;

        ///
        /// Deserializes an object.
        ///
        /// \returns The deserializer.
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
    /// Provides access for decoding structured data from a data value.
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

}