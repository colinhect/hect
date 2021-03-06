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
#pragma once

#include <vector>
#include <cstdint>

#include "Hect/Core/Export.h"
#include "Hect/IO/ByteVector.h"
#include "Hect/IO/ReadStream.h"

namespace hect
{

///
/// Provides read access to raw data in memory.
class HECT_EXPORT MemoryReadStream :
    public ReadStream
{
public:

    ///
    /// Constructs a stream.
    ///
    /// \param data The data to read from.
    MemoryReadStream(const ByteVector& data);

    void read(uint8_t* bytes, size_t byte_count) override;
    bool end_of_stream() const override;
    size_t length() const override;
    size_t position() const override;
    void seek(size_t position) override;

private:
    const ByteVector& _data;
    size_t _position { 0 };
};

}