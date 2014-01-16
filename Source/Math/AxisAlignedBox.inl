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
namespace hect
{

template <typename T>
AxisAlignedBox<T>::AxisAlignedBox() :
    _flags(0)
{
}

template <typename T>
AxisAlignedBox<T>::AxisAlignedBox(const Vector3<T>& minimum, const Vector3<T>& maximum) :
    _flags(MinX | MinY | MinZ | MaxX | MaxY | MaxZ),
    _minimum(minimum),
    _maximum(maximum)
{
}

template <typename T>
void AxisAlignedBox<T>::expandToInclude(const Vector3<T>& point)
{
    _mergeMinimum(point);
    _mergeMaximum(point);
}

template <typename T>
void AxisAlignedBox<T>::expandToInclude(const AxisAlignedBox<T>& box)
{
    if (!box.hasSize())
    {
        return;
    }
    else if (!hasSize())
    {
        *this = box;
        return;
    }

    _mergeMinimum(box._minimum);
    _mergeMaximum(box._maximum);
}

template <typename T>
const Vector3<T>& AxisAlignedBox<T>::minimum() const
{
    return _minimum;
}

template <typename T>
const Vector3<T>& AxisAlignedBox<T>::maximum() const
{
    return _maximum;
}

template <typename T>
bool AxisAlignedBox<T>::hasSize() const
{
    return _flags != 0;
}

template <typename T>
void AxisAlignedBox<T>::_mergeMinimum(const Vector3<T>& point)
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

template <typename T>
void AxisAlignedBox<T>::_mergeMaximum(const Vector3<T>& point)
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

}