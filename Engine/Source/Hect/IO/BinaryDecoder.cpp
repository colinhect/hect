///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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
#include "BinaryDecoder.h"

#include "Hect/Core/Exception.h"
#include "Hect/IO/MemoryReadStream.h"

using namespace hect;

BinaryDecoder::BinaryDecoder(ReadStream& stream) :
    _stream(stream)
{
}

BinaryDecoder::BinaryDecoder(ReadStream& stream, AssetCache& asset_cache) :
    Decoder(asset_cache),
    _stream(stream)
{
}

BinaryDecoder::BinaryDecoder(const ByteVector& data) :
    _owned_stream(new MemoryReadStream(data)),
    _stream(*_owned_stream)
{
}

bool BinaryDecoder::is_binary_stream() const
{
    return true;
}

ReadStream& BinaryDecoder::binary_stream()
{
    return _stream;
}

void BinaryDecoder::begin_array()
{
    increment_index();

    uint32_t count;
    _stream >> count;

    _value_type_stack.push(ValueType_Array);
    _count_stack.push(count);
    _index_stack.push(0);
}

void BinaryDecoder::end_array()
{
    _index_stack.pop();
    _count_stack.pop();
    _value_type_stack.pop();
}

bool BinaryDecoder::has_more_elements() const
{
    return _index_stack.top() < _count_stack.top();
}

void BinaryDecoder::begin_object()
{
    increment_index();

    _value_type_stack.push(ValueType_Object);
}

void BinaryDecoder::end_object()
{
    _value_type_stack.pop();
}

bool BinaryDecoder::select_member(const char* name)
{
    (void)name;
    return true;
}

std::vector<std::string> BinaryDecoder::member_names() const
{
    throw InvalidOperation("Cannot enumerate member names from a binary data source");
}

std::string BinaryDecoder::decode_string()
{
    increment_index();

    std::string value;
    _stream >> value;
    return value;
}

int8_t BinaryDecoder::decode_int8()
{
    increment_index();

    int8_t value;
    _stream >> value;
    return value;
}

uint8_t BinaryDecoder::decode_uint8()
{
    increment_index();

    uint8_t value;
    _stream >> value;
    return value;
}

int16_t BinaryDecoder::decode_int16()
{
    increment_index();

    int16_t value;
    _stream >> value;
    return value;
}

uint16_t BinaryDecoder::decode_uint16()
{
    increment_index();

    uint16_t value;
    _stream >> value;
    return value;
}

int32_t BinaryDecoder::decode_int32()
{
    increment_index();

    int32_t value;
    _stream >> value;
    return value;
}

uint32_t BinaryDecoder::decode_uint32()
{
    increment_index();

    uint32_t value;
    _stream >> value;
    return value;
}

int64_t BinaryDecoder::decode_int64()
{
    increment_index();

    int64_t value;
    _stream >> value;
    return value;
}

uint64_t BinaryDecoder::decode_uint64()
{
    increment_index();

    uint64_t value;
    _stream >> value;
    return value;
}

float BinaryDecoder::decode_float32()
{
    increment_index();

    float value;
    _stream >> value;
    return value;
}

double BinaryDecoder::decode_float64()
{
    increment_index();

    double value;
    _stream >> value;
    return value;
}

bool BinaryDecoder::decode_bool()
{
    increment_index();

    bool value;
    _stream >> value;
    return value;
}

void BinaryDecoder::increment_index()
{
    if (!_value_type_stack.empty() && _value_type_stack.top() == ValueType_Array)
    {
        ++_index_stack.top();
    }
}