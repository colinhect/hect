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

#include "Hect/IO/ReadStream.h"
#include "Hect/Core/Enum.h"

namespace hect
{

///
/// Provides access for decoding an array.
class ArrayDecoder
{
    friend class Decoder;
    friend class JsonDecoder;
    friend class BinaryDecoder;
    friend class ObjectDecoder;
public:
    ArrayDecoder(const ArrayDecoder& decoder);
    ArrayDecoder(ArrayDecoder&& decoder);
    ~ArrayDecoder();

    ///
    /// Returns whether the decoder is reading from a binary stream.
    bool isBinaryStream() const;

    ///
    /// Returns the raw binary stream.
    ///
    /// \throws Error If the decoder is not reading from a binary stream.
    ReadStream& binaryStream();

    ///
    /// Returns whether there are more elements in the array.
    bool hasMoreElements() const;

    ///
    /// Begins decoding an array.
    ///
    /// \returns The decoder for the array.
    ArrayDecoder decodeArray();

    ///
    /// Begins decoding an object.
    ///
    /// \returns The decoder for the object.
    ObjectDecoder decodeObject();

    ///
    /// Decodes a string.
    std::string decodeString();

    ///
    /// Decodes a signed 8-bit integer.
    int8_t decodeByte();

    ///
    /// Decodes an unsigned 16-bit integer.
    uint8_t decodeUnsignedByte();

    ///
    /// Decodes a signed 16-bit integer.
    int16_t decodeShort();

    ///
    /// Decodes an unsigned 16-bit integer.
    uint16_t decodeUnsignedShort();

    ///
    /// Decodes a signed 32-bit integer.
    int32_t decodeInt();

    ///
    /// Decodes an unsigned 32-bit integer.
    uint32_t decodeUnsignedInt();

    ///
    /// Decodes a signed 64-bit integer.
    int64_t decodeLong();

    ///
    /// Decodes an unsigned 64-bit integer.
    uint64_t decodeUnsignedLong();

    ///
    /// Decodes a 32-bit float.
    float decodeFloat();

    ///
    /// Decodes a 64-bit float.
    double decodeDouble();

    ///
    /// Decodes a real number.
    Real decodeReal();

    ///
    /// Decodes bool as an unsigned 8-bit integer.
    bool decodeBool();

    ///
    /// Decodes a 2-dimensional vector.
    Vector2 decodeVector2();

    ///
    /// Decodes a 3-dimensional vector.
    Vector3 decodeVector3();

    ///
    /// Decodes a 4-dimensional vector.
    Vector4 decodeVector4();

    ///
    /// Decodes an enum.
    template <typename T>
    T decodeEnum();

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
    friend class JsonDecoder;
    friend class BinaryDecoder;
    friend class ArrayDecoder;
public:
    ObjectDecoder(const ObjectDecoder& decoder);
    ObjectDecoder(ObjectDecoder&& decoder);
    ~ObjectDecoder();

    ///
    /// Returns whether the decoder is reading from a binary stream.
    bool isBinaryStream() const;

    ///
    /// Returns the raw binary stream.
    ///
    /// \throws Error If the decoder is not reading from a binary stream.
    ReadStream& binaryStream();

    ///
    /// Returns whether the object has a member with a specific name.
    ///
    /// \param name The name of the member in question.
    bool hasMember(const char* name) const;

    ///
    /// Begins decoding an array.
    ///
    /// \param name The name of the member to begin decoding.
    ///
    /// \returns The decoder for the array.
    ArrayDecoder decodeArray(const char* name);

    ///
    /// Begins decoding an object.
    ///
    /// \param name The name of the member to begin decoding.
    ///
    /// \returns The decoder for the object.
    ObjectDecoder decodeObject(const char* name);

    ///
    /// Decodes a string.
    ///
    /// \param name The name of the member to decode.
    std::string decodeString(const char* name);

    ///
    /// Decodes a signed 8-bit integer.
    ///
    /// \param name The name of the member to decode.
    int8_t decodeByte(const char* name);

    ///
    /// Decodes an unsigned 16-bit integer.
    ///
    /// \param name The name of the member to decode.
    uint8_t decodeUnsignedByte(const char* name);

    ///
    /// Decodes a signed 16-bit integer.
    ///
    /// \param name The name of the member to decode.
    int16_t decodeShort(const char* name);

    ///
    /// Decodes an unsigned 16-bit integer.
    ///
    /// \param name The name of the member to decode.
    uint16_t decodeUnsignedShort(const char* name);

