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
#include "Hect/Math/Vector2.h"

namespace hect
{

///
/// A 2-dimensional box in screen-space.
class HECT_EXPORT Rectangle :
    public Encodable
{
public:

    ///
    /// Constructs a default rectangle
    Rectangle();

    ///
    /// Constructs a rectangle.
    ///
    /// \param minX The x coordinate of the minimum point.
    /// \param minY The y coordinate of the minimum point.
    /// \param maxX The x coordinate of the maximum point.
    /// \param maxY The y coordinate of the maximum point.
    Rectangle(double minX, double minY, double maxX, double maxY);

    ///
    /// Constructs a rectangle.
    ///
    /// \param minimum The minimum point.
    /// \param maximum The maximum point.
    Rectangle(Vector2 minimum, Vector2 maximum);

    ///
    /// Returns the minimum point.
    Vector2 minimum() const;

    ///
    /// Returns the maximum point.
    Vector2 maximum() const;

    ///
    /// Returns the center point.
    Vector2 center() const;

    ///
    /// Returns the width and height.
    Vector2 size() const;

    ///
    /// Returns whether the rectangle contains a point.
    ///
    /// \param point The point.
    ///
    /// \returns True if the rectangle contains the point; false otherwise.
    bool contains(Vector2 point) const;

    ///
    /// Returns the intersection between the rectangle and another.
    ///
    /// \param rectangle The other rectangle.
    Rectangle intersect(Rectangle rectangle) const;

    ///
    /// Returns whether the rectangle is equivalent to another.
    ///
    /// \param rectangle The other rectangle.
    bool operator==(Rectangle rectangle) const;

    ///
    /// Returns whether the rectangle is different from another.
    ///
    /// \param rectangle The other rectangle.
    bool operator!=(Rectangle rectangle) const;

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    Vector2 _minimum;
    Vector2 _maximum;
};

}