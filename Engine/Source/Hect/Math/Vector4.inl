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
Vector4T<T> Vector4T<T>::zero()
{
    return Vector4T(0);
}

template <typename T>
Vector4T<T> Vector4T<T>::one()
{
    return Vector4T(1);
}

template <typename T>
Vector4T<T> Vector4T<T>::unitX()
{
    return Vector4T(1, 0, 0, 0);
}

template <typename T>
Vector4T<T> Vector4T<T>::unitY()
{
    return Vector4T(0, 1, 0, 0);
}

template <typename T>
Vector4T<T> Vector4T<T>::unitZ()
{
    return Vector4T(0, 0, 1, 0);
}

template <typename T>
Vector4T<T> Vector4T<T>::unitW()
{
    return Vector4T(0, 0, 0, 1);
}

template <typename T>
Vector4T<T>::Vector4T() :
    x(0),
    y(0),
    z(0),
    w(0)
{
}

template <typename T>
Vector4T<T>::Vector4T(T value) :
    x(value),
    y(value),
    z(value),
    w(value)
{
}

template <typename T>
Vector4T<T>::Vector4T(T x, T y, T z, T w) :
    x(x),
    y(y),
    z(z),
    w(w)
{
}

template <typename T>
void Vector4T<T>::normalize()
{
    T inv = (T)1.0 / length();
    *this *= inv;
}

template <typename T>
Vector4T<T> Vector4T<T>::normalized() const
{
    Vector4T v(*this);
    v.normalize();
    return v;
}

template <typename T>
T Vector4T<T>::dot(const Vector4T& v) const
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

template <typename T>
Angle Vector4T<T>::angleFrom(const Vector4T& v) const
{
    Real radians = static_cast<Real>(std::acos(dot(v)));
    return Angle::fromRadians(radians);
}

template <typename T>
T Vector4T<T>::length() const
{
    return (T)std::sqrt(lengthSquared());
}

template <typename T>
T Vector4T<T>::lengthSquared() const
{
    return dot(*this);
}

template <typename T>
Vector4T<T> Vector4T<T>::operator+(const Vector4T& v) const
{
    return Vector4T(x + v.x, y + v.y, z + v.z, w + v.w);
}

template <typename T>
Vector4T<T> Vector4T<T>::operator-(const Vector4T& v) const
{
    return Vector4T(x - v.x, y - v.y, z - v.z, w - v.w);
}

template <typename T>
Vector4T<T> Vector4T<T>::operator*(T value) const
{
    return Vector4T(x * value, y * value, z * value, w * value);
}

template <typename T>
Vector4T<T> Vector4T<T>::operator*(const Vector4T& v) const
{
    return Vector4T(x * v.x, y * v.y, z * v.z, w * v.w);
}

template <typename T>
Vector4T<T> Vector4T<T>::operator/(T value) const
{
    return Vector4T(x / value, y / value, z / value, w / value);
}

template <typename T>
Vector4T<T> Vector4T<T>::operator/(const Vector4T& v) const
{
    return Vector4T(x / v.x, y / v.y, z / v.z, w / v.w);
}

template <typename T>
Vector4T<T> Vector4T<T>::operator-() const
{
    return Vector4T(-x, -y, -z, -w);
}

template <typename T>
Vector4T<T>& Vector4T<T>::operator+=(const Vector4T& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

template <typename T>
Vector4T<T>& Vector4T<T>::operator-=(const Vector4T& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

template <typename T>
Vector4T<T>& Vector4T<T>::operator*=(T value)
{
    x *= value;
    y *= value;
    z *= value;
    w *= value;
    return *this;
}

template <typename T>
Vector4T<T>& Vector4T<T>::operator*=(const Vector4T& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
}

template <typename T>
Vector4T<T>& Vector4T<T>::operator/=(T value)
{
    x /= value;
    y /= value;
    z /= value;
    w /= value;
    return *this;
}

template <typename T>
Vector4T<T>& Vector4T<T>::operator/=(const Vector4T& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
}

template <typename T>
T& Vector4T<T>::operator[](size_t i)
{
    assert(i < 4);
    return ((T*)this)[i];
}

template <typename T>
const T& Vector4T<T>::operator[](size_t i) const
{
    assert(i < 4);
    return ((const T*)this)[i];
}

template <typename T>
bool Vector4T<T>::operator==(const Vector4T& v) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

template <typename T>
bool Vector4T<T>::operator!=(const Vector4T& v) const
{
    return !(*this == v);
}

template <typename T>
Encoder& operator<<(Encoder& encoder, const Vector4T<T>& v)
{
    return encoder << beginArray() << v.x << v.y << v.z << v.w << endArray();
}

template <typename T>
Decoder& operator>>(Decoder& decoder, Vector4T<T>& v)
{
    return decoder >> beginArray() >> v.x >> v.y >> v.z >> v.w >> endArray();
}

}