    ///
    /// Decodes a signed 32-bit integer.
    ///
    /// \param name The name of the member to decode.
    int32_t decodeInt(const char* name);

    ///
    /// Decodes an unsigned 32-bit integer.
    ///
    /// \param name The name of the member to decode.
    uint32_t decodeUnsignedInt(const char* name);

    ///
    /// Decodes a signed 64-bit integer.
    ///
    /// \param name The name of the member to decode.
    int64_t decodeLong(const char* name);

    ///
    /// Decodes an unsigned 64-bit integer.
    ///
    /// \param name The name of the member to decode.
    uint64_t decodeUnsignedLong(const char* name);

    ///
    /// Decodes a 32-bit float.
    ///
    /// \param name The name of the member to decode.
    float decodeFloat(const char* name);

    ///
    /// Decodes a 64-bit float.
    ///
    /// \param name The name of the member to decode.
    double decodeDouble(const char* name);

    ///
    /// Decodes a real number.
    ///
    /// \param name The name of the member to decode.
    Real decodeReal(const char* name);

    ///
    /// Decodes bool as an unsigned 8-bit integer.
    ///
    /// \param name The name of the member to decode
    bool decodeBool(const char* name);

    ///
    /// Decodes a 2-dimensional vector.
    ///
    /// \param name The name of the member to decode.
    Vector2 decodeVector2(const char* name);

    ///
    /// Decodes a 3-dimensional vector.
    ///
    /// \param name The name of the member to decode.
    Vector3 decodeVector3(const char* name);

    ///
    /// Decodes a 4-dimensional vector.
    ///
    /// \param name The name of the member to decode.
    Vector4 decodeVector4(const char* name);

    ///
    /// Decodes an enum.
    ///
    /// \param name The name of the member to decode.
    template <typename T>
    T decodeEnum(const char* name);

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
    /// Returns whether the decoder is reading from a binary stream.
    virtual bool isBinaryStream() const = 0;

    ///
    /// Returns the raw binary stream.
    ///
    /// \throws Error If the decoder is not reading from a binary stream.
    virtual ReadStream& binaryStream() = 0;

    ///
    /// Begins decoding an array.
    ///
    /// \returns The decoder for the array.
    virtual ArrayDecoder decodeArray() = 0;

    ///
    /// Begins decoding an object.
    ///
    /// \returns The decoder for the object.
    virtual ObjectDecoder decodeObject() = 0;

protected:
    virtual void beginArray() = 0;
    virtual void beginArray(const char* name) = 0;
    virtual void endArray() = 0;

    virtual bool hasMoreElements() const = 0;

    virtual void beginObject() = 0;
    virtual void beginObject(const char* name) = 0;
    virtual void endObject() = 0;

    virtual bool hasMember(const char* name) const = 0;

    virtual std::string decodeString() = 0;
    virtual std::string decodeString(const char* name) = 0;
    virtual int8_t decodeByte() = 0;
    virtual int8_t decodeByte(const char* name) = 0;
    virtual uint8_t decodeUnsignedByte() = 0;
    virtual uint8_t decodeUnsignedByte(const char* name) = 0;
    virtual int16_t decodeShort() = 0;
    virtual int16_t decodeShort(const char* name) = 0;
    virtual uint16_t decodeUnsignedShort() = 0;
    virtual uint16_t decodeUnsignedShort(const char* name) = 0;
    virtual int32_t decodeInt() = 0;
    virtual int32_t decodeInt(const char* name) = 0;
    virtual uint32_t decodeUnsignedInt() = 0;
    virtual uint32_t decodeUnsignedInt(const char* name) = 0;
    virtual int64_t decodeLong() = 0;
    virtual int64_t decodeLong(const char* name) = 0;
    virtual uint64_t decodeUnsignedLong() = 0;
    virtual uint64_t decodeUnsignedLong(const char* name) = 0;
    virtual float decodeFloat() = 0;
    virtual float decodeFloat(const char* name) = 0;
    virtual double decodeDouble() = 0;
    virtual double decodeDouble(const char* name) = 0;
    virtual Real decodeReal() = 0;
    virtual Real decodeReal(const char* name) = 0;
    virtual bool decodeBool() = 0;
    virtual bool decodeBool(const char* name) = 0;
    virtual Vector2 decodeVector2() = 0;
    virtual Vector2 decodeVector2(const char* name) = 0;
    virtual Vector3 decodeVector3() = 0;
    virtual Vector3 decodeVector3(const char* name) = 0;
    virtual Vector4 decodeVector4() = 0;
    virtual Vector4 decodeVector4(const char* name) = 0;
};

}

#include "Decoder.inl"