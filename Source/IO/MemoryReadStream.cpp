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

MemoryReadStream::MemoryReadStream(const std::vector<uint8_t>& data) :
    _data(&data),
    _position(0)
{
}

void MemoryReadStream::readBytes(uint8_t* bytes, size_t byteCount)
{
    assert(bytes);

    size_t length = this->length();
    size_t position = this->position();

    if (position + byteCount >= length + 1)
    {
        throw Error("Attempt to read past end of data");
    }

    std::memcpy(bytes, &(*_data)[position], byteCount);
    _position += byteCount;
}

bool MemoryReadStream::endOfStream() const
{
    return _position >= length();
}

size_t MemoryReadStream::length() const
{
    return _data->size();
}

size_t MemoryReadStream::position() const
{
    return _position;
}

void MemoryReadStream::seek(size_t position)
{
    size_t length = this->length();

    if (position >= length + 1)
    {
        throw Error("Attempt to seek past end of data");
    }

    _position = position;
}