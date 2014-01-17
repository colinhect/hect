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
/// A plane in 3-dimensional space.
template <typename T = Real>
class PlaneT
{
public:

    ///
    /// Constructs a plane from 3 points.
    ///
    /// \param p0 The first point.
    /// \param p1 The second point.
    /// \param p2 The third point.
    static PlaneT fromPoints(const Vector3T<T>& p0, const Vector3T<T>& p1, const Vector3T<T>& p2);

    ///
    /// Constructs a default plane.
    PlaneT();

    ///
    /// Constructs a plane given a position, normal, and distance.
    ///
    /// \param position The origin of the plane.
    /// \param normal The direction from the origin the plane faces.
    /// \param distance The distance from the origin along the normal.
    PlaneT(const Vector3T<T>& position, const Vector3T<T>& normal, T distance);

    ///
    /// Returns the position.
    const Vector3T<T>& position() const;

    ///
    /// Returns the normal.
    const Vector3T<T>& normal() const;

    ///
    /// Returns the distance.
    T distance() const;

private:
    Vector3T<T> _position;
    Vector3T<T> _normal;
    T _distance;
};

typedef PlaneT<> Plane;

}

#include "Plane.inl"