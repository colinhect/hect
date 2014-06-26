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
#include "Encoder.h"

using namespace hect;

ArrayEncoder::ArrayEncoder(const ArrayEncoder& encoder) :
    _encoder(nullptr)
{
    encoder;
}

ArrayEncoder::ArrayEncoder(ArrayEncoder&& encoder) :
    _encoder(encoder._encoder)
{
    encoder._encoder = nullptr;
}

ArrayEncoder::~ArrayEncoder()
{
    if (_encoder)
    {
        _encoder->endArray();
    }
}

bool ArrayEncoder::isBinaryStream() const
{
    assert(_encoder);
    return _encoder->isBinaryStream();
}

WriteStream& ArrayEncoder::binaryStream()
{
    assert(_encoder);
    return _encoder->binaryStream();
}

ArrayEncoder ArrayEncoder::encodeArray()
{
    assert(_encoder);
    _encoder->beginArray();
    return ArrayEncoder(_encoder);
}

ObjectEncoder ArrayEncoder::encodeObject()
{
    assert(_encoder);
    _encoder->beginObject();
    return ObjectEncoder(_encoder);
}

void ArrayEncoder::encodeString(const std::string& value)
{
    assert(_encoder);
    _encoder->encodeString(value);
}

void ArrayEncoder::encodeByte(int8_t value)
{
    assert(_encoder);
    _encoder->encodeByte(value);
}

void ArrayEncoder::encodeUnsignedByte(uint8_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedByte(value);
}

void ArrayEncoder::encodeShort(int16_t value)
{
    assert(_encoder);
    _encoder->encodeShort(value);
}

void ArrayEncoder::encodeUnsignedShort(uint16_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedShort(value);
}

void ArrayEncoder::encodeInt(int32_t value)
{
    assert(_encoder);
    _encoder->encodeInt(value);
}

void ArrayEncoder::encodeUnsignedInt(uint32_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedInt(value);
}

void ArrayEncoder::encodeLong(int64_t value)
{
    assert(_encoder);
    _encoder->encodeLong(value);
}

void ArrayEncoder::encodeUnsignedLong(uint64_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedLong(value);
}

void ArrayEncoder::encodeFloat(float value)
{
    assert(_encoder);
    _encoder->encodeFloat(value);
}

void ArrayEncoder::encodeDouble(double value)
{
    assert(_encoder);
    _encoder->encodeDouble(value);
}

void ArrayEncoder::encodeReal(Real value)
{
    assert(_encoder);
    _encoder->encodeReal(value);
}

void ArrayEncoder::encodeBool(bool value)
{
    assert(_encoder);
    _encoder->encodeBool(value);
}

void ArrayEncoder::encodeVector2(const Vector2& value)
{
    assert(_encoder);
    _encoder->encodeVector2(value);
}

void ArrayEncoder::encodeVector3(const Vector3& value)
{
    assert(_encoder);
    _encoder->encodeVector3(value);
}

void ArrayEncoder::encodeVector4(const Vector4& value)
{
    assert(_encoder);
    _encoder->encodeVector4(value);
}

ArrayEncoder::ArrayEncoder() :
    _encoder(nullptr)
{
}

ArrayEncoder::ArrayEncoder(Encoder* encoder) :
    _encoder(encoder)
{
}

ObjectEncoder::ObjectEncoder(const ObjectEncoder& encoder) :
    _encoder(nullptr)
{
    encoder;
}

ObjectEncoder::ObjectEncoder(ObjectEncoder&& encoder) :
    _encoder(encoder._encoder)
{
    encoder._encoder = nullptr;
}

ObjectEncoder::~ObjectEncoder()
{
    if (_encoder)
    {
        _encoder->endObject();
    }
}

bool ObjectEncoder::isBinaryStream() const
{
    assert(_encoder);
    return _encoder->isBinaryStream();
}

WriteStream& ObjectEncoder::binaryStream()
{
    assert(_encoder);
    return _encoder->binaryStream();
}

ArrayEncoder ObjectEncoder::encodeArray(const char* name)
{
    assert(_encoder);
    _encoder->beginArray(name);
    return ArrayEncoder(_encoder);
}

ObjectEncoder ObjectEncoder::encodeObject(const char* name)
{
    assert(_encoder);
    _encoder->beginObject(name);
    return ObjectEncoder(_encoder);
}

void ObjectEncoder::encodeString(const char* name, const std::string& value)
{
    assert(_encoder);
    _encoder->encodeString(name, value);
}

void ObjectEncoder::encodeByte(const char* name, int8_t value)
{
    assert(_encoder);
    _encoder->encodeByte(name, value);
}

void ObjectEncoder::encodeUnsignedByte(const char* name, uint8_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedByte(name, value);
}

void ObjectEncoder::encodeShort(const char* name, int16_t value)
{
    assert(_encoder);
    _encoder->encodeShort(name, value);
}

void ObjectEncoder::encodeUnsignedShort(const char* name, uint16_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedShort(name, value);
}

void ObjectEncoder::encodeInt(const char* name, int32_t value)
{
    assert(_encoder);
    _encoder->encodeInt(name, value);
}

void ObjectEncoder::encodeUnsignedInt(const char* name, uint32_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedInt(name, value);
}

void ObjectEncoder::encodeLong(const char* name, int64_t value)
{
    assert(_encoder);
    _encoder->encodeLong(name, value);
}

void ObjectEncoder::encodeUnsignedLong(const char* name, uint64_t value)
{
    assert(_encoder);
    _encoder->encodeUnsignedLong(name, value);
}

void ObjectEncoder::encodeFloat(const char* name, float value)
{
    assert(_encoder);
    _encoder->encodeFloat(name, value);
}

void ObjectEncoder::encodeDouble(const char* name, double value)
{
    assert(_encoder);
    _encoder->encodeDouble(name, value);
}

void ObjectEncoder::encodeReal(const char* name, Real value)
{
    assert(_encoder);
    _encoder->encodeReal(name, value);
}

void ObjectEncoder::encodeBool(const char* name, bool value)
{
    assert(_encoder);
    _encoder->encodeBool(name, value);
}

void ObjectEncoder::encodeVector2(const char* name, const Vector2& value)
{
    assert(_encoder);
    _encoder->encodeVector2(name, value);
}

void ObjectEncoder::encodeVector3(const char* name, const Vector3& value)
{
    assert(_encoder);
    _encoder->encodeVector3(name, value);
}

void ObjectEncoder::encodeVector4(const char* name, const Vector4& value)
{
    assert(_encoder);
    _encoder->encodeVector4(name, value);
}

ObjectEncoder::ObjectEncoder() :
    _encoder(nullptr)
{
}

ObjectEncoder::ObjectEncoder(Encoder* encoder) :
    _encoder(encoder)
{
}