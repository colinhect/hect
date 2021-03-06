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
#include "Font.h"

using namespace hect;

Font::Font()
{
}

Font::Font(Name name) :
    Asset(name)
{
}

bool Font::operator==(const Font& font) const
{
    // Raw data size
    if (_data.size() != font._data.size())
    {
        return false;
    }

    // Raw data
    for (size_t i = 0; i < _data.size(); ++i)
    {
        if (_data[i] != font._data[i])
        {
            return false;
        }
    }

    return true;
}

bool Font::operator!=(const Font& font) const
{
    return !(*this == font);
}

void Font::encode(Encoder& encoder) const
{
    WriteStream& stream = encoder.binary_stream();

    // Write the raw font data to the stream
    stream.write(&_data[0], _data.size());
}

void Font::decode(Decoder& decoder)
{
    ReadStream& stream = decoder.binary_stream();

    // See how long the data is
    size_t length = stream.length();
    _data.resize(length);

    // Read the raw font data from the stream
    stream.read(&_data[0], length);
}
