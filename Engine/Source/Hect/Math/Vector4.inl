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

template <typename Type>
const Vector4T<Type> Vector4T<Type>::Zero = Vector4T<Type>(0);

template <typename Type>
const Vector4T<Type> Vector4T<Type>::One = Vector4T<Type>(1);

template <typename Type>
const Vector4T<Type> Vector4T<Type>::UnitX = Vector4T<Type>(1, 0, 0, 0);

template <typename Type>
const Vector4T<Type> Vector4T<Type>::UnitY = Vector4T<Type>(0, 1, 0, 0);

template <typename Type>
const Vector4T<Type> Vector4T<Type>::UnitZ = Vector4T<Type>(0, 0, 1, 0);

template <typename Type>
const Vector4T<Type> Vector4T<Type>::UnitW = Vector4T<Type>(0, 0, 0, 1);

template <typename Type>
Vector4T<Type>::Vector4T()
{
}

template <typename Type>
Vector4T<Type>::Vector4T(Type value) :
    x(value),
    y(value),
    z(value),
    w(value)
{
}

template <typename Type>
Vector4T<Type>::Vector4T(Type x, Type y, Type z, Type w) :
    x(x),
    y(y),
    z(z),
    w(w)
{
}

template <typename Type>
Vector4T<Type>::Vector4T(Vector2T<Type> v) :
    x(v.x),
    y(v.y)
{
}

template <typename Type>
Vector4T<Type>::Vector4T(Vector3T<Type> v) :
    x(v.x),
    y(v.y),
    z(v.z)
{
}

template <typename Type>
void Vector4T<Type>::normalize()
{
    Type inv = Type(1) / length();
    *this *= inv;
}

template <typename Type>
Vector4T<Type> Vector4T<Type>::normalized() const
{
    Vector4T v(*this);
    v.normalize();
    return v;
}

template <typename Type>
Type Vector4T<Type>::dot(Vector4T v) const
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

template <typename Type>
Radians Vector4T<Type>::angle_from(Vector4T v) const
{
    double radians = static_cast<double>(std::acos(dot(v)));
    return Radians(radians);
}

template <typename Type>
Type Vector4T<Type>::length() const
{
    return static_cast<Type>(std::sqrt(length_squared()));
}

template <typename Type>
Type Vector4T<Type>::length_squared() const
{
    return dot(*this);
}

template <typename Type>
Vector4T<Type> Vector4T<Type>::floor() const
{
    Vector4T<Type> v;
    v.x = static_cast<Type>(std::floor(x));
    v.y = static_cast<Type>(std::floor(y));
    v.z = static_cast<Type>(std::floor(z));
    v.w = static_cast<Type>(std::floor(w));
    return v;
}

template <typename Type>
Vector4T<Type> Vector4T<Type>::ceil() const
{
    Vector4T<Type> v;
    v.x = static_cast<Type>(std::ceil(x));
    v.y = static_cast<Type>(std::ceil(y));
    v.z = static_cast<Type>(std::ceil(z));
    v.w = static_cast<Type>(std::ceil(w));
    return v;
}

template <typename Type>
Vector4T<Type> Vector4T<Type>::min(Vector4T v) const
{
    Vector4T<Type> m;
    m.x = static_cast<Type>(std::min(v.x, x));
    m.y = static_cast<Type>(std::min(v.y, y));
    m.z = static_cast<Type>(std::min(v.z, z));
    m.w = static_cast<Type>(std::min(v.w, w));
    return m;
}

template <typename Type>
Vector4T<Type> Vector4T<Type>::max(Vector4T v) const
{
    Vector4T<Type> m;
    m.x = static_cast<Type>(std::max(v.x, x));
    m.y = static_cast<Type>(std::max(v.y, y));
    m.z = static_cast<Type>(std::max(v.z, z));
    m.w = static_cast<Type>(std::max(v.w, w));
    return m;
}

