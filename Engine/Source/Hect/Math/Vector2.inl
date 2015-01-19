///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
Vector2T<T> Vector2T<T>::zero()
{
    return Vector2T(0);
}

template <typename T>
Vector2T<T> Vector2T<T>::one()
{
    return Vector2T(1);
}

template <typename T>
Vector2T<T> Vector2T<T>::unitX()
{
    return Vector2T(1, 0);
}

template <typename T>
Vector2T<T> Vector2T<T>::unitY()
{
    return Vector2T(0, 1);
}

template <typename T>
Vector2T<T>::Vector2T()
{
}

template <typename T>
Vector2T<T>::Vector2T(T value) :
    x(value),
    y(value)
{
}

template <typename T>
Vector2T<T>::Vector2T(T x, T y) :
    x(x),
    y(y)
{
}

template <typename T>
void Vector2T<T>::normalize()
{
    T inv = T(1) / length();
    *this *= inv;
}

template <typename T>
Vector2T<T> Vector2T<T>::normalized() const
{
    Vector2T v(*this);
    v.normalize();
    return v;
}

template <typename T>
T Vector2T<T>::dot(const Vector2T& v) const
{
    return x * v.x + y * v.y;
}

template <typename T>
Angle Vector2T<T>::angleFrom(const Vector2T& v) const
{
    double radians = static_cast<double>(std::acos(dot(v)));
    return Angle::fromRadians(radians);
}

template <typename T>
T Vector2T<T>::length() const
{
    return static_cast<T>(std::sqrt(lengthSquared()));
}

template <typename T>
T Vector2T<T>::lengthSquared() const
{
    return dot(*this);
}

template <typename T>
Vector2T<T> Vector2T<T>::floor() const
{
    Vector2T<T> v;
    v.x = static_cast<T>(std::floor(x));
    v.y = static_cast<T>(std::floor(y));
    return v;
}

template <typename T>
Vector2T<T> Vector2T<T>::ceil() const
{
    Vector2T<T> v;
    v.x = static_cast<T>(std::ceil(x));
    v.y = static_cast<T>(std::ceil(y));
    return v;
}

template <typename T>
Vector2T<T> Vector2T<T>::min(const Vector2T& v) const
{
    Vector2T<T> m;
    m.x = static_cast<T>(std::min(v.x, x));
    m.y = static_cast<T>(std::min(v.y, y));
    return m;
}

template <typename T>
Vector2T<T> Vector2T<T>::max(const Vector2T& v) const
{
    Vector2T<T> m;
    m.x = static_cast<T>(std::max(v.x, x));
    m.y = static_cast<T>(std::max(v.y, y));
    return m;
}

template <typename T>
Vector2T<T> Vector2T<T>::abs() const
{
    Vector2T<T> v;
    v.x = static_cast<T>(std::abs(x));
    v.y = static_cast<T>(std::abs(y));
    return v;
}

template <typename T>
Vector2T<T> Vector2T<T>::step(const Vector2T& edge) const
{
    Vector2T<T> v;
    v.x = x < edge.x ? 0 : 1;
    v.y = y < edge.y ? 0 : 1;
    return v;
}

template <typename T>
Vector2T<T> Vector2T<T>::operator+(const Vector2T& v) const
{
    return Vector2T(x + v.x, y + v.y);
}

template <typename T>
Vector2T<T> Vector2T<T>::operator-(const Vector2T& v) const
{
    return Vector2T(x - v.x, y - v.y);
}

template <typename T>
Vector2T<T> Vector2T<T>::operator*(T value) const
{
    return Vector2T(x * value, y * value);
}

template <typename T>
Vector2T<T> Vector2T<T>::operator*(const Vector2T& v) const
{
    return Vector2T(x * v.x, y * v.y);
}

template <typename T>
Vector2T<T> Vector2T<T>::operator/(T value) const
{
    return Vector2T(x / value, y / value);
}

template <typename T>
Vector2T<T> Vector2T<T>::operator/(const Vector2T& v) const
{
    return Vector2T(x / v.x, y / v.y);
}

template <typename T>
Vector2T<T> Vector2T<T>::operator-() const
{
    return Vector2T(-x, -y);
}

template <typename T>
Vector2T<T>& Vector2T<T>::operator+=(const Vector2T& v)
{
    x += v.x;
    y += v.y;
    return *this;
}

template <typename T>
Vector2T<T>& Vector2T<T>::operator-=(const Vector2T& v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

template <typename T>
Vector2T<T>& Vector2T<T>::operator*=(T value)
{
    x *= value;
    y *= value;
    return *this;
}

template <typename T>
Vector2T<T>& Vector2T<T>::operator*=(const Vector2T& v)
{
    x *= v.x;
    y *= v.y;
    return *this;
}

template <typename T>
Vector2T<T>& Vector2T<T>::operator/=(T value)
{
    x /= value;
    y /= value;
    return *this;
}

template <typename T>
Vector2T<T>& Vector2T<T>::operator/=(const Vector2T& v)
{
    x /= v.x;
    y /= v.y;
    return *this;
}

template <typename T>
T& Vector2T<T>::operator[](size_t i)
{
    assert(i < 2);
    return reinterpret_cast<T*>(this)[i];
}

template <typename T>
const T& Vector2T<T>::operator[](size_t i) const
{
    assert(i < 2);
    return reinterpret_cast<const T*>(this)[i];
}

template <typename T>
bool Vector2T<T>::operator<(const Vector2T& v) const
{
    return x < v.x && y < v.y;
}

template <typename T>
bool Vector2T<T>::operator==(const Vector2T& v) const
{
    return x == v.x && y == v.y;
}

template <typename T>
bool Vector2T<T>::operator!=(const Vector2T& v) const
{
    return !(*this == v);
}

template <typename T>
template <typename U>
Vector2T<T>::operator Vector2T<U>() const
{
    return Vector2T<U>(static_cast<U>(x), static_cast<U>(y));
}

template <typename T>
Encoder& operator<<(Encoder& encoder, const Vector2T<T>& v)
{
    encoder << beginArray() << v.x << v.y << endArray();
    return encoder;
}

template <typename T>
Decoder& operator>>(Decoder& decoder, Vector2T<T>& v)
{
    decoder >> beginArray() >> v.x >> v.y >> endArray();
    return decoder;
}

}
