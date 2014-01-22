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

#include "Hect/Core/DataValue.h"
#include "Hect/IO/ReadStream.h"

namespace hect
{

///
/// Provides access for reading an array.
class ArrayReader
{
    friend class DataReader;
    friend class DataValueReader;
    friend class BinaryReader;

    friend class ObjectReader;
public:
    ArrayReader(const ArrayReader& reader);
    ArrayReader(ArrayReader&& reader);
    ~ArrayReader();

    ///
    /// Returns whether the reader has reached the end of the array.
    bool hasMoreElements() const;

    ///
    /// Begins reading an array.
    ///
    /// \returns The reader for the array.
    ArrayReader readArray();

    ///
    /// Begins reading an object.
    ///
    /// \returns The reader for the object.
    ObjectReader readObject();

    ///
    /// Reads a string.
    std::string readString();

    ///
    /// Reads a signed 8-bit integer.
    int8_t readByte();

    ///
    /// Reads an unsigned 16-bit integer.
    uint8_t readUnsignedByte();

    ///
    /// Reads a signed 16-bit integer.
    int16_t readShort();

    ///
    /// Reads an unsigned 16-bit integer.
    uint16_t readUnsignedShort();

    ///
    /// Reads a signed 32-bit integer.
    int32_t readInt();

    ///
    /// Reads an unsigned 32-bit integer.
    uint32_t readUnsignedInt();

    ///
    /// Reads a signed 64-bit integer.
    int64_t readLong();

    ///
    /// Reads an unsigned 64-bit integer.
    uint64_t readUnsignedLong();

    ///
    /// Reads a 32-bit float.
    float readFloat();

    ///
    /// Reads a 64-bit float.
    double readDouble();

    ///
    /// Reads a real number.
    Real readReal();
    
    ///
    /// Reads bool as an unsigned 8-bit integer.
    bool readBool();

    ///
    /// Reads a 2-dimensional vector.
    Vector2 readVector2();

    ///
    /// Reads a 3-dimensional vector.
    Vector3 readVector3();

    ///
    /// Reads a 4-dimensional vector.
    Vector4 readVector4();

private:
    ArrayReader();
    ArrayReader(DataReader* reader);

    DataReader* _reader;
};

///
/// Provides access for reading an object.
class ObjectReader
{
    friend class DataReader;
    friend class DataValueReader;
    friend class BinaryReader;

    friend class ArrayReader;
public:
    ObjectReader(const ObjectReader& reader);
    ObjectReader(ObjectReader&& reader);
    ~ObjectReader();

    ///
    /// Returns whether the object has a member with a specific name.
    ///
    /// \param name The name of the member in question.
    bool hasMember(const char* name) const;

    ///
    /// Begins reading an array.
    ///
    /// \param name The name of the member to begin reading.
    ///
    /// \returns The reader for the array.
    ArrayReader readArray(const char* name);

    ///
    /// Begins reading an object.
    ///
    /// \param name The name of the member to begin reading.
    ///
    /// \returns The reader for the object.
    ObjectReader readObject(const char* name);

    ///
    /// Reads a string.
    ///
    /// \param name The name of the member to read.
    std::string readString(const char* name);

    ///
    /// Reads a signed 8-bit integer.
    ///
    /// \param name The name of the member to read.
    int8_t readByte(const char* name);

    ///
    /// Reads an unsigned 16-bit integer.
    ///
    /// \param name The name of the member to read.
    uint8_t readUnsignedByte(const char* name);

    ///
    /// Reads a signed 16-bit integer.
    ///
    /// \param name The name of the member to read.
    int16_t readShort(const char* name);

    ///
    /// Reads an unsigned 16-bit integer.
    ///
    /// \param name The name of the member to read.
    uint16_t readUnsignedShort(const char* name);

    ///
    /// Reads a signed 32-bit integer.
    ///
    /// \param name The name of the member to read.
    int32_t readInt(const char* name);

