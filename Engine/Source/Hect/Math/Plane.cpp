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
#include "Plane.h"

namespace hect
{

Plane Plane::fromPoints(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
    Vector3 position = p1;
    Vector3 normal = (p2 - p1).cross(p0 - p1).normalized();
    double distance = -normal.dot(position);

    return Plane(position, normal, distance);
}

Plane::Plane() :
    _distance(0)
{
}

Plane::Plane(const Vector3& position, const Vector3& normal, double distance) :
    _position(position),
    _normal(normal),
    _distance(distance)
{
}

const Vector3& Plane::position() const
{
    return _position;
}

const Vector3& Plane::normal() const
{
    return _normal;
}

double Plane::distance() const
{
    return _distance;
}

}