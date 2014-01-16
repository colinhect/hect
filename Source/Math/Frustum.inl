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
Frustum<T>::Frustum()
{
}

template <typename T>
Frustum<T>::Frustum(
    const Vector3<T>& position,
    const Vector3<T>& front,
    const Vector3<T>& up,
    Angle<T> fieldOfView,
    T aspectRatio,
    T nearClip,
    T farClip) :
    _position(position)
{
    T tangent = (T)std::tan(fieldOfView.radians() * (T)0.5);

    T nearHeight = nearClip * tangent;
    T nearWidth = nearHeight * aspectRatio;

    T farHeight = farClip * tangent;
    T farWidth = farHeight * aspectRatio;

    Vector3<T> z = -front;
    z.normalize();

    Vector3<T> x = up.cross(z);
    x.normalize();

    Vector3<T> y = z.cross(x);

    // Compute center of near/far planes
    Vector3<T> nearCenter = position - z * nearClip;
    Vector3<T> farCenter = position - z * farClip;

    // Compute corners of near plane
    Vector3<T> nearTopLeft = nearCenter + y * nearHeight - x * nearWidth;
    Vector3<T> nearTopRight = nearCenter + y * nearHeight + x * nearWidth;
    Vector3<T> nearBottomLeft = nearCenter - y * nearHeight - x * nearWidth;
    Vector3<T> nearBottomRight = nearCenter - y * nearHeight + x * nearWidth;

    // Compute corners of far plane
    Vector3<T> farTopLeft = farCenter + y * farHeight - x * farWidth;
    Vector3<T> farTopRight = farCenter + y * farHeight + x * farWidth;
    Vector3<T> farBottomLeft = farCenter - y * farHeight - x * farWidth;
    Vector3<T> farBottomRight = farCenter - y * farHeight + x * farWidth;

    // Build planes from points
    _planes[0] = Plane<T>::fromPoints(nearTopRight, nearTopLeft, farTopLeft); // Top
    _planes[1] = Plane<T>::fromPoints(nearBottomLeft, nearBottomRight, farBottomRight); // Bottom
    _planes[2] = Plane<T>::fromPoints(nearTopLeft, nearBottomLeft, farBottomLeft); // Left
    _planes[3] = Plane<T>::fromPoints(nearBottomRight, nearTopRight, farBottomRight); // Right
    _planes[4] = Plane<T>::fromPoints(nearTopLeft, nearTopRight, nearBottomRight); // Near
    _planes[5] = Plane<T>::fromPoints(farTopRight, farTopLeft, farBottomLeft); // Far
}

template <typename T>
FrustumTestResult Frustum<T>::testAxisAlignedBox(const AxisAlignedBox<T>& box) const
{
    FrustumTestResult result = FrustumTestResult::Inside;

    if (!box.hasSize())
    {
        return result;
    }

    for (int i = 0; i < 6; i++)
    {
        Vector3<T> normal(_planes[i].normal());

        Vector3<T> minimum(box.minimum());
        Vector3<T> maximum(box.maximum());

        Vector3<T> p(minimum);
        Vector3<T> n(maximum);

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

        T distanceP = _planes[i].distance() + normal.dot(p);
        T distanceN = _planes[i].distance() + normal.dot(n);

        if (distanceP < 0)
        {
            return FrustumTestResult::Outside;
        }
        else if (distanceN < 0)
        {
            result = FrustumTestResult::Intersect;
        }
    }

    return result;
}

template <typename T>
bool Frustum<T>::containsSphere(const Sphere<T>& sphere, const Vector3<T>& position) const
{
    T radius = sphere.radius();

    for (int i = 0; i < 6; ++i)
    {
        T distance = _planes[i].distance() + _planes[i].normal().dot(position);

        if (distance < -radius)
        {
            return false;
        }
    }

    return true;
}

}