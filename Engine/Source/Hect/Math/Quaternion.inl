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
#include <cassert>

namespace hect
{

template <typename T>
QuaternionT<T> QuaternionT<T>::fromAxisAngle(const Vector3T<T>& axis, Angle angle)
{
    // Special case for identity quaternion
    if (angle.degrees() == 0 || axis.lengthSquared() == (T)0)
    {
        return QuaternionT();
    }
    else
    {
        T halfRadians = (T)angle.radians() / (T)2.0;

        Vector3T<T> v = axis.normalized() * (T)std::sin(halfRadians);
        T w = (T)std::cos(halfRadians);

        return QuaternionT(v, w).normalized();
    }
}

template <typename T>
QuaternionT<T>::QuaternionT() :
    x(0),
    y(0),
    z(0),
    w(1)
{
}

template <typename T>
QuaternionT<T>::QuaternionT(T x, T y, T z, T w) :
    x(x),
    y(y),
    z(z),
    w(w)
{
}

template <typename T>
QuaternionT<T>::QuaternionT(const Vector3T<T>& v, T w) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(w)
{
}

template <typename T>
QuaternionT<T>::QuaternionT(const Vector4T<T>& v) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(v.w)
{
}

template <typename T>
void QuaternionT<T>::normalize()
{
    T inv = (T)1.0 / length();
    x *= inv;
    y *= inv;
    z *= inv;
    w *= inv;
}

template <typename T>
QuaternionT<T> QuaternionT<T>::normalized() const
{
    QuaternionT q(*this);
    q.normalize();
    return q;
}

template <typename T>
T QuaternionT<T>::dot(const QuaternionT& q) const
{
    return x * q.x + y * q.y + z * q.z + w * q.w;
}

template <typename T>
T QuaternionT<T>::length() const
{
    return (T)std::sqrt(lengthSquared());
}

template <typename T>
T QuaternionT<T>::lengthSquared() const
{
    return dot(*this);
}

template <typename T>
QuaternionT<T> QuaternionT<T>::conjugate() const
{
    return QuaternionT(-x, -y, -z, w);
}

template <typename T>
QuaternionT<T> QuaternionT<T>::inverse() const
{
    T inv = 1.0 / length();
    return QuaternionT(-(x * inv), -(y * inv), -(z * inv), w * inv);
}

template <typename T>
void QuaternionT<T>::toAxisAngle(Vector3T<T>& axis, Angle& angle) const
{
    // Special case for identity quaternion
    if (w == (T)0 || (x + y + z) == (T)0)
    {
        axis = Vector3T<T>::zero();
        angle = Angle::fromRadians(0);
    }
    else
    {
        QuaternionT q(*this);
        q.normalize();

        axis = Vector3T<T>(q.x, q.y, q.z).normalized();
        angle = Angle::fromRadians((Real)std::acos(w) * 2);
    }
}

template <typename T>
QuaternionT<T> QuaternionT<T>::operator*(T value) const
{
    return QuaternionT(x * value, y * value, z * value, w * value);
}

template <typename T>
Vector3T<T> QuaternionT<T>::operator*(const Vector3T<T>& v) const
{
    return Matrix4T<T>::fromRotation(*this) * v;
}

template <typename T>
QuaternionT<T> QuaternionT<T>::operator*(const QuaternionT& q) const
{
    Vector3T<T> av(x, y, z);
    Vector3T<T> bv(q.x, q.y, q.z);

    Vector3T<T> newV = (bv * w) + (av * q.w) + av.cross(bv);
    T newW = w * q.w - av.dot(bv);

    return QuaternionT<T>(newV, newW);
}

template <typename T>
QuaternionT<T> QuaternionT<T>::operator-() const
{
    return QuaternionT(-x, -y, -z, -w);
}

template <typename T>
QuaternionT<T>& QuaternionT<T>::operator*=(T value)
{
    x *= value;
    y *= value;
    z *= value;
    w *= value;
    return *this;
}

template <typename T>
QuaternionT<T>& QuaternionT<T>::operator*=(const QuaternionT& q)
{
    return *this = *this * q;
}

template <typename T>
T& QuaternionT<T>::operator[](size_t i)
{
    assert(i < 4);
    return ((T*)this)[i];
}

template <typename T>
const T& QuaternionT<T>::operator[](size_t i) const
{
    assert(i < 4);
    return ((const T*)this)[i];
}

template <typename T>
bool QuaternionT<T>::operator==(const QuaternionT& q) const
{
    return x == q.x && y == q.y && z == q.z;
}

template <typename T>
bool QuaternionT<T>::operator!=(const QuaternionT& q) const
{
    return !(*this == q);
}

template <typename T>
QuaternionT<T>::operator Vector4T<T>() const
{
    return Vector4T<T>(x, y, z, w);
}

template <typename T>
Encoder& operator<<(Encoder& encoder, const QuaternionT<T>& q)
{
    Vector3T<T> axis;
    Angle angle;
    q.toAxisAngle(axis, angle);
    return encoder << encodeValue("axis", axis) << encodeValue("angle", angle);
}

template <typename T>
Decoder& operator>>(Decoder& decoder, QuaternionT<T>& q)
{
    Vector3T<T> axis;
    Angle angle;
    decoder >> decodeValue("axis", axis) >> decodeValue("angle", angle);
    q = QuaternionT<T>::fromAxisAngle(axis, angle);
    return decoder;
}

}
