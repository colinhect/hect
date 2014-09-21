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

#include "Hect/IO/WriteStream.h"

namespace hect
{

///
/// Provides access for encoding an array.
class ArrayEncoder
{
    friend class Encoder;
    friend class JsonEncoder;
    friend class BinaryEncoder;

    friend class ObjectEncoder;
public:
    ArrayEncoder(const ArrayEncoder& encoder);
    ArrayEncoder(ArrayEncoder&& encoder);
    ~ArrayEncoder();

    ///
    /// Returns whether the encoder is writing to a binary stream.
    bool isBinaryStream() const;

    ///
    /// Returns the raw binary stream.
    ///
    /// \throws Error If the encoder is not writing to a binary stream.
    WriteStream& binaryStream();

    ///
    /// Begins encoding an array.
    ///
    /// \returns The encoder for the array.
    ArrayEncoder encodeArray();

    ///
    /// Begins encoding an object.
    ///
    /// \returns The encoder for the object.
    ObjectEncoder encodeObject();

    ///
    /// Encodes a string.
    ///
    /// \param value The string to encode.
    void encodeString(const std::string& value);

    ///
    /// Encodes a signed 8-bit integer.
    ///
    /// \param value The value to encode.
    void encodeInt8(int8_t value);

    ///
    /// Encodes an unsigned 8-bit integer.
    ///
    /// \param value The value to encode.
    void encodeUInt8(uint8_t value);

    ///
    /// Encodes a signed 16-bit integer.
    ///
    /// \param value The value to encode.
    void encodeInt16(int16_t value);

    ///
    /// Encodes an unsigned 16-bit integer.
    ///
    /// \param value The value to encode.
    void encodeUInt16(uint16_t value);

    ///
    /// Encodes a signed 32-bit integer.
    ///
    /// \param value The value to encode.
    void encodeInt32(int32_t value);

    ///
    /// Encodes an unsigned 32-bit integer.
    ///
    /// \param value The value to encode.
    void encodeUInt32(uint32_t value);

    ///
    /// Encodes a signed 64-bit integer.
    ///
    /// \param value The value to encode.
    void encodeInt64(int64_t value);

    ///
    /// Encodes an unsigned 64-bit integer.
    ///
    /// \param value The value to encode.
    void encodeUInt64(uint64_t value);

    ///
    /// Encodes a 32-bit float.
    ///
    /// \param value The value to encode.
    void encodeFloat32(float value);

    ///
    /// Encodes a 64-bit float
    ///
    /// \param value The value to encode.
    void encodeFloat64(double value);

    ///
    /// Encodes a real number.
    ///
    /// \param value The value to encode.
    void encodeReal(Real value);

    ///
    /// Encodes a bool as an unsigned 8-bit integer.
    ///
    /// \param value The value to encode.
    void encodeBool(bool value);

    ///
    /// Encodes a 2-dimensional vector.
    ///
    /// \param value The vector to encode.
    void encodeVector2(const Vector2& value);

    ///
    /// Encodes a 3-dimensional vector.
    ///
    /// \param value The vector to encode.
    void encodeVector3(const Vector3& value);

    ///
    /// Encodes a 4-dimensional vector.
    ///
    /// \param value The vector to encode.
    void encodeVector4(const Vector4& value);

    ///
    /// Encodes an enum.
    ///
    /// \param value The enum to encode.
    template <typename T>
    void encodeEnum(T value);

private:
    ArrayEncoder();
    ArrayEncoder(Encoder* encoder);

    Encoder* _encoder;
};

class ObjectEncoder
{
    friend class Encoder;
    friend class JsonEncoder;
    friend class BinaryEncoder;

    friend class ArrayEncoder;
public:
    ObjectEncoder(const ObjectEncoder& encoder);
    ObjectEncoder(ObjectEncoder&& encoder);
    ~ObjectEncoder();

    ///
    /// Returns whether the encoder is writing to a binary stream.
    bool isBinaryStream() const;

    ///
    /// Returns the raw binary stream.
    ///
    /// \throws Error If the encoder is not writing to a binary stream.
    WriteStream& binaryStream();

    ///
    /// Begins encoding an array.
    ///
    /// \param name The name of the member to encode to.
    ///
    /// \returns The encoder for the array.
    ArrayEncoder encodeArray(const char* name);

    ///
    /// Begins encoding an object.
    ///
    /// \param name The name of the member to encode to.
    ///
    /// \returns The encoder for the object.
    ObjectEncoder encodeObject(const char* name);

    ///
    /// Encodes a string.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The string to encode.
    void encodeString(const char* name, const std::string& value);

