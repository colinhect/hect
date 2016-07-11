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
#include "Hect/Math/Vector3.h"

namespace hect
{

///
/// A plane in 3-dimensional space.
class HECT_EXPORT Plane
{
public:

    ///
    /// Constructs a plane from 3 points.
    ///
    /// \param p0 The first point.
    /// \param p1 The second point.
    /// \param p2 The third point.
    static Plane fromPoints(Vector3 p0, Vector3 p1, Vector3 p2);

    ///
    /// Constructs a default plane.
    Plane();

    ///
    /// Constructs a plane given a position, normal, and distance.
    ///
    /// \param position The origin of the plane.
    /// \param normal The direction from the origin the plane faces.
    /// \param distance The distance from the origin along the normal.
    Plane(Vector3 position, Vector3 normal, double distance);

    ///
    /// Returns the position.
    Vector3 position() const;

    ///
    /// Returns the normal.
    Vector3 normal() const;

    ///
    /// Returns the distance.
    double distance() const;

private:
    Vector3 _position;
    Vector3 _normal;
    double _distance { 0 };
};

}