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
Vector2T<T>::Vector2T() :
    x(0),
    y(0)
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
    T inv = (T)1.0 / length();
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
AngleT<T> Vector2T<T>::angleFrom(const Vector2T& v) const
{
    T radians = (T)std::acos(dot(v));
    return AngleT<T>::fromRadians(radians);
}

template <typename T>
T Vector2T<T>::length() const
{
    return (T)std::sqrt(lengthSquared());
}

template <typename T>
T Vector2T<T>::lengthSquared() const
{
    return dot(*this);
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
    return ((T*)this)[i];
}

template <typename T>
const T& Vector2T<T>::operator[](size_t i) const
{
    assert(i < 2);
    return ((const T*)this)[i];
}

}