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
#include "BinaryEncoder.h"

#include "Hect/IO/MemoryWriteStream.h"

using namespace hect;

BinaryEncoder::BinaryEncoder(WriteStream& stream) :
    _stream(stream)
{
}

BinaryEncoder::BinaryEncoder(ByteVector& data) :
    _owned_stream(new MemoryWriteStream(data)),
    _stream(*_owned_stream)
{
}

bool BinaryEncoder::is_binary_stream() const
{
    return true;
}

WriteStream& BinaryEncoder::binary_stream()
{
    return _stream;
}

void BinaryEncoder::begin_array()
{
    increment_count();
    _value_type_stack.push(ValueType_Array);
    _count_stack.push(0);
    _count_position_stack.push(_stream.position());
    _stream << static_cast<uint32_t>(0);
}

void BinaryEncoder::end_array()
{
    size_t current_position = _stream.position();
    _stream.seek(_count_position_stack.top());
    _stream << _count_stack.top();
    _stream.seek(current_position);

    _count_position_stack.pop();
    _count_stack.pop();
    _value_type_stack.pop();
}

void BinaryEncoder::begin_object()
{
    increment_count();
    _value_type_stack.push(ValueType_Object);
}

void BinaryEncoder::end_object()
{
    _value_type_stack.pop();
}

void BinaryEncoder::select_member(const char* name)
{
    (void)name;
}

void BinaryEncoder::encode_string(const std::string& value)
{
    increment_count();
    _stream << value;
}

void BinaryEncoder::encode_int8(int8_t value)
{
    increment_count();
    _stream << value;
}

void BinaryEncoder::encode_uint8(uint8_t value)
{
    increment_count();
    _stream << value;
}

void BinaryEncoder::encode_int16(int16_t value)
{
    increment_count();
    _stream << value;
}

void BinaryEncoder::encode_uint16(uint16_t value)
{
    increment_count();
    _stream << value;
}

void BinaryEncoder::encode_int32(int32_t value)
{
    increment_count();
    _stream << value;
}

void BinaryEncoder::encode_uint32(uint32_t value)
{
    increment_count();
    _stream << value;
}

void BinaryEncoder::encode_int64(int64_t value)
{
    increment_count();
    _stream << value;
}

void BinaryEncoder::encode_uint64(uint64_t value)
{
    increment_count();
    _stream << value;
}

void BinaryEncoder::encode_float32(float value)
{
    increment_count();
    _stream << value;
}

void BinaryEncoder::encode_float64(double value)
{
    increment_count();
    _stream << value;
}

void BinaryEncoder::encode_bool(bool value)
{
    increment_count();
    _stream << value;
}

void BinaryEncoder::increment_count()
{
    if (!_value_type_stack.empty() && _value_type_stack.top() == ValueType_Array)
    {
        ++_count_stack.top();
    }
}