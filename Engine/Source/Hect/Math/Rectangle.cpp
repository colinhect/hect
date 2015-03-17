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
#include "Rectangle.h"

using namespace hect;

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(double minX, double minY, double maxX, double maxY) :
    _minimum(minX, minY),
    _maximum(maxX, maxY)
{
}

Rectangle::Rectangle(const Vector2& minimum, const Vector2& maximum) :
    _minimum(minimum),
    _maximum(maximum)
{
}

const Vector2& Rectangle::minimum() const
{
    return _minimum;
}

const Vector2& Rectangle::maximum() const
{
    return _maximum;
}

Vector2 Rectangle::size() const
{
    return _maximum - _minimum;
}

Vector2 Rectangle::center() const
{
    return _minimum + size() * 0.5;
}

void Rectangle::encode(Encoder& encoder) const
{
    encoder << encodeValue("minimum", _minimum)
            << encodeValue("maximum", _maximum);
}

void Rectangle::decode(Decoder& decoder)
{
    decoder >> decodeValue("minimum", _minimum, true)
            >> decodeValue("maximum", _maximum, true);
}
