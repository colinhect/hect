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
    class ArrayDecoder
    {
        friend class Decoder;
        friend class DataValueDecoder;

        friend class ObjectDecoder;
    public:
        ArrayDecoder(const ArrayDecoder& decoder);
        ArrayDecoder(ArrayDecoder&& decoder);
        ~ArrayDecoder();

        ///
        /// Returns whether the decoder has reached the end of the array.
        bool hasMoreElements() const;

        ///
        /// Decodes a string.
        ///
        /// \returns The decoded string.
        std::string decodeString();

        ///
        /// Decodes an array.
        ///
        /// \returns The decoder.
        ArrayDecoder decodeArray();

        ///
        /// Decodes an object.
        ///
        /// \returns The decoder.
        ObjectDecoder decodeObject();

        ///
        /// Returns whether the decoder has an array to decode.
        operator bool() const;

    private:
        ArrayDecoder();
        ArrayDecoder(Decoder* decoder);

        Decoder* _decoder;
    };

    ///
    /// Provides access for decoding an object.
    class ObjectDecoder
    {
        friend class Decoder;
        friend class DataValueDecoder;

        friend class ArrayDecoder;
    public:
        ObjectDecoder(const ObjectDecoder& decoder);
        ObjectDecoder(ObjectDecoder&& decoder);
        ~ObjectDecoder();

        ///
        /// Returns whether the object has a member with a specific name.
        ///
        /// \param name The name of the member in question.
        bool hasMember(const char* name) const;
        
        ///
        /// Decodes a string.
        ///
        /// \param name The name of the member to decode.
        ///
        /// \returns The decoded string.
        std::string decodeString(const char* name);
        
        ///
        /// Decodes an array.
        ///
        /// \param name The name of the member to decode.
        ///
        /// \returns The decoder.
        ArrayDecoder decodeArray(const char* name);

        ///
        /// Decodes an object.
        ///
        /// \param name The name of the member to decode.
        ///
        /// \returns The decoder.
        ObjectDecoder decodeObject(const char* name);
        
        ///
        /// Returns whether the decoder has an object to decode.
        operator bool() const;

    private:
        ObjectDecoder();
        ObjectDecoder(Decoder* decoder);

        Decoder* _decoder;
    };

    ///
    /// Provides abstract access for decoding structured data.
    class Decoder
    {
        friend class ArrayDecoder;
        friend class ObjectDecoder;
    public:

        ///
        /// Returns whether the source data is human-readable.
        virtual bool isHumanReadable() const = 0;

        ///
        /// Decodes an array.
        ///
        /// \returns The decoder.
        virtual ArrayDecoder decodeArray() = 0;

        ///
        /// Decodes an object.
        ///
        /// \returns The decoder.
        virtual ObjectDecoder decodeObject() = 0;

    protected:
        virtual bool beginArray() = 0;
        virtual bool beginArray(const char* name) = 0;
        virtual void endArray() = 0;

        virtual bool hasMoreElements() const = 0;

        virtual bool beginObject() = 0;
        virtual bool beginObject(const char* name) = 0;
        virtual void endObject() = 0;

        virtual bool hasMember(const char* name) const = 0;

        virtual std::string decodeString() = 0;
        virtual std::string decodeString(const char* name) = 0;
    };

    ///
    /// Provides access for decoding structured data from a data value.
    class DataValueDecoder :
        public Decoder
    {
    public:

        ///
        /// Constructs a decoder given the data value to decode.
        ///
        /// \param dataValue The data value to decode.
        DataValueDecoder(const DataValue& dataValue);

        bool isHumanReadable() const;

        ArrayDecoder decodeArray();
        ObjectDecoder decodeObject();

    private:
        bool beginArray();
        bool beginArray(const char* name);
        void endArray();

        bool hasMoreElements() const;

        bool beginObject();
        bool beginObject(const char* name);
        void endObject();

        bool hasMember(const char* name) const;

        std::string decodeString();
        std::string decodeString(const char* name);

        const DataValue& _decode();
        const DataValue& _decode(const char* name);

        std::stack<size_t> _indexStack;
        std::stack<DataValue> _valueStack;
    };

}