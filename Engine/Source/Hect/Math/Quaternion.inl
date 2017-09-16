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
#include <cassert>

namespace hect
{

template <typename T>
const QuaternionT<T> QuaternionT<T>::Identity = QuaternionT<T>();

template <typename T>
QuaternionT<T> QuaternionT<T>::from_axis_angle(Vector3T<T> axis, Radians angle)
{
    // Special case for identity quaternion
    if (angle.value == 0 || axis.length_squared() == T(0))
    {
        return QuaternionT();
    }
    else
    {
        T half_radians = static_cast<T>(angle.value / T(2));

        Vector3T<T> v = axis.normalized() * static_cast<T>(std::sin(half_radians));
        T w = static_cast<T>(std::cos(half_radians));

        return QuaternionT(v, w).normalized();
    }
}

template <typename T>
QuaternionT<T>::QuaternionT()
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
QuaternionT<T>::QuaternionT(Vector3T<T> v, T w) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(w)
{
}

template <typename T>
QuaternionT<T>::QuaternionT(Vector4T<T> v) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(v.w)
{
}

template <typename T>
void QuaternionT<T>::normalize()
{
    T inv = T(1) / length();
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
    return (T)std::sqrt(length_squared());
}

template <typename T>
T QuaternionT<T>::length_squared() const
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
    T inv = T(1) / length();
    return QuaternionT(-(x * inv), -(y * inv), -(z * inv), w * inv);
}

template <typename T>
void QuaternionT<T>::to_axis_angle(Vector3T<T>& axis, Radians& angle) const
{
    // Special case for identity quaternion
    if (w == T(0) || (x + y + z) == T(0))
    {
        axis = Vector3T<T>::Zero;
        angle = Radians(0);
    }
    else
    {
        QuaternionT q(*this);
        q.normalize();

        axis = Vector3T<T>(q.x, q.y, q.z).normalized();
        angle = Radians(static_cast<double>(std::acos(w)) * 2);
    }
}

template <typename T>
QuaternionT<T> QuaternionT<T>::operator*(T value) const
{
    return QuaternionT(x * value, y * value, z * value, w * value);
}

template <typename T>
Vector3T<T> QuaternionT<T>::operator*(Vector3T<T> v) const
{
    return Matrix4T<T>::from_rotation(*this) * v;
}

template <typename T>
QuaternionT<T> QuaternionT<T>::operator*(const QuaternionT& q) const
{
    Vector3T<T> av(x, y, z);
    Vector3T<T> bv(q.x, q.y, q.z);

    Vector3T<T> new_v = (bv * w) + (av * q.w) + av.cross(bv);
    T new_w = w * q.w - av.dot(bv);

    return QuaternionT<T>(new_v, new_w);
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
    return reinterpret_cast<T*>(this)[i];
}

template <typename T>
const T& QuaternionT<T>::operator[](size_t i) const
{
    assert(i < 4);
    return reinterpret_cast<const T*>(this)[i];
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
template <typename U>
QuaternionT<T>::operator QuaternionT<U>() const
{
    return QuaternionT<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z), static_cast<U>(w));
}

template <typename T>
QuaternionT<T>::operator Vector4T<T>() const
{
    return Vector4T<T>(x, y, z, w);
}

template <typename T>
Encoder& operator<<(Encoder& encoder, QuaternionT<T> q)
{
    Vector3T<T> axis;
    Radians angle;
    q.to_axis_angle(axis, angle);
    encoder << begin_object()
            << encode_value("axis", axis)
            << encode_value("angle", angle)
            << end_object();
    return encoder;
}

template <typename T>
Decoder& operator>>(Decoder& decoder, QuaternionT<T>& q)
{
    Vector3T<T> axis;
    Radians angle;
    decoder >> begin_object()
            >> decode_value("axis", axis, true)
            >> decode_value("angle", angle, true)
            >> end_object();
    q = QuaternionT<T>::from_axis_angle(axis, angle);
    return decoder;
}

}
