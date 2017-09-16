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
#include "Rectangle.h"

using namespace hect;

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(double min_x, double min_y, double max_x, double max_y) :
    _minimum(min_x, min_y),
    _maximum(max_x, max_y)
{
}

Rectangle::Rectangle(Vector2 minimum, Vector2 maximum) :
    _minimum(minimum),
    _maximum(maximum)
{
}

Vector2 Rectangle::minimum() const
{
    return _minimum;
}

Vector2 Rectangle::maximum() const
{
    return _maximum;
}

Vector2 Rectangle::center() const
{
    return _minimum + size() * 0.5;
}

Vector2 Rectangle::size() const
{
    return _maximum - _minimum;
}

bool Rectangle::contains(Vector2 point) const
{
    return point.x >= _minimum.x && point.x <= _maximum.x
           && point.y >= _minimum.y && point.y <= _maximum.y;
}

Rectangle Rectangle::intersect(Rectangle rectangle) const
{
    Vector2 minimum(std::max(_minimum.x, rectangle._minimum.x), std::max(_minimum.y, rectangle._minimum.y));
    Vector2 maximum(std::min(_maximum.x, rectangle._maximum.x), std::min(_maximum.y, rectangle._maximum.y));

    return Rectangle(minimum, maximum);
}

bool Rectangle::operator==(Rectangle rectangle) const
{
    return _minimum == rectangle._minimum && _maximum == rectangle._maximum;
}

bool Rectangle::operator!=(Rectangle rectangle) const
{
    return _minimum != rectangle._minimum || _maximum != rectangle._maximum;
}

void Rectangle::encode(Encoder& encoder) const
{
    encoder << encode_value("minimum", _minimum)
            << encode_value("maximum", _maximum);
}

void Rectangle::decode(Decoder& decoder)
{
    decoder >> decode_value("minimum", _minimum, true)
            >> decode_value("maximum", _maximum, true);
}
