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

namespace hect
{

template <typename T>
const Vector4T<T> Vector4T<T>::Zero = Vector4T<T>(0);

template <typename T>
const Vector4T<T> Vector4T<T>::One = Vector4T<T>(1);

template <typename T>
const Vector4T<T> Vector4T<T>::UnitX = Vector4T<T>(1, 0, 0, 0);

template <typename T>
const Vector4T<T> Vector4T<T>::UnitY = Vector4T<T>(0, 1, 0, 0);

template <typename T>
const Vector4T<T> Vector4T<T>::UnitZ = Vector4T<T>(0, 0, 1, 0);

template <typename T>
const Vector4T<T> Vector4T<T>::UnitW = Vector4T<T>(0, 0, 0, 1);

template <typename T>
Vector4T<T>::Vector4T()
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
Vector4T<T>::Vector4T(Vector2T<T> v) :
    x(v.x),
    y(v.y)
{
}

template <typename T>
Vector4T<T>::Vector4T(Vector3T<T> v) :
    x(v.x),
    y(v.y),
    z(v.z)
{
}

template <typename T>
void Vector4T<T>::normalize()
{
    T inv = T(1) / length();
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
T Vector4T<T>::dot(Vector4T v) const
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

template <typename T>
Radians Vector4T<T>::angleFrom(Vector4T v) const
{
    double radians = static_cast<double>(std::acos(dot(v)));
    return Radians(radians);
}

template <typename T>
T Vector4T<T>::length() const
{
    return static_cast<T>(std::sqrt(lengthSquared()));
}

template <typename T>
T Vector4T<T>::lengthSquared() const
{
    return dot(*this);
}

template <typename T>
Vector4T<T> Vector4T<T>::floor() const
{
    Vector4T<T> v;
    v.x = static_cast<T>(std::floor(x));
    v.y = static_cast<T>(std::floor(y));
    v.z = static_cast<T>(std::floor(z));
    v.w = static_cast<T>(std::floor(w));
    return v;
}

template <typename T>
Vector4T<T> Vector4T<T>::ceil() const
{
    Vector4T<T> v;
    v.x = static_cast<T>(std::ceil(x));
    v.y = static_cast<T>(std::ceil(y));
    v.z = static_cast<T>(std::ceil(z));
    v.w = static_cast<T>(std::ceil(w));
    return v;
}

template <typename T>
Vector4T<T> Vector4T<T>::min(Vector4T v) const
{
    Vector4T<T> m;
    m.x = static_cast<T>(std::min(v.x, x));
    m.y = static_cast<T>(std::min(v.y, y));
    m.z = static_cast<T>(std::min(v.z, z));
    m.w = static_cast<T>(std::min(v.w, w));
    return m;
}

template <typename T>
Vector4T<T> Vector4T<T>::max(Vector4T v) const
{
    Vector4T<T> m;
    m.x = static_cast<T>(std::max(v.x, x));
    m.y = static_cast<T>(std::max(v.y, y));
    m.z = static_cast<T>(std::max(v.z, z));
    m.w = static_cast<T>(std::max(v.w, w));
    return m;
}

template <typename T>
Vector4T<T> Vector4T<T>::abs() const
{
    Vector4T<T> v;
    v.x = static_cast<T>(std::abs(x));
    v.y = static_cast<T>(std::abs(y));
    v.z = static_cast<T>(std::abs(z));
    v.w = static_cast<T>(std::abs(w));
    return v;
}

template <typename T>
Vector4T<T> Vector4T<T>::step(Vector4T edge) const
{
    Vector4T<T> v;
    v.x = x < edge.x ? 0 : 1;
    v.y = y < edge.y ? 0 : 1;
    v.z = z < edge.z ? 0 : 1;
    v.w = w < edge.w ? 0 : 1;
    return v;
}

template <typename T>
Vector4T<T> Vector4T<T>::operator+(Vector4T v) const
{
    return Vector4T(x + v.x, y + v.y, z + v.z, w + v.w);
}

template <typename T>
Vector4T<T> Vector4T<T>::operator-(Vector4T v) const
{
    return Vector4T(x - v.x, y - v.y, z - v.z, w - v.w);
}

template <typename T>
Vector4T<T> Vector4T<T>::operator*(T value) const
{
    return Vector4T(x * value, y * value, z * value, w * value);
}

template <typename T>
Vector4T<T> Vector4T<T>::operator*(Vector4T v) const
{
    return Vector4T(x * v.x, y * v.y, z * v.z, w * v.w);
}

template <typename T>
Vector4T<T> Vector4T<T>::operator/(T value) const
{
    return Vector4T(x / value, y / value, z / value, w / value);
}

template <typename T>
Vector4T<T> Vector4T<T>::operator/(Vector4T v) const
{
    return Vector4T(x / v.x, y / v.y, z / v.z, w / v.w);
}

template <typename T>
Vector4T<T> Vector4T<T>::operator-() const
{
    return Vector4T(-x, -y, -z, -w);
}

template <typename T>
Vector4T<T>& Vector4T<T>::operator+=(Vector4T v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

template <typename T>
Vector4T<T>& Vector4T<T>::operator-=(Vector4T v)
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
Vector4T<T>& Vector4T<T>::operator*=(Vector4T v)
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
Vector4T<T>& Vector4T<T>::operator/=(Vector4T v)
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
    return reinterpret_cast<T*>(this)[i];
}

template <typename T>
const T& Vector4T<T>::operator[](size_t i) const
{
    assert(i < 4);
    return reinterpret_cast<const T*>(this)[i];
}

template <typename T>
bool Vector4T<T>::operator<(Vector4T v) const
{
    return x < v.x && y < v.y && z < v.z && w < v.w;
}

template <typename T>
bool Vector4T<T>::operator==(Vector4T v) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

template <typename T>
bool Vector4T<T>::operator!=(Vector4T v) const
{
    return !(*this == v);
}

template <typename T>
template <typename U>
Vector4T<T>::operator Vector4T<U>() const
{
    return Vector4T<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z), static_cast<U>(w));
}

template <typename T>
Encoder& operator<<(Encoder& encoder, Vector4T<T> v)
{
    encoder << beginArray() << v.x << v.y << v.z << v.w << endArray();
    return encoder;
}

template <typename T>
Decoder& operator>>(Decoder& decoder, Vector4T<T>& v)
{
    decoder >> beginArray() >> v.x >> v.y >> v.z >> v.w >> endArray();
    return decoder;
}

}
