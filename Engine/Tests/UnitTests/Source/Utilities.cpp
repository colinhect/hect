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
#include "Utilities.h"

void test_encode_and_decode(std::function<void(Encoder&)> encode, std::function<void(Decoder&)> decode)
{
    // JSON
    {
        DataValue data_value;
        {
            DataValueEncoder encoder;
            encode(encoder);
            data_value = *encoder.data_values().begin();
        }
        {
            DataValueDecoder decoder(data_value);
            decode(decoder);
        }
    }

    // Binary
    {
        std::vector<uint8_t> data;
        {
            MemoryWriteStream stream(data);
            BinaryEncoder encoder(stream);
            encode(encoder);
        }
        {
            MemoryReadStream stream(data);
            BinaryDecoder decoder(stream);
            decode(decoder);
        }
    }
}