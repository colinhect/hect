///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
#include "PixelFormat.h"

using namespace hect;

PixelFormat::PixelFormat()
{
}

PixelFormat::PixelFormat(PixelType type, unsigned cardinality) :
    _type(type),
    _cardinality(cardinality)
{
    if (cardinality == 0 || cardinality > 4)
    {
        throw InvalidOperation("A pixel format cannot exceed a cardinality of 4");
    }
}

PixelType PixelFormat::type() const
{
    return _type;
}

unsigned PixelFormat::cardinality() const
{
    return _cardinality;
}

unsigned PixelFormat::size() const
{
    switch (_type)
    {
    case PixelType::Byte:
        return 1 * _cardinality;
    case PixelType::Float16:
        return 2 * _cardinality;
    case PixelType::Float32:
        return 4 * _cardinality;
    default:
        break;
    }

    return 0;
}

bool PixelFormat::operator==(const PixelFormat& pixelFormat) const
{
    return _type == pixelFormat._type && _cardinality == pixelFormat._cardinality;
}

bool PixelFormat::operator!=(const PixelFormat& pixelFormat) const
{
    return _type != pixelFormat._type || _cardinality != pixelFormat._cardinality;
}

void PixelFormat::encode(Encoder& encoder) const
{
    encoder << encodeEnum("type", _type)
            << encodeValue("cardinality", _cardinality);
}

void PixelFormat::decode(Decoder& decoder)
{
    decoder >> decodeEnum("type", _type)
            >> decodeValue("cardinality", _cardinality);
}
