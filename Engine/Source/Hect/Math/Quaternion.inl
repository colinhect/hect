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

template <typename Type>
const QuaternionT<Type> QuaternionT<Type>::Identity = QuaternionT<Type>();

template <typename Type>
QuaternionT<Type> QuaternionT<Type>::from_axis_angle(Vector3T<Type> axis, Radians angle)
{
    // Special case for identity quaternion
    if (angle.value == 0 || axis.length_squared() == Type(0))
    {
        return QuaternionT();
    }
    else
    {
        Type half_radians = static_cast<Type>(angle.value / Type(2));

        Vector3T<Type> v = axis.normalized() * static_cast<Type>(std::sin(half_radians));
        Type w = static_cast<Type>(std::cos(half_radians));

        return QuaternionT(v, w).normalized();
    }
}

template <typename Type>
QuaternionT<Type>::QuaternionT()
{
}

template <typename Type>
QuaternionT<Type>::QuaternionT(Type x, Type y, Type z, Type w) :
    x(x),
    y(y),
    z(z),
    w(w)
{
}

template <typename Type>
QuaternionT<Type>::QuaternionT(Vector3T<Type> v, Type w) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(w)
{
}

template <typename Type>
QuaternionT<Type>::QuaternionT(Vector4T<Type> v) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(v.w)
{
}

template <typename Type>
void QuaternionT<Type>::normalize()
{
    Type inv = Type(1) / length();
    x *= inv;
    y *= inv;
    z *= inv;
    w *= inv;
}

template <typename Type>
QuaternionT<Type> QuaternionT<Type>::normalized() const
{
    QuaternionT q(*this);
    q.normalize();
    return q;
}

template <typename Type>
Type QuaternionT<Type>::dot(const QuaternionT& q) const
{
    return x * q.x + y * q.y + z * q.z + w * q.w;
}

template <typename Type>
Type QuaternionT<Type>::length() const
{
    return (Type)std::sqrt(length_squared());
}

template <typename Type>
Type QuaternionT<Type>::length_squared() const
{
    return dot(*this);
}

template <typename Type>
QuaternionT<Type> QuaternionT<Type>::conjugate() const
{
    return QuaternionT(-x, -y, -z, w);
}

template <typename Type>
QuaternionT<Type> QuaternionT<Type>::inverse() const
{
    Type inv = Type(1) / length();
    return QuaternionT(-(x * inv), -(y * inv), -(z * inv), w * inv);
}

template <typename Type>
void QuaternionT<Type>::to_axis_angle(Vector3T<Type>& axis, Radians& angle) const
{
    // Special case for identity quaternion
    if (w == Type(0) || (x + y + z) == Type(0))
    {
        axis = Vector3T<Type>::Zero;
        angle = Radians(0);
    }
    else
    {
        QuaternionT q(*this);
        q.normalize();

        axis = Vector3T<Type>(q.x, q.y, q.z).normalized();
        angle = Radians(static_cast<double>(std::acos(w)) * 2);
    }
}

template <typename Type>
QuaternionT<Type> QuaternionT<Type>::operator*(Type value) const
{
    return QuaternionT(x * value, y * value, z * value, w * value);
}

template <typename Type>
Vector3T<Type> QuaternionT<Type>::operator*(Vector3T<Type> v) const
{
    return Matrix4T<Type>::from_rotation(*this) * v;
}

template <typename Type>
QuaternionT<Type> QuaternionT<Type>::operator*(const QuaternionT& q) const
{
    Vector3T<Type> av(x, y, z);
    Vector3T<Type> bv(q.x, q.y, q.z);

    Vector3T<Type> new_v = (bv * w) + (av * q.w) + av.cross(bv);
    Type new_w = w * q.w - av.dot(bv);

    return QuaternionT<Type>(new_v, new_w);
}

template <typename Type>
QuaternionT<Type> QuaternionT<Type>::operator-() const
{
    return QuaternionT(-x, -y, -z, -w);
}

template <typename Type>
QuaternionT<Type>& QuaternionT<Type>::operator*=(Type value)
{
    x *= value;
    y *= value;
    z *= value;
    w *= value;
    return *this;
}

template <typename Type>
QuaternionT<Type>& QuaternionT<Type>::operator*=(const QuaternionT& q)
{
    return *this = *this * q;
}

template <typename Type>
Type& QuaternionT<Type>::operator[](size_t i)
{
    assert(i < 4);
    return reinterpret_cast<Type*>(this)[i];
}

template <typename Type>
const Type& QuaternionT<Type>::operator[](size_t i) const
{
    assert(i < 4);
    return reinterpret_cast<const Type*>(this)[i];
}

template <typename Type>
bool QuaternionT<Type>::operator==(const QuaternionT& q) const
{
    return x == q.x && y == q.y && z == q.z;
}

template <typename Type>
bool QuaternionT<Type>::operator!=(const QuaternionT& q) const
{
    return !(*this == q);
}

template <typename Type>
template <typename U>
QuaternionT<Type>::operator QuaternionT<U>() const
{
    return QuaternionT<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z), static_cast<U>(w));
}

template <typename Type>
QuaternionT<Type>::operator Vector4T<Type>() const
{
    return Vector4T<Type>(x, y, z, w);
}

template <typename Type>
Encoder& operator<<(Encoder& encoder, QuaternionT<Type> q)
{
    Vector3T<Type> axis;
    Radians angle;
    q.to_axis_angle(axis, angle);
    encoder << begin_object()
            << encode_value("axis", axis)
            << encode_value("angle", angle)
            << end_object();
    return encoder;
}

template <typename Type>
Decoder& operator>>(Decoder& decoder, QuaternionT<Type>& q)
{
    Vector3T<Type> axis;
    Radians angle;
    decoder >> begin_object()
            >> decode_value("axis", axis, true)
            >> decode_value("angle", angle, true)
            >> end_object();
    q = QuaternionT<Type>::from_axis_angle(axis, angle);
    return decoder;
}

}
