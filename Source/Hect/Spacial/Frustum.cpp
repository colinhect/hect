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
#include "Frustum.h"

namespace hect
{

Frustum::Frustum()
{
}

Frustum::Frustum(
    const Vector3& position,
    const Vector3& front,
    const Vector3& up,
    Angle fieldOfView,
    Real aspectRatio,
    Real nearClip,
    Real farClip) :
    _position(position)
{
    Real tangent = (Real)std::tan(fieldOfView.radians() * (Real)0.5);

    Real nearHeight = nearClip * tangent;
    Real nearWidth = nearHeight * aspectRatio;

    Real farHeight = farClip * tangent;
    Real farWidth = farHeight * aspectRatio;

    Vector3 z = -front;
    z.normalize();

    Vector3 x = up.cross(z);
    x.normalize();

    Vector3 y = z.cross(x);

    // Compute center of near/far planes
    Vector3 nearCenter = position - z * nearClip;
    Vector3 farCenter = position - z * farClip;

    // Compute corners of near plane
    Vector3 nearTopLeft = nearCenter + y * nearHeight - x * nearWidth;
    Vector3 nearTopRight = nearCenter + y * nearHeight + x * nearWidth;
    Vector3 nearBottomLeft = nearCenter - y * nearHeight - x * nearWidth;
    Vector3 nearBottomRight = nearCenter - y * nearHeight + x * nearWidth;

    // Compute corners of far plane
    Vector3 farTopLeft = farCenter + y * farHeight - x * farWidth;
    Vector3 farTopRight = farCenter + y * farHeight + x * farWidth;
    Vector3 farBottomLeft = farCenter - y * farHeight - x * farWidth;
    Vector3 farBottomRight = farCenter - y * farHeight + x * farWidth;

    // Build planes from points
    _planes[0] = Plane::fromPoints(nearTopRight, nearTopLeft, farTopLeft); // Top
    _planes[1] = Plane::fromPoints(nearBottomLeft, nearBottomRight, farBottomRight); // Bottom
    _planes[2] = Plane::fromPoints(nearTopLeft, nearBottomLeft, farBottomLeft); // Left
    _planes[3] = Plane::fromPoints(nearBottomRight, nearTopRight, farBottomRight); // Right
    _planes[4] = Plane::fromPoints(nearTopLeft, nearTopRight, nearBottomRight); // Near
    _planes[5] = Plane::fromPoints(farTopRight, farTopLeft, farBottomLeft); // Far
}

FrustumTestResult Frustum::testAxisAlignedBox(const AxisAlignedBox& box) const
{
    FrustumTestResult result = FrustumTestResult_Inside;

    if (!box.hasSize())
    {
        return result;
    }

    for (int i = 0; i < 6; i++)
    {
        Vector3 normal(_planes[i].normal());

        Vector3 minimum(box.minimum());
        Vector3 maximum(box.maximum());

        Vector3 p(minimum);
        Vector3 n(maximum);

        if (normal.x >= 0)
        {
            p.x = maximum.x;
            n.x = minimum.x;
        }

        if (normal.y >= 0)
        {
            p.y = maximum.y;
            n.y = minimum.y;
        }

        if (normal.z >= 0)
        {
            p.z = maximum.z;
            n.z = minimum.z;
        }

        Real distanceP = _planes[i].distance() + normal.dot(p);
        Real distanceN = _planes[i].distance() + normal.dot(n);

        if (distanceP < 0)
        {
            return FrustumTestResult_Outside;
        }
        else if (distanceN < 0)
        {
            result = FrustumTestResult_Intersect;
        }
    }

    return result;
}

bool Frustum::containsSphere(const Sphere& sphere, const Vector3& position) const
{
    Real radius = sphere.radius();

    for (int i = 0; i < 6; ++i)
    {
        Real distance = _planes[i].distance() + _planes[i].normal().dot(position);

        if (distance < -radius)
        {
            return false;
        }
    }

    return true;
}

}