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
/// Provides access for writing an array.
class ArrayWriter
{
    friend class DataWriter;
    friend class DataValueWriter;
    friend class BinaryWriter;

    friend class ObjectWriter;
public:
    ArrayWriter(const ArrayWriter& writer);
    ArrayWriter(ArrayWriter&& writer);
    ~ArrayWriter();

    ///
    /// Begins writing an array.
    ///
    /// \returns The writer for the array.
    ArrayWriter writeArray();

    ///
    /// Begins writing an object.
    ///
    /// \returns The writer for the object.
    ObjectWriter writeObject();

    ///
    /// Writes a string.
    ///
    /// \param value The string to write.
    void writeString(const std::string& value);

    ///
    /// Writes a signed 8-bit integer.
    ///
    /// \param value The value to write.
    void writeByte(int8_t value);

    ///
    /// Writes an unsigned 8-bit integer.
    ///
    /// \param value The value to write.
    void writeUnsignedByte(uint8_t value);

    ///
    /// Writes a signed 16-bit integer.
    ///
    /// \param value The value to write.
    void writeShort(int16_t value);

    ///
    /// Writes an unsigned 16-bit integer.
    ///
    /// \param value The value to write.
    void writeUnsignedShort(uint16_t value);

    ///
    /// Writes a signed 32-bit integer.
    ///
    /// \param value The value to write.
    void writeInt(int32_t value);

    ///
    /// Writes an unsigned 32-bit integer.
    ///
    /// \param value The value to write.
    void writeUnsignedInt(uint32_t value);

    ///
    /// Writes a signed 64-bit integer.
    ///
    /// \param value The value to write.
    void writeLong(int64_t value);

    ///
    /// Writes an unsigned 64-bit integer.
    ///
    /// \param value The value to write.
    void writeUnsignedLong(uint64_t value);

    ///
    /// Writes a 32-bit float.
    ///
    /// \param value The value to write.
    void writeFloat(float value);

    ///
    /// Writes a 64-bit float
    ///
    /// \param value The value to write.
    void writeDouble(double value);

    ///
    /// Writes a real number.
    ///
    /// \param value The value to write.
    void writeReal(Real value);

    ///
    /// Writes a 2-dimensional vector.
    ///
    /// \param value The vector to write.
    void writeVector2(const Vector2& value);

    ///
    /// Writes a 3-dimensional vector.
    ///
    /// \param value The vector to write.
    void writeVector3(const Vector3& value);

    ///
    /// Writes a 4-dimensional vector.
    ///
    /// \param value The vector to write.
    void writeVector4(const Vector4& value);

private:
    ArrayWriter();
    ArrayWriter(DataWriter* writer);

    DataWriter* _writer;
};

class ObjectWriter
{
    friend class DataWriter;
    friend class DataValueWriter;
    friend class BinaryWriter;

    friend class ArrayWriter;
public:
    ObjectWriter(const ObjectWriter& writer);
    ObjectWriter(ObjectWriter&& writer);
    ~ObjectWriter();

    ///
    /// Begins writing an array.
    ///
    /// \param name The name of the member to write to.
    ///
    /// \returns The writer for the array.
    ArrayWriter writeArray(const char* name);

    ///
    /// Begins writing an object.
    ///
    /// \param name The name of the member to write to.
    ///
    /// \returns The writer for the object.
    ObjectWriter writeObject(const char* name);

    ///
    /// Writes a string.
    ///
    /// \param name The name of the member to write to.
    /// \param value The string to write.
    void writeString(const char* name, const std::string& value);

    ///
    /// Writes a signed 8-bit integer.
    ///
    /// \param name The name of the member to write to.
    /// \param value The value to write.
    void writeByte(const char* name, int8_t value);

    ///
    /// Writes an unsigned 8-bit integer.
    ///
    /// \param name The name of the member to write to.
    /// \param value The value to write.
    void writeUnsignedByte(const char* name, uint8_t value);

    ///
    /// Writes a signed 16-bit integer.
    ///
    /// \param name The name of the member to write to.
    /// \param value The value to write.
    void writeShort(const char* name, int16_t value);

    ///
    /// Writes an unsigned 16-bit integer.
    ///
    /// \param name The name of the member to write to.
    /// \param value The value to write.
    void writeUnsignedShort(const char* name, uint16_t value);

