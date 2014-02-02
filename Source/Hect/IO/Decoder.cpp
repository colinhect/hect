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
#include "Decoder.h"

using namespace hect;

ArrayDecoder::ArrayDecoder(const ArrayDecoder& decoder) :
    _decoder(nullptr)
{
    decoder;
}

ArrayDecoder::ArrayDecoder(ArrayDecoder&& decoder) :
    _decoder(decoder._decoder)
{
    decoder._decoder = nullptr;
}

ArrayDecoder::~ArrayDecoder()
{
    if (_decoder)
    {
        _decoder->endArray();
    }
}

bool ArrayDecoder::isBinaryStream() const
{
    assert(_decoder);
    return _decoder->isBinaryStream();
}

ReadStream& ArrayDecoder::binaryStream()
{
    assert(_decoder);
    return _decoder->binaryStream();
}

bool ArrayDecoder::hasMoreElements() const
{
    assert(_decoder);
    return _decoder->hasMoreElements();
}

ArrayDecoder ArrayDecoder::decodeArray()
{
    assert(_decoder);
    _decoder->beginArray();
    return ArrayDecoder(_decoder);
}

ObjectDecoder ArrayDecoder::decodeObject()
{
    assert(_decoder);
    _decoder->beginObject();
    return ObjectDecoder(_decoder);
}

std::string ArrayDecoder::decodeString()
{
    assert(_decoder);
    return _decoder->decodeString();
}

int8_t ArrayDecoder::decodeByte()
{
    assert(_decoder);
    return _decoder->decodeByte();
}

uint8_t ArrayDecoder::decodeUnsignedByte()
{
    assert(_decoder);
    return _decoder->decodeUnsignedByte();
}

int16_t ArrayDecoder::decodeShort()
{
    assert(_decoder);
    return _decoder->decodeShort();
}

uint16_t ArrayDecoder::decodeUnsignedShort()
{
    assert(_decoder);
    return _decoder->decodeUnsignedShort();
}

int32_t ArrayDecoder::decodeInt()
{
    assert(_decoder);
    return _decoder->decodeInt();
}

uint32_t ArrayDecoder::decodeUnsignedInt()
{
    assert(_decoder);
    return _decoder->decodeUnsignedInt();
}

int64_t ArrayDecoder::decodeLong()
{
    assert(_decoder);
    return _decoder->decodeLong();
}

uint64_t ArrayDecoder::decodeUnsignedLong()
{
    assert(_decoder);
    return _decoder->decodeUnsignedLong();
}

float ArrayDecoder::decodeFloat()
{
    assert(_decoder);
    return _decoder->decodeFloat();
}

double ArrayDecoder::decodeDouble()
{
    assert(_decoder);
    return _decoder->decodeDouble();
}

Real ArrayDecoder::decodeReal()
{
    assert(_decoder);
    return _decoder->decodeReal();
}

bool ArrayDecoder::decodeBool()
{
    assert(_decoder);
    return _decoder->decodeBool();
}

Vector2 ArrayDecoder::decodeVector2()
{
    assert(_decoder);
    return _decoder->decodeVector2();
}

Vector3 ArrayDecoder::decodeVector3()
{
    assert(_decoder);
    return _decoder->decodeVector3();
}

Vector4 ArrayDecoder::decodeVector4()
{
    assert(_decoder);
    return _decoder->decodeVector4();
}

ArrayDecoder::ArrayDecoder() :
    _decoder(nullptr)
{
}

ArrayDecoder::ArrayDecoder(Decoder* decoder) :
    _decoder(decoder)
{
}

ObjectDecoder::ObjectDecoder(const ObjectDecoder& decoder) :
    _decoder(nullptr)
{
    decoder;
}

ObjectDecoder::ObjectDecoder(ObjectDecoder&& decoder) :
    _decoder(decoder._decoder)
{
    decoder._decoder = nullptr;
}

ObjectDecoder::~ObjectDecoder()
{
    if (_decoder)
    {
        _decoder->endObject();
    }
}

bool ObjectDecoder::isBinaryStream() const
{
    assert(_decoder);
    return _decoder->isBinaryStream();
}

ReadStream& ObjectDecoder::binaryStream()
{
    assert(_decoder);
    return _decoder->binaryStream();
}

bool ObjectDecoder::hasMember(const char* name) const
{
    return _decoder->hasMember(name);
}

ArrayDecoder ObjectDecoder::decodeArray(const char* name)
{
    assert(_decoder);
    _decoder->beginArray(name);
    return ArrayDecoder(_decoder);
}

ObjectDecoder ObjectDecoder::decodeObject(const char* name)
{
    assert(_decoder);
    _decoder->beginObject(name);
    return ObjectDecoder(_decoder);
}

std::string ObjectDecoder::decodeString(const char* name)
{
    assert(_decoder);
    return _decoder->decodeString(name);
}

int8_t ObjectDecoder::decodeByte(const char* name)
{
    assert(_decoder);
    return _decoder->decodeByte(name);
}

uint8_t ObjectDecoder::decodeUnsignedByte(const char* name)
{
    assert(_decoder);
    return _decoder->decodeUnsignedByte(name);
}

int16_t ObjectDecoder::decodeShort(const char* name)
{
    assert(_decoder);
    return _decoder->decodeShort(name);
}

uint16_t ObjectDecoder::decodeUnsignedShort(const char* name)
{
    assert(_decoder);
    return _decoder->decodeUnsignedShort(name);
}

int32_t ObjectDecoder::decodeInt(const char* name)
{
    assert(_decoder);
    return _decoder->decodeInt(name);
}

uint32_t ObjectDecoder::decodeUnsignedInt(const char* name)
{
    assert(_decoder);
    return _decoder->decodeUnsignedInt(name);
}

int64_t ObjectDecoder::decodeLong(const char* name)
{
    assert(_decoder);
    return _decoder->decodeLong(name);
}

uint64_t ObjectDecoder::decodeUnsignedLong(const char* name)
{
    assert(_decoder);
    return _decoder->decodeUnsignedLong(name);
}

float ObjectDecoder::decodeFloat(const char* name)
{
    assert(_decoder);
    return _decoder->decodeFloat(name);
}

double ObjectDecoder::decodeDouble(const char* name)
{
    assert(_decoder);
    return _decoder->decodeDouble(name);
}

Real ObjectDecoder::decodeReal(const char* name)
{
    assert(_decoder);
    return _decoder->decodeReal(name);
}

bool ObjectDecoder::decodeBool(const char* name)
{
    assert(_decoder);
    return _decoder->decodeBool(name);
}

Vector2 ObjectDecoder::decodeVector2(const char* name)
{
    assert(_decoder);
    return _decoder->decodeVector2(name);
}

Vector3 ObjectDecoder::decodeVector3(const char* name)
{
    assert(_decoder);
    return _decoder->decodeVector3(name);
}

Vector4 ObjectDecoder::decodeVector4(const char* name)
{
    assert(_decoder);
    return _decoder->decodeVector4(name);
}

ObjectDecoder::ObjectDecoder() :
    _decoder(nullptr)
{
}

ObjectDecoder::ObjectDecoder(Decoder* decoder) :
    _decoder(decoder)
{
}