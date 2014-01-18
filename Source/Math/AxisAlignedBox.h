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
#pragma once

namespace hect
{

///
/// A box aligned with the global axes.
template <typename T>
class AxisAlignedBoxT
{
public:

    ///
    /// Constructs an axis aligned box at the origin without a size.
    AxisAlignedBoxT();

    ///
    /// Constructs an axis aligned box given a minimum and maximum point.
    ///
    /// \param minimum The minimum point.
    /// \param maximum The maximum point.
    AxisAlignedBoxT(const Vector3T<T>& minimum, const Vector3T<T>& maximum);

    ///
    /// Expands the bounds of the box to include point.
    ///
    /// \param point The point to include.
    void expandToInclude(const Vector3T<T>& point);

    ///
    /// Expands the bounds of the box to include another box.
    ///
    /// \param box The box to include.
    void expandToInclude(const AxisAlignedBoxT<T>& box);

    ///
    /// Returns the minimum point.
    const Vector3T<T>& minimum() const;

    ///
    /// Returns the maximum point.
    const Vector3T<T>& maximum() const;

    ///
    /// Returns whether the box has a non-negligible size.
    bool hasSize() const;

    ///
    /// Converts to an equivalent vector with a different underlying type.
    template <typename U>
    operator AxisAlignedBoxT<U>() const
    {
        return AxisAlignedBoxT<U>(_minimum, _maximum);
    }

private:
    void _mergeMinimum(const Vector3T<T>& point);
    void _mergeMaximum(const Vector3T<T>& point);

    enum Flags
    {
        MinX = 1,
        MinY = 2,
        MinZ = 4,
        MaxX = 8,
        MaxY = 16,
        MaxZ = 32
    };

    int _flags;

    Vector3T<T> _minimum;
    Vector3T<T> _maximum;
};

typedef AxisAlignedBoxT<Real> AxisAlignedBox;

}

#include "AxisAlignedBox.inl"