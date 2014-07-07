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

#include "Hect/Math/Vector3.h"

namespace hect
{

///
/// A box aligned with the global axes.
class AxisAlignedBox
{
public:

    ///
    /// Constructs an axis aligned box at the origin without a size.
    AxisAlignedBox();

    ///
    /// Constructs an axis aligned box given a minimum and maximum point.
    ///
    /// \param minimum The minimum point.
    /// \param maximum The maximum point.
    AxisAlignedBox(const Vector3& minimum, const Vector3& maximum);

    ///
    /// Expands the bounds of the box to include point.
    ///
    /// \param point The point to include.
    void expandToInclude(const Vector3& point);

    ///
    /// Expands the bounds of the box to include another box.
    ///
    /// \param box The box to include.
    void expandToInclude(const AxisAlignedBox& box);

    ///
    /// Translates the box's center.
    ///
    /// \param translation The translation to perform.
    void translate(const Vector3& translation);

    ///
    /// Returns the minimum point.
    const Vector3& minimum() const;

    ///
    /// Returns the maximum point.
    const Vector3& maximum() const;

    ///
    /// Returns whether the box has a non-negligible size.
    bool hasSize() const;

private:
    void _mergeMinimum(const Vector3& point);
    void _mergeMaximum(const Vector3& point);

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

    Vector3 _minimum;
    Vector3 _maximum;
};

}