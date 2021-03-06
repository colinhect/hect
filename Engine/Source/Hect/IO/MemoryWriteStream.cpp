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
#include "MemoryWriteStream.h"

#include <cassert>
#include <cstring>

#include "Hect/Core/Exception.h"

using namespace hect;

MemoryWriteStream::MemoryWriteStream(ByteVector& data) :
    _data(data),
    _position(data.size())
{
}

void MemoryWriteStream::write(const uint8_t* bytes, size_t byte_count)
{
    assert(bytes);

    while (_position + byte_count > _data.size())
    {
        _data.push_back(0);
    }

    std::memcpy(&_data[_position], bytes, byte_count);
    _position += byte_count;
}

size_t MemoryWriteStream::position() const
{
    return _position;
}

void MemoryWriteStream::seek(size_t position)
{
    size_t length = _data.size();

    if (position >= length + 1)
    {
        throw InvalidOperation("Attempt to seek past end of data");
    }

    _position = position;
}
