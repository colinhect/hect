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
FrustumT<T>::FrustumT()
{
}

template <typename T>
FrustumT<T>::FrustumT(
    const Vector3T<T>& position,
    const Vector3T<T>& front,
    const Vector3T<T>& up,
    Angle fieldOfView,
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

    Vector3T<T> z = -front;
    z.normalize();

    Vector3T<T> x = up.cross(z);
    x.normalize();

    Vector3T<T> y = z.cross(x);

    // Compute center of near/far planes
    Vector3T<T> nearCenter = position - z * nearClip;
    Vector3T<T> farCenter = position - z * farClip;

    // Compute corners of near plane
    Vector3T<T> nearTopLeft = nearCenter + y * nearHeight - x * nearWidth;
    Vector3T<T> nearTopRight = nearCenter + y * nearHeight + x * nearWidth;
    Vector3T<T> nearBottomLeft = nearCenter - y * nearHeight - x * nearWidth;
    Vector3T<T> nearBottomRight = nearCenter - y * nearHeight + x * nearWidth;

    // Compute corners of far plane
    Vector3T<T> farTopLeft = farCenter + y * farHeight - x * farWidth;
    Vector3T<T> farTopRight = farCenter + y * farHeight + x * farWidth;
    Vector3T<T> farBottomLeft = farCenter - y * farHeight - x * farWidth;
    Vector3T<T> farBottomRight = farCenter - y * farHeight + x * farWidth;

    // Build planes from points
    _planes[0] = PlaneT<T>::fromPoints(nearTopRight, nearTopLeft, farTopLeft); // Top
    _planes[1] = PlaneT<T>::fromPoints(nearBottomLeft, nearBottomRight, farBottomRight); // Bottom
    _planes[2] = PlaneT<T>::fromPoints(nearTopLeft, nearBottomLeft, farBottomLeft); // Left
    _planes[3] = PlaneT<T>::fromPoints(nearBottomRight, nearTopRight, farBottomRight); // Right
    _planes[4] = PlaneT<T>::fromPoints(nearTopLeft, nearTopRight, nearBottomRight); // Near
    _planes[5] = PlaneT<T>::fromPoints(farTopRight, farTopLeft, farBottomLeft); // Far
}

template <typename T>
FrustumTestResult FrustumT<T>::testAxisAlignedBox(const AxisAlignedBox& box) const
{
    FrustumTestResult result = FrustumTestResult::Inside;

    if (!box.hasSize())
    {
        return result;
    }

    for (int i = 0; i < 6; i++)
    {
        Vector3T<T> normal(_planes[i].normal());

        Vector3T<T> minimum(box.minimum());
        Vector3T<T> maximum(box.maximum());

        Vector3T<T> p(minimum);
        Vector3T<T> n(maximum);

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
bool FrustumT<T>::containsSphere(const SphereT<T>& sphere, const Vector3T<T>& position) const
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