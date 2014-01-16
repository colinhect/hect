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
Vector4<T> Vector4<T>::zero()
{
    return Vector4(0);
}

template <typename T>
Vector4<T> Vector4<T>::one()
{
    return Vector4(1);
}

template <typename T>
Vector4<T> Vector4<T>::unitX()
{
    return Vector4(1, 0, 0, 0);
}

template <typename T>
Vector4<T> Vector4<T>::unitY()
{
    return Vector4(0, 1, 0, 0);
}

template <typename T>
Vector4<T> Vector4<T>::unitZ()
{
    return Vector4(0, 0, 1, 0);
}

template <typename T>
Vector4<T> Vector4<T>::unitW()
{
    return Vector4(0, 0, 0, 1);
}

template <typename T>
Vector4<T>::Vector4() :
    x(0),
    y(0),
    z(0),
    w(0)
{
}

template <typename T>
Vector4<T>::Vector4(T value) :
    x(value),
    y(value),
    z(value),
    w(value)
{
}

template <typename T>
Vector4<T>::Vector4(T x, T y, T z, T w) :
    x(x),
    y(y),
    z(z),
    w(w)
{
}

template <typename T>
void Vector4<T>::normalize()
{
    T inv = (T)1.0 / length();
    *this *= inv;
}

template <typename T>
Vector4<T> Vector4<T>::normalized() const
{
    Vector4 v(*this);
    v.normalize();
    return v;
}

template <typename T>
T Vector4<T>::dot(const Vector4& v) const
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

template <typename T>
Angle<T> Vector4<T>::angleFrom(const Vector4& v) const
{
    T radians = (T)std::acos(dot(v));
    return Angle<T>::fromRadians(radians);
}

template <typename T>
T Vector4<T>::length() const
{
    return (T)std::sqrt(lengthSquared());
}

template <typename T>
T Vector4<T>::lengthSquared() const
{
    return dot(*this);
}

template <typename T>
Vector4<T> Vector4<T>::operator+(const Vector4& v) const
{
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

template <typename T>
Vector4<T> Vector4<T>::operator-(const Vector4& v) const
{
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

template <typename T>
Vector4<T> Vector4<T>::operator*(T value) const
{
    return Vector4(x * value, y * value, z * value, w * value);
}

template <typename T>
Vector4<T> Vector4<T>::operator*(const Vector4& v) const
{
    return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

template <typename T>
Vector4<T> Vector4<T>::operator/(T value) const
{
    return Vector4(x / value, y / value, z / value, w / value);
}

template <typename T>
Vector4<T> Vector4<T>::operator/(const Vector4& v) const
{
    return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

template <typename T>
Vector4<T> Vector4<T>::operator-() const
{
    return Vector4(-x, -y, -z, -w);
}

template <typename T>
Vector4<T>& Vector4<T>::operator+=(const Vector4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

template <typename T>
Vector4<T>& Vector4<T>::operator-=(const Vector4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

template <typename T>
Vector4<T>& Vector4<T>::operator*=(T value)
{
    x *= value;
    y *= value;
    z *= value;
    w *= value;
    return *this;
}

template <typename T>
Vector4<T>& Vector4<T>::operator*=(const Vector4& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
}

template <typename T>
Vector4<T>& Vector4<T>::operator/=(T value)
{
    x /= value;
    y /= value;
    z /= value;
    w /= value;
    return *this;
}

template <typename T>
Vector4<T>& Vector4<T>::operator/=(const Vector4& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
}

template <typename T>
T& Vector4<T>::operator[](size_t i)
{
    assert(i < 4);
    return ((T*)this)[i];
}

template <typename T>
const T& Vector4<T>::operator[](size_t i) const
{
    assert(i < 4);
    return ((const T*)this)[i];
}

}