    ///
    /// Writes a signed 32-bit integer.
    ///
    /// \param name The name of the member to write to.
    /// \param value The value to write.
    void writeInt(const char* name, int32_t value);

    ///
    /// Writes an unsigned 32-bit integer.
    ///
    /// \param name The name of the member to write to.
    /// \param value The value to write.
    void writeUnsignedInt(const char* name, uint32_t value);

    ///
    /// Writes a signed 64-bit integer.
    ///
    /// \param name The name of the member to write to.
    /// \param value The value to write.
    void writeLong(const char* name, int64_t value);

    ///
    /// Writes an unsigned 64-bit integer.
    ///
    /// \param name The name of the member to write to.
    /// \param value The value to write.
    void writeUnsignedLong(const char* name, uint64_t value);

    ///
    /// Writes a 32-bit float.
    ///
    /// \param name The name of the member to write to.
    /// \param value The value to write.
    void writeFloat(const char* name, float value);

    ///
    /// Writes a 64-bit float
    ///
    /// \param name The name of the member to write to.
    /// \param value The value to write.
    void writeDouble(const char* name, double value);

    ///
    /// Writes a real number.
    ///
    /// \param name The name of the member to write to.
    /// \param value The value to write.
    void writeReal(const char* name, Real value);

    ///
    /// Writes a 2-dimensional vector.
    ///
    /// \param name The name of the member to write to.
    /// \param value The vector to write.
    void writeVector2(const char* name, const Vector2& value);

    ///
    /// Writes a 3-dimensional vector.
    ///
    /// \param name The name of the member to write to.
    /// \param value The vector to write.
    void writeVector3(const char* name, const Vector3& value);

    ///
    /// Writes a 4-dimensional vector.
    ///
    /// \param name The name of the member to write to.
    /// \param value The vector to write.
    void writeVector4(const char* name, const Vector4& value);

private:
    ObjectWriter();
    ObjectWriter(DataWriter* writer);

    DataWriter* _writer;
};

///
/// Provides abstract access for writing structured data.
class DataWriter
{
    friend class ArrayWriter;
    friend class ObjectWriter;
public:

    ///
    /// Returns whether the resulting data is human-readable.
    virtual bool isHumanReadable() const = 0;

    ///
    /// Begins writing an array.
    ///
    /// \returns The writer for the array.
    virtual ArrayWriter writeArray() = 0;

    ///
    /// Begins writing an object.
    ///
    /// \returns The writer for the object.
    virtual ObjectWriter writeObject() = 0;

protected:
    virtual void beginArray() = 0;
    virtual void beginArray(const char* name) = 0;
    virtual void endArray() = 0;

    virtual void beginObject() = 0;
    virtual void beginObject(const char* name) = 0;
    virtual void endObject() = 0;

    virtual void writeString(const std::string& value) = 0;
    virtual void writeString(const char* name, const std::string& value) = 0;
    virtual void writeByte(int8_t value) = 0;
    virtual void writeByte(const char* name, int8_t value) = 0;
    virtual void writeUnsignedByte(uint8_t value) = 0;
    virtual void writeUnsignedByte(const char* name, uint8_t value) = 0;
    virtual void writeShort(int16_t value) = 0;
    virtual void writeShort(const char* name, int16_t value) = 0;
    virtual void writeUnsignedShort(uint16_t value) = 0;
    virtual void writeUnsignedShort(const char* name, uint16_t value) = 0;
    virtual void writeInt(int32_t value) = 0;
    virtual void writeInt(const char* name, int32_t value) = 0;
    virtual void writeUnsignedInt(uint32_t value) = 0;
    virtual void writeUnsignedInt(const char* name, uint32_t value) = 0;
    virtual void writeLong(int64_t value) = 0;
    virtual void writeLong(const char* name, int64_t value) = 0;
    virtual void writeUnsignedLong(uint64_t value) = 0;
    virtual void writeUnsignedLong(const char* name, uint64_t value) = 0;
    virtual void writeFloat(float value) = 0;
    virtual void writeFloat(const char* name, float value) = 0;
    virtual void writeDouble(double value) = 0;
    virtual void writeDouble(const char* name, double value) = 0;
    virtual void writeReal(Real value) = 0;
    virtual void writeReal(const char* name, Real value) = 0;
    virtual void writeVector2(const Vector2& value) = 0;
    virtual void writeVector2(const char* name, const Vector2& value) = 0;
    virtual void writeVector3(const Vector3& value) = 0;
    virtual void writeVector3(const char* name, const Vector3& value) = 0;
    virtual void writeVector4(const Vector4& value) = 0;
    virtual void writeVector4(const char* name, const Vector4& value) = 0;
};