    ///
    /// Reads an unsigned 32-bit integer.
    ///
    /// \param name The name of the member to read.
    uint32_t readUnsignedInt(const char* name);

    ///
    /// Reads a signed 64-bit integer.
    ///
    /// \param name The name of the member to read.
    int64_t readLong(const char* name);

    ///
    /// Reads an unsigned 64-bit integer.
    ///
    /// \param name The name of the member to read.
    uint64_t readUnsignedLong(const char* name);

    ///
    /// Reads a 32-bit float.
    ///
    /// \param name The name of the member to read.
    float readFloat(const char* name);

    ///
    /// Reads a 64-bit float.
    ///
    /// \param name The name of the member to read.
    double readDouble(const char* name);

    ///
    /// Reads a real number.
    ///
    /// \param name The name of the member to read.
    Real readReal(const char* name);
    
    ///
    /// Reads bool as an unsigned 8-bit integer.
    ///
    /// \param name The name of the member to read
    bool readBool(const char* name);

    ///
    /// Reads a 2-dimensional vector.
    ///
    /// \param name The name of the member to read.
    Vector2 readVector2(const char* name);

    ///
    /// Reads a 3-dimensional vector.
    ///
    /// \param name The name of the member to read.
    Vector3 readVector3(const char* name);

    ///
    /// Reads a 4-dimensional vector.
    ///
    /// \param name The name of the member to read.
    Vector4 readVector4(const char* name);

private:
    ObjectReader();
    ObjectReader(DataReader* reader);

    DataReader* _reader;
};

///
/// Provides abstract access for reading structured data.
class DataReader
{
    friend class ArrayReader;
    friend class ObjectReader;
public:

    ///
    /// Returns whether the source data is human-readable.
    virtual bool isHumanReadable() const = 0;

    ///
    /// Begins reading an array.
    ///
    /// \returns The reader for the array.
    virtual ArrayReader readArray() = 0;

    ///
    /// Begins reading an object.
    ///
    /// \returns The reader for the object.
    virtual ObjectReader readObject() = 0;

protected:
    virtual void beginArray() = 0;
    virtual void beginArray(const char* name) = 0;
    virtual void endArray() = 0;

    virtual bool hasMoreElements() const = 0;

    virtual void beginObject() = 0;
    virtual void beginObject(const char* name) = 0;
    virtual void endObject() = 0;

    virtual bool hasMember(const char* name) const = 0;

