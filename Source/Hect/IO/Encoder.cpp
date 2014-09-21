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

void ArrayEncoder::encodeInt8(int8_t value)
{
    assert(_encoder);
    _encoder->encodeInt8(value);
}

void ArrayEncoder::encodeUInt8(uint8_t value)
{
    assert(_encoder);
    _encoder->encodeUInt8(value);
}

void ArrayEncoder::encodeInt16(int16_t value)
{
    assert(_encoder);
    _encoder->encodeInt16(value);
}

void ArrayEncoder::encodeUInt16(uint16_t value)
{
    assert(_encoder);
    _encoder->encodeUInt16(value);
}

void ArrayEncoder::encodeInt32(int32_t value)
{
    assert(_encoder);
    _encoder->encodeInt32(value);
}

void ArrayEncoder::encodeUInt32(uint32_t value)
{
    assert(_encoder);
    _encoder->encodeUInt32(value);
}

void ArrayEncoder::encodeInt64(int64_t value)
{
    assert(_encoder);
    _encoder->encodeInt64(value);
}

void ArrayEncoder::encodeUInt64(uint64_t value)
{
    assert(_encoder);
    _encoder->encodeUInt64(value);
}

void ArrayEncoder::encodeFloat32(float value)
{
    assert(_encoder);
    _encoder->encodeFloat32(value);
}

void ArrayEncoder::encodeFloat64(double value)
{
    assert(_encoder);
    _encoder->encodeFloat64(value);
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

void ObjectEncoder::encodeInt8(const char* name, int8_t value)
{
    assert(_encoder);
    _encoder->encodeInt8(name, value);
}

void ObjectEncoder::encodeUInt8(const char* name, uint8_t value)
{
    assert(_encoder);
    _encoder->encodeUInt8(name, value);
}

void ObjectEncoder::encodeInt16(const char* name, int16_t value)
{
    assert(_encoder);
    _encoder->encodeInt16(name, value);
}

void ObjectEncoder::encodeUInt16(const char* name, uint16_t value)
{
    assert(_encoder);
    _encoder->encodeUInt16(name, value);
}

void ObjectEncoder::encodeInt32(const char* name, int32_t value)
{
    assert(_encoder);
    _encoder->encodeInt32(name, value);
}

void ObjectEncoder::encodeUInt32(const char* name, uint32_t value)
{
    assert(_encoder);
    _encoder->encodeUInt32(name, value);
}

void ObjectEncoder::encodeInt64(const char* name, int64_t value)
{
    assert(_encoder);
    _encoder->encodeInt64(name, value);
}

void ObjectEncoder::encodeUInt64(const char* name, uint64_t value)
{
    assert(_encoder);
    _encoder->encodeUInt64(name, value);
}

void ObjectEncoder::encodeFloat32(const char* name, float value)
{
    assert(_encoder);
    _encoder->encodeFloat32(name, value);
}

void ObjectEncoder::encodeFloat64(const char* name, double value)
{
    assert(_encoder);
    _encoder->encodeFloat64(name, value);
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