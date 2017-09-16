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
#include "AxisAlignedBox.h"

using namespace hect;

AxisAlignedBox::AxisAlignedBox() :
    _flags(0)
{
}

AxisAlignedBox::AxisAlignedBox(Vector3 minimum, Vector3 maximum) :
    _flags(MinX | MinY | MinZ | MaxX | MaxY | MaxZ),
    _minimum(minimum),
    _maximum(maximum)
{
}

void AxisAlignedBox::expand_to_include(Vector3 point)
{
    merge_minimum(point);
    merge_maximum(point);
}

void AxisAlignedBox::expand_to_include(const AxisAlignedBox& box)
{
    if (!box.has_size())
    {
        return;
    }
    else if (!has_size())
    {
        *this = box;
        return;
    }

    merge_minimum(box._minimum);
    merge_maximum(box._maximum);
}

void AxisAlignedBox::translate(Vector3 translation)
{
    _minimum += translation;
    _maximum += translation;
}

void AxisAlignedBox::scale(Vector3 scale)
{
    _minimum *= scale;
    _maximum *= scale;
}

void AxisAlignedBox::rotate(Quaternion rotation)
{
    Vector3 points[8] =
    {
        _minimum,
        Vector3(_minimum.x, _minimum.y, _maximum.z),
        Vector3(_minimum.x, _maximum.y, _minimum.z),
        Vector3(_minimum.x, _maximum.y, _maximum.z),
        Vector3(_maximum.x, _minimum.y, _minimum.z),
        Vector3(_maximum.x, _minimum.y, _maximum.z),
        Vector3(_maximum.x, _maximum.y, _minimum.z),
        _maximum
    };

    for (size_t i = 0; i < 8; ++i)
    {
        points[i] = rotation * points[i];
    }

    *this = AxisAlignedBox();

    for (size_t i = 0; i < 8; ++i)
    {
        expand_to_include(points[i]);
    }
}

Vector3 AxisAlignedBox::minimum() const
{
    return _minimum;
}

Vector3 AxisAlignedBox::maximum() const
{
    return _maximum;
}

Vector3 AxisAlignedBox::size() const
{
    return _maximum - _minimum;
}

Vector3 AxisAlignedBox::center() const
{
    return _minimum + size() * 0.5;
}

bool AxisAlignedBox::has_size() const
{
    return _flags != 0;
}

void AxisAlignedBox::merge_minimum(Vector3 point)
{
    if (!(_flags & MinX) || point.x < _minimum.x)
    {
        _minimum.x = point.x;
        _flags |= MinX;
    }

    if (!(_flags & MinY) || point.y < _minimum.y)
    {
        _minimum.y = point.y;
        _flags |= MinY;
    }

    if (!(_flags & MinZ) || point.z < _minimum.z)
    {
        _minimum.z = point.z;
        _flags |= MinZ;
    }
}

void AxisAlignedBox::merge_maximum(Vector3 point)
{
    if (!(_flags & MaxX) || point.x > _maximum.x)
    {
        _maximum.x = point.x;
        _flags |= MaxX;
    }

    if (!(_flags & MaxY) || point.y > _maximum.y)
    {
        _maximum.y = point.y;
        _flags |= MaxY;
    }

    if (!(_flags & MaxZ) || point.z > _maximum.z)
    {
        _maximum.z = point.z;
        _flags |= MaxZ;
    }
}

void AxisAlignedBox::encode(Encoder& encoder) const
{
    encoder << encode_value("minimum", _minimum)
            << encode_value("maximum", _maximum);
}

void AxisAlignedBox::decode(Decoder& decoder)
{
    decoder >> decode_value("minimum", _minimum, true)
            >> decode_value("maximum", _maximum, true);
}