///
/// Provides access for writing structured data to a data value.
class DataValueWriter :
    public DataWriter
{
public:
    bool isHumanReadable() const;

    ArrayWriter writeArray();
    ObjectWriter writeObject();

    ///
    /// Returns the serialized data values.
    DataValue::Array& dataValues();

private:
    void beginArray();
    void beginArray(const char* name);
    void endArray();

    void beginObject();
    void beginObject(const char* name);
    void endObject();

    void writeString(const std::string& value);
    void writeString(const char* name, const std::string& value);
    void writeByte(int8_t value);
    void writeByte(const char* name, int8_t value);
    void writeUnsignedByte(uint8_t value);
    void writeUnsignedByte(const char* name, uint8_t value);
    void writeShort(int16_t value);
    void writeShort(const char* name, int16_t value);
    void writeUnsignedShort(uint16_t value);
    void writeUnsignedShort(const char* name, uint16_t value);
    void writeInt(int32_t value);
    void writeInt(const char* name, int32_t value);
    void writeUnsignedInt(uint32_t value);
    void writeUnsignedInt(const char* name, uint32_t value);
    void writeLong(int64_t value);
    void writeLong(const char* name, int64_t value);
    void writeUnsignedLong(uint64_t value);
    void writeUnsignedLong(const char* name, uint64_t value);
    void writeFloat(float value);
    void writeFloat(const char* name, float value);
    void writeDouble(double value);
    void writeDouble(const char* name, double value);
    void writeReal(Real value);
    void writeReal(const char* name, Real value);
    void writeVector2(const Vector2& value);
    void writeVector2(const char* name, const Vector2& value);
    void writeVector3(const Vector3& value);
    void writeVector3(const char* name, const Vector3& value);
    void writeVector4(const Vector4& value);
    void writeVector4(const char* name, const Vector4& value);

    void _write(const DataValue& value);
    void _write(const char* name, const DataValue& value);

    std::stack<std::string> _nameStack;
    std::stack<DataValue> _valueStack;

    DataValue::Array _completed;
};

///
/// Provides access for writing structured data to a binary stream.
class BinaryWriter :
    public DataWriter
{
public:

    ///
    /// Constructs a writer given the stream to serialize to.
    ///
    /// \param stream The stream to serialize to.
    BinaryWriter(WriteStream& stream);

    bool isHumanReadable() const;

    ArrayWriter writeArray();
    ObjectWriter writeObject();

private:
    void beginArray();
    void beginArray(const char* name);
    void endArray();

    void beginObject();
    void beginObject(const char* name);
    void endObject();

    void writeString(const std::string& value);
    void writeString(const char* name, const std::string& value);
    void writeByte(int8_t value);
    void writeByte(const char* name, int8_t value);
    void writeUnsignedByte(uint8_t value);
    void writeUnsignedByte(const char* name, uint8_t value);
    void writeShort(int16_t value);
    void writeShort(const char* name, int16_t value);
    void writeUnsignedShort(uint16_t value);
    void writeUnsignedShort(const char* name, uint16_t value);
    void writeInt(int32_t value);
    void writeInt(const char* name, int32_t value);
    void writeUnsignedInt(uint32_t value);
    void writeUnsignedInt(const char* name, uint32_t value);
    void writeLong(int64_t value);
    void writeLong(const char* name, int64_t value);
    void writeUnsignedLong(uint64_t value);
    void writeUnsignedLong(const char* name, uint64_t value);
    void writeFloat(float value);
    void writeFloat(const char* name, float value);
    void writeDouble(double value);
    void writeDouble(const char* name, double value);
    void writeReal(Real value);
    void writeReal(const char* name, Real value);
    void writeVector2(const Vector2& value);
    void writeVector2(const char* name, const Vector2& value);
    void writeVector3(const Vector3& value);
    void writeVector3(const char* name, const Vector3& value);
    void writeVector4(const Vector4& value);
    void writeVector4(const char* name, const Vector4& value);

    std::stack<size_t> _countPositionStack;
    std::stack<unsigned> _countStack;

    WriteStream* _stream;
};
}