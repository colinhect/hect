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
#include <algorithm>
#include <cassert>
#include <cmath>

#include "Hect/Math/Radians.h"

namespace hect
{

template <typename T>
const Vector3T<T> Vector3T<T>::Zero = Vector3T<T>(0);

template <typename T>
const Vector3T<T> Vector3T<T>::One = Vector3T<T>(1);

template <typename T>
const Vector3T<T> Vector3T<T>::UnitX = Vector3T<T>(1, 0, 0);

template <typename T>
const Vector3T<T> Vector3T<T>::UnitY = Vector3T<T>(0, 1, 0);

template <typename T>
const Vector3T<T> Vector3T<T>::UnitZ = Vector3T<T>(0, 0, 1);

template <typename T>
Vector3T<T>::Vector3T()
{
}

template <typename T>
Vector3T<T>::Vector3T(T value) :
    x(value),
    y(value),
    z(value)
{
}

template <typename T>
Vector3T<T>::Vector3T(T x, T y, T z) :
    x(x),
    y(y),
    z(z)
{
}

template <typename T>
Vector3T<T>::Vector3T(const Vector2T<T>& v) :
    x(v.x),
    y(v.y)
{
}

template <typename T>
void Vector3T<T>::normalize()
{
    T inv = T(1) / length();
    *this *= inv;
}

template <typename T>
Vector3T<T> Vector3T<T>::normalized() const
{
    Vector3T v(*this);
    v.normalize();
    return v;
}

template <typename T>
T Vector3T<T>::dot(const Vector3T& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

template <typename T>
Vector3T<T> Vector3T<T>::cross(const Vector3T& v) const
{
    return Vector3T(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

template <typename T>
Angle Vector3T<T>::angleFrom(const Vector3T& v) const
{
    double radians = static_cast<double>(std::acos(dot(v)));
    return Radians(radians);
}

template <typename T>
T Vector3T<T>::length() const
{
    return static_cast<T>(std::sqrt(lengthSquared()));
}

template <typename T>
T Vector3T<T>::lengthSquared() const
{
    return dot(*this);
}

template <typename T>
Vector3T<T> Vector3T<T>::floor() const
{
    Vector3T<T> v;
    v.x = static_cast<T>(std::floor(x));
    v.y = static_cast<T>(std::floor(y));
    v.z = static_cast<T>(std::floor(z));
    return v;
}

template <typename T>
Vector3T<T> Vector3T<T>::ceil() const
{
    Vector3T<T> v;
    v.x = static_cast<T>(std::ceil(x));
    v.y = static_cast<T>(std::ceil(y));
    v.z = static_cast<T>(std::ceil(z));
    return v;
}

template <typename T>
Vector3T<T> Vector3T<T>::min(const Vector3T& v) const
{
    Vector3T<T> m;
    m.x = static_cast<T>(std::min(v.x, x));
    m.y = static_cast<T>(std::min(v.y, y));
    m.z = static_cast<T>(std::min(v.z, z));
    return m;
}

template <typename T>
Vector3T<T> Vector3T<T>::max(const Vector3T& v) const
{
    Vector3T<T> m;
    m.x = static_cast<T>(std::max(v.x, x));
    m.y = static_cast<T>(std::max(v.y, y));
    m.z = static_cast<T>(std::max(v.z, z));
    return m;
}

template <typename T>
Vector3T<T> Vector3T<T>::abs() const
{
    Vector3T<T> v;
    v.x = static_cast<T>(std::abs(x));
    v.y = static_cast<T>(std::abs(y));
    v.z = static_cast<T>(std::abs(z));
    return v;
}

template <typename T>
Vector3T<T> Vector3T<T>::step(const Vector3T& edge) const
{
    Vector3T<T> v;
    v.x = x < edge.x ? 0 : 1;
    v.y = y < edge.y ? 0 : 1;
    v.z = z < edge.z ? 0 : 1;
    return v;
}

template <typename T>
Vector3T<T> Vector3T<T>::operator+(const Vector3T& v) const
{
    return Vector3T(x + v.x, y + v.y, z + v.z);
}

template <typename T>
Vector3T<T> Vector3T<T>::operator-(const Vector3T& v) const
{
    return Vector3T(x - v.x, y - v.y, z - v.z);
}

template <typename T>
Vector3T<T> Vector3T<T>::operator*(T value) const
{
    return Vector3T(x * value, y * value, z * value);
}

template <typename T>
Vector3T<T> Vector3T<T>::operator*(const Vector3T& v) const
{
    return Vector3T(x * v.x, y * v.y, z * v.z);
}

template <typename T>
Vector3T<T> Vector3T<T>::operator/(T value) const
{
    return Vector3T(x / value, y / value, z / value);
}

template <typename T>
Vector3T<T> Vector3T<T>::operator/(const Vector3T& v) const
{
    return Vector3T(x / v.x, y / v.y, z / v.z);
}

template <typename T>
Vector3T<T> Vector3T<T>::operator-() const
{
    return Vector3T(-x, -y, -z);
}

template <typename T>
Vector3T<T>& Vector3T<T>::operator+=(const Vector3T& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

template <typename T>
Vector3T<T>& Vector3T<T>::operator-=(const Vector3T& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

template <typename T>
Vector3T<T>& Vector3T<T>::operator*=(T value)
{
    x *= value;
    y *= value;
    z *= value;
    return *this;
}

template <typename T>
Vector3T<T>& Vector3T<T>::operator*=(const Vector3T& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

template <typename T>
Vector3T<T>& Vector3T<T>::operator/=(T value)
{
    x /= value;
    y /= value;
    z /= value;
    return *this;
}

template <typename T>
Vector3T<T>& Vector3T<T>::operator/=(const Vector3T& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}

template <typename T>
T& Vector3T<T>::operator[](size_t i)
{
    assert(i < 3);
    return reinterpret_cast<T*>(this)[i];
}

template <typename T>
const T& Vector3T<T>::operator[](size_t i) const
{
    assert(i < 3);
    return reinterpret_cast<const T*>(this)[i];
}

template <typename T>
bool Vector3T<T>::operator<(const Vector3T& v) const
{
    return x < v.x && y < v.y && z < v.z;
}

template <typename T>
bool Vector3T<T>::operator==(const Vector3T& v) const
{
    return x == v.x && y == v.y && z == v.z;
}

template <typename T>
bool Vector3T<T>::operator!=(const Vector3T& v) const
{
    return !(*this == v);
}

template <typename T>
template <typename U>
Vector3T<T>::operator Vector3T<U>() const
{
    return Vector3T<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z));
}

template <typename T>
Encoder& operator<<(Encoder& encoder, const Vector3T<T>& v)
{
    encoder << beginArray() << v.x << v.y << v.z << endArray();
    return encoder;
}

template <typename T>
Decoder& operator>>(Decoder& decoder, Vector3T<T>& v)
{
    decoder >> beginArray() >> v.x >> v.y >> v.z >> endArray();
    return decoder;
}

}
