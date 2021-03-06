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

#include "Hect/Core/Export.h"
#include "Hect/IO/Encodable.h"
#include "Hect/Math/Quaternion.h"
#include "Hect/Math/Vector3.h"

namespace hect
{

///
/// A box aligned with the global axes in world-space.
class HECT_EXPORT AxisAlignedBox :
    public Encodable
{
public:

    ///
    /// Constructs an axis aligned box at the origin without a size.
    AxisAlignedBox();

    ///
    /// Constructs an axis aligned box.
    ///
    /// \param minimum The minimum point.
    /// \param maximum The maximum point.
    AxisAlignedBox(Vector3 minimum, Vector3 maximum);

    ///
    /// Expands the bounds of the box to include point.
    ///
    /// \param point The point to include.
    void expand_to_include(Vector3 point);

    ///
    /// Expands the bounds of the box to include another box.
    ///
    /// \param box The box to include.
    void expand_to_include(const AxisAlignedBox& box);

    ///
    /// Applies a translation to the box.
    ///
    /// \param translation The translation to apply.
    void translate(Vector3 translation);

    ///
    /// Applies a scale to the box.
    ///
    /// \param scale The scale to apply.
    void scale(Vector3 scale);

    ///
    /// Applies a rotation to the box.
    ///
    /// \param rotation The rotation to apply.
    void rotate(Quaternion rotation);

    ///
    /// Returns the minimum point.
    Vector3 minimum() const;

    ///
    /// Returns the maximum point.
    Vector3 maximum() const;

    ///
    /// Returns the width, height, and depth.
    Vector3 size() const;

    ///
    /// Returns the center point.
    Vector3 center() const;

    ///
    /// Returns whether the box has a non-negligible size.
    bool has_size() const;

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    void merge_minimum(Vector3 point);
    void merge_maximum(Vector3 point);

    enum Flags
    {
        MinX = 1,
        MinY = 2,
        MinZ = 4,
        MaxX = 8,
        MaxY = 16,
        MaxZ = 32
    };

    int _flags { 0 };

    Vector3 _minimum;
    Vector3 _maximum;
};

}