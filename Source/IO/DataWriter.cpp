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
#include "Hect.h"

using namespace hect;

DataValueWriter::DataValueWriter()
{
    _valueStack.push(DataValue(DataValueType::Object));
}

DataValue DataValueWriter::currentDataValue() const
{
    return _valueStack.top();
}

void DataValueWriter::beginObject()
{
    DataValue& top = _valueStack.top();
    if (!top.isArray())
    {
        throw Error("Cannot begin an unnamed object when the current value is an object");
    }

    _valueStack.push(DataValue(DataValueType::Object));
}

void DataValueWriter::beginObject(const char* name)
{
    DataValue& top = _valueStack.top();
    if (!top.isObject())
    {
        throw Error("Cannot begin a named object when the current value is an array");
    }

    _nameStack.push(name);
    _valueStack.push(DataValue(DataValueType::Object));
}

void DataValueWriter::endObject()
{
    DataValue top = _valueStack.top();
    if (!top.isObject())
    {
        throw Error("Current value is not an object");
    }

    _valueStack.pop();
    if (_valueStack.top().isObject())
    {
        std::string name = _nameStack.top();
        _nameStack.pop();
        _valueStack.top().addMember(name, top);
    }
    else
    {
        _valueStack.top().addElement(top);
    }
}

void DataValueWriter::beginArray(const char* name)
{
    DataValue& top = _valueStack.top();
    if (!top.isObject())
    {
        throw Error("Cannot begin a named array when the current value is an array");
    }

    _nameStack.push(name);
    _valueStack.push(DataValue(DataValueType::Array));
}

void DataValueWriter::endArray()
{
    DataValue top = _valueStack.top();
    if (!top.isArray())
    {
        throw Error("Current value is not an array");
    }

    std::string name = _nameStack.top();
    _nameStack.pop();

    _valueStack.pop();
    _valueStack.top().addMember(name, top);
}

void DataValueWriter::writeDouble(double value)
{
    _write(value);
}

void DataValueWriter::writeDouble(const char* name, double value)
{
    _write(name, value);
}

void DataValueWriter::writeString(const std::string& value)
{
    _write(value);
}

void DataValueWriter::writeString(const char* name, const std::string& value)
{
    _write(name, value);
}

void DataValueWriter::writeVector2(const Vector2<>& value)
{
    _write(value);
}

void DataValueWriter::writeVector2(const char* name, const Vector2<>& value)
{
    _write(name, value);
}

void DataValueWriter::writeVector3(const Vector3<>& value)
{
    _write(value);
}

void DataValueWriter::writeVector3(const char* name, const Vector3<>& value)
{
    _write(name, value);
}

void DataValueWriter::writeVector4(const Vector4<>& value)
{
    _write(value);
}

void DataValueWriter::writeVector4(const char* name, const Vector4<>& value)
{
    _write(name, value);
}

void DataValueWriter::_write(const DataValue& value)
{
    DataValue& top = _valueStack.top();
    if (top.isObject())
    {
        throw Error("Cannot write an unnamed value when the current value is an object");
    }
    top.addElement(value);
}

void DataValueWriter::_write(const char* name, const DataValue& value)
{
    DataValue& top = _valueStack.top();
    if (!top.isObject())
    {
        throw Error("Cannot write a named value when the current value is an array");
    }
    top.addMember(name, value);
}

BinaryDataWriter::BinaryDataWriter(WriteStream& stream) :
    _elementCountPosition(0),
    _elementCount(0),
    _stream(&stream)
{
}

void BinaryDataWriter::beginObject()
{
    ++_elementCount;
}

void BinaryDataWriter::beginObject(const char* name)
{
    name;
}

void BinaryDataWriter::endObject()
{
}

void BinaryDataWriter::beginArray(const char* name)
{
    name;
    _elementCount = 0;
    _elementCountPosition = _stream->position();
    _stream->writeUnsignedInt(0);
}

void BinaryDataWriter::endArray()
{
    size_t currentPosition = _stream->position();
    _stream->seek(_elementCountPosition);
    _stream->writeUnsignedInt(_elementCount);
    _stream->seek(currentPosition);
}

void BinaryDataWriter::writeDouble(double value)
{
    ++_elementCount;
    _stream->writeDouble(value);
}

void BinaryDataWriter::writeDouble(const char* name, double value)
{
    name;
    _stream->writeDouble(value);
}

void BinaryDataWriter::writeString(const std::string& value)
{
    ++_elementCount;
    _stream->writeString(value);
}

void BinaryDataWriter::writeString(const char* name, const std::string& value)
{
    name;
    _stream->writeString(value);
}

void BinaryDataWriter::writeVector2(const Vector2<>& value)
{
    ++_elementCount;
    _stream->writeVector2(value);
}

void BinaryDataWriter::writeVector2(const char* name, const Vector2<>& value)
{
    name;
    _stream->writeVector2(value);
}

void BinaryDataWriter::writeVector3(const Vector3<>& value)
{
    ++_elementCount;
    _stream->writeVector3(value);
}

void BinaryDataWriter::writeVector3(const char* name, const Vector3<>& value)
{
    name;
    _stream->writeVector3(value);
}

void BinaryDataWriter::writeVector4(const Vector4<>& value)
{
    ++_elementCount;
    _stream->writeVector4(value);
}

void BinaryDataWriter::writeVector4(const char* name, const Vector4<>& value)
{
    name;
    _stream->writeVector4(value);
}