    virtual std::string readString() = 0;
    virtual std::string readString(const char* name) = 0;
    virtual int8_t readByte() = 0;
    virtual int8_t readByte(const char* name) = 0;
    virtual uint8_t readUnsignedByte() = 0;
    virtual uint8_t readUnsignedByte(const char* name) = 0;
    virtual int16_t readShort() = 0;
    virtual int16_t readShort(const char* name) = 0;
    virtual uint16_t readUnsignedShort() = 0;
    virtual uint16_t readUnsignedShort(const char* name) = 0;
    virtual int32_t readInt() = 0;
    virtual int32_t readInt(const char* name) = 0;
    virtual uint32_t readUnsignedInt() = 0;
    virtual uint32_t readUnsignedInt(const char* name) = 0;
    virtual int64_t readLong() = 0;
    virtual int64_t readLong(const char* name) = 0;
    virtual uint64_t readUnsignedLong() = 0;
    virtual uint64_t readUnsignedLong(const char* name) = 0;
    virtual float readFloat() = 0;
    virtual float readFloat(const char* name) = 0;
    virtual double readDouble() = 0;
    virtual double readDouble(const char* name) = 0;
    virtual Real readReal() = 0;
    virtual Real readReal(const char* name) = 0;
    virtual bool readBool() = 0;
    virtual bool readBool(const char* name) = 0;
    virtual Vector2 readVector2() = 0;
    virtual Vector2 readVector2(const char* name) = 0;
    virtual Vector3 readVector3() = 0;
    virtual Vector3 readVector3(const char* name) = 0;
    virtual Vector4 readVector4() = 0;
    virtual Vector4 readVector4(const char* name) = 0;
};

///
/// Provides access for reading structured data from a data value.
class DataValueReader :
    public DataReader
{
public:

    ///
    /// Constructs a reader given the data value to deserialize.
    ///
    /// \param dataValue The data value to deserialize.
    DataValueReader(const DataValue& dataValue);

    bool isHumanReadable() const;

    ArrayReader readArray();
    ObjectReader readObject();

private:
    void beginArray();
    void beginArray(const char* name);
    void endArray();

    bool hasMoreElements() const;

    void beginObject();
    void beginObject(const char* name);
    void endObject();

    bool hasMember(const char* name) const;

    std::string readString();
    std::string readString(const char* name);
    int8_t readByte();
    int8_t readByte(const char* name);
    uint8_t readUnsignedByte();
    uint8_t readUnsignedByte(const char* name);
    int16_t readShort();
    int16_t readShort(const char* name);
    uint16_t readUnsignedShort();
    uint16_t readUnsignedShort(const char* name);
    int32_t readInt();
    int32_t readInt(const char* name);
    uint32_t readUnsignedInt();
    uint32_t readUnsignedInt(const char* name);
    int64_t readLong();
    int64_t readLong(const char* name);
    uint64_t readUnsignedLong();
    uint64_t readUnsignedLong(const char* name);
    float readFloat();
    float readFloat(const char* name);
    double readDouble();
    double readDouble(const char* name);
    Real readReal();
    Real readReal(const char* name);
    bool readBool();
    bool readBool(const char* name);
    Vector2 readVector2();
    Vector2 readVector2(const char* name);
    Vector3 readVector3();
    Vector3 readVector3(const char* name);
    Vector4 readVector4();
    Vector4 readVector4(const char* name);

    const DataValue& _read();
    const DataValue& _read(const char* name);

    std::stack<size_t> _indexStack;
    std::stack<DataValue> _valueStack;
};

///
/// Provides access for reading structured data from a binary stream.
class BinaryReader :
    public DataReader
{
public:

    ///
    /// Constructs a reader given the stream to deserialize from.
    ///
    /// \param stream The stream to deserialize from.
    BinaryReader(ReadStream& stream);

    bool isHumanReadable() const;

    ArrayReader readArray();
    ObjectReader readObject();

private:
    void beginArray();
    void beginArray(const char* name);
    void endArray();

    bool hasMoreElements() const;

    void beginObject();
    void beginObject(const char* name);
    void endObject();

    bool hasMember(const char* name) const;

    std::string readString();
    std::string readString(const char* name);
    int8_t readByte();
    int8_t readByte(const char* name);
    uint8_t readUnsignedByte();
    uint8_t readUnsignedByte(const char* name);
    int16_t readShort();
    int16_t readShort(const char* name);
    uint16_t readUnsignedShort();
    uint16_t readUnsignedShort(const char* name);
    int32_t readInt();
    int32_t readInt(const char* name);
    uint32_t readUnsignedInt();
    uint32_t readUnsignedInt(const char* name);
    int64_t readLong();
    int64_t readLong(const char* name);
    uint64_t readUnsignedLong();
    uint64_t readUnsignedLong(const char* name);
    float readFloat();
    float readFloat(const char* name);
    double readDouble();
    double readDouble(const char* name);
    Real readReal();
    Real readReal(const char* name);
    bool readBool();
    bool readBool(const char* name);
    Vector2 readVector2();
    Vector2 readVector2(const char* name);
    Vector3 readVector3();
    Vector3 readVector3(const char* name);
    Vector4 readVector4();
    Vector4 readVector4(const char* name);

    std::stack<unsigned> _indexStack;
    std::stack<unsigned> _countStack;
    ReadStream* _stream;
};
}