template <typename Type>
Vector4T<Type> Vector4T<Type>::abs() const
{
    Vector4T<Type> v;
    v.x = static_cast<Type>(std::abs(x));
    v.y = static_cast<Type>(std::abs(y));
    v.z = static_cast<Type>(std::abs(z));
    v.w = static_cast<Type>(std::abs(w));
    return v;
}

template <typename Type>
Vector4T<Type> Vector4T<Type>::step(Vector4T edge) const
{
    Vector4T<Type> v;
    v.x = x < edge.x ? 0 : 1;
    v.y = y < edge.y ? 0 : 1;
    v.z = z < edge.z ? 0 : 1;
    v.w = w < edge.w ? 0 : 1;
    return v;
}

template <typename Type>
Vector4T<Type> Vector4T<Type>::operator+(Vector4T v) const
{
    return Vector4T(x + v.x, y + v.y, z + v.z, w + v.w);
}

template <typename Type>
Vector4T<Type> Vector4T<Type>::operator-(Vector4T v) const
{
    return Vector4T(x - v.x, y - v.y, z - v.z, w - v.w);
}

template <typename Type>
Vector4T<Type> Vector4T<Type>::operator*(Type value) const
{
    return Vector4T(x * value, y * value, z * value, w * value);
}

template <typename Type>
Vector4T<Type> Vector4T<Type>::operator*(Vector4T v) const
{
    return Vector4T(x * v.x, y * v.y, z * v.z, w * v.w);
}

template <typename Type>
Vector4T<Type> Vector4T<Type>::operator/(Type value) const
{
    return Vector4T(x / value, y / value, z / value, w / value);
}

template <typename Type>
Vector4T<Type> Vector4T<Type>::operator/(Vector4T v) const
{
    return Vector4T(x / v.x, y / v.y, z / v.z, w / v.w);
}

template <typename Type>
Vector4T<Type> Vector4T<Type>::operator-() const
{
    return Vector4T(-x, -y, -z, -w);
}

template <typename Type>
Vector4T<Type>& Vector4T<Type>::operator+=(Vector4T v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

template <typename Type>
Vector4T<Type>& Vector4T<Type>::operator-=(Vector4T v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

template <typename Type>
Vector4T<Type>& Vector4T<Type>::operator*=(Type value)
{
    x *= value;
    y *= value;
    z *= value;
    w *= value;
    return *this;
}

template <typename Type>
Vector4T<Type>& Vector4T<Type>::operator*=(Vector4T v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
}

template <typename Type>
Vector4T<Type>& Vector4T<Type>::operator/=(Type value)
{
    x /= value;
    y /= value;
    z /= value;
    w /= value;
    return *this;
}

template <typename Type>
Vector4T<Type>& Vector4T<Type>::operator/=(Vector4T v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
}

template <typename Type>
Type& Vector4T<Type>::operator[](size_t i)
{
    assert(i < 4);
    return reinterpret_cast<Type*>(this)[i];
}

template <typename Type>
const Type& Vector4T<Type>::operator[](size_t i) const
{
    assert(i < 4);
    return reinterpret_cast<const Type*>(this)[i];
}

template <typename Type>
bool Vector4T<Type>::operator<(Vector4T v) const
{
    return x < v.x && y < v.y && z < v.z && w < v.w;
}

template <typename Type>
bool Vector4T<Type>::operator==(Vector4T v) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

template <typename Type>
bool Vector4T<Type>::operator!=(Vector4T v) const
{
    return !(*this == v);
}

template <typename Type>
template <typename U>
Vector4T<Type>::operator Vector4T<U>() const
{
    return Vector4T<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z), static_cast<U>(w));
}

template <typename Type>
Encoder& operator<<(Encoder& encoder, Vector4T<Type> v)
{
    encoder << begin_array() << v.x << v.y << v.z << v.w << end_array();
    return encoder;
}

template <typename Type>
Decoder& operator>>(Decoder& decoder, Vector4T<Type>& v)
{
    decoder >> begin_array() >> v.x >> v.y >> v.z >> v.w >> end_array();
    return decoder;
}

}