    ///
    /// Encodes a signed 8-bit integer.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The value to encode.
    void encodeInt8(const char* name, int8_t value);

    ///
    /// Encodes an unsigned 8-bit integer.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The value to encode.
    void encodeUInt8(const char* name, uint8_t value);

    ///
    /// Encodes a signed 16-bit integer.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The value to encode.
    void encodeInt16(const char* name, int16_t value);

    ///
    /// Encodes an unsigned 16-bit integer.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The value to encode.
    void encodeUInt16(const char* name, uint16_t value);

    ///
    /// Encodes a signed 32-bit integer.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The value to encode.
    void encodeInt32(const char* name, int32_t value);

    ///
    /// Encodes an unsigned 32-bit integer.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The value to encode.
    void encodeUInt32(const char* name, uint32_t value);

    ///
    /// Encodes a signed 64-bit integer.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The value to encode.
    void encodeInt64(const char* name, int64_t value);

    ///
    /// Encodes an unsigned 64-bit integer.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The value to encode.
    void encodeUInt64(const char* name, uint64_t value);

    ///
    /// Encodes a 32-bit float.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The value to encode.
    void encodeFloat32(const char* name, float value);

    ///
    /// Encodes a 64-bit float
    ///
    /// \param name The name of the member to encode to.
    /// \param value The value to encode.
    void encodeFloat64(const char* name, double value);

    ///
    /// Encodes a real number.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The value to encode.
    void encodeReal(const char* name, Real value);

    ///
    /// Encodes a bool as an unsigned 8-bit integer.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The value to encode.
    void encodeBool(const char* name, bool value);

    ///
    /// Encodes a 2-dimensional vector.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The vector to encode.
    void encodeVector2(const char* name, const Vector2& value);

    ///
    /// Encodes a 3-dimensional vector.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The vector to encode.
    void encodeVector3(const char* name, const Vector3& value);

    ///
    /// Encodes a 4-dimensional vector.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The vector to encode.
    void encodeVector4(const char* name, const Vector4& value);

    ///
    /// Encodes an enum.
    ///
    /// \param name The name of the member to encode to.
    /// \param value The enum to encode.
    template <typename T>
    void encodeEnum(const char* name, T value);

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
    /// Returns whether the encoder is writing to a binary stream.
    virtual bool isBinaryStream() const = 0;

    ///
    /// Returns the raw binary stream.
    ///
    /// \throws Error If the encoder is not writing to a binary stream.
    virtual WriteStream& binaryStream() = 0;

    ///
    /// Begins encoding an array.
    ///
    /// \returns The encoder for the array.
    virtual ArrayEncoder encodeArray() = 0;

    ///
    /// Begins encoding an object.
    ///
    /// \returns The encoder for the object.
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
    virtual void encodeInt8(int8_t value) = 0;
    virtual void encodeInt8(const char* name, int8_t value) = 0;
    virtual void encodeUInt8(uint8_t value) = 0;
    virtual void encodeUInt8(const char* name, uint8_t value) = 0;
    virtual void encodeInt16(int16_t value) = 0;
    virtual void encodeInt16(const char* name, int16_t value) = 0;
    virtual void encodeUInt16(uint16_t value) = 0;
    virtual void encodeUInt16(const char* name, uint16_t value) = 0;
    virtual void encodeInt32(int32_t value) = 0;
    virtual void encodeInt32(const char* name, int32_t value) = 0;
    virtual void encodeUInt32(uint32_t value) = 0;
    virtual void encodeUInt32(const char* name, uint32_t value) = 0;
    virtual void encodeInt64(int64_t value) = 0;
    virtual void encodeInt64(const char* name, int64_t value) = 0;
    virtual void encodeUInt64(uint64_t value) = 0;
    virtual void encodeUInt64(const char* name, uint64_t value) = 0;
    virtual void encodeFloat32(float value) = 0;
    virtual void encodeFloat32(const char* name, float value) = 0;
    virtual void encodeFloat64(double value) = 0;
    virtual void encodeFloat64(const char* name, double value) = 0;
    virtual void encodeReal(Real value) = 0;
    virtual void encodeReal(const char* name, Real value) = 0;
    virtual void encodeBool(bool value) = 0;
    virtual void encodeBool(const char* name, bool value) = 0;
    virtual void encodeVector2(const Vector2& value) = 0;
    virtual void encodeVector2(const char* name, const Vector2& value) = 0;
    virtual void encodeVector3(const Vector3& value) = 0;
    virtual void encodeVector3(const char* name, const Vector3& value) = 0;
    virtual void encodeVector4(const Vector4& value) = 0;
    virtual void encodeVector4(const char* name, const Vector4& value) = 0;
};

}

#include "Encoder.inl"