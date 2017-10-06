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
const Vector3T<Type> Vector3T<Type>::Zero = Vector3T<Type>(0);

template <typename Type>
const Vector3T<Type> Vector3T<Type>::One = Vector3T<Type>(1);

template <typename Type>
const Vector3T<Type> Vector3T<Type>::UnitX = Vector3T<Type>(1, 0, 0);

template <typename Type>
const Vector3T<Type> Vector3T<Type>::UnitY = Vector3T<Type>(0, 1, 0);

template <typename Type>
const Vector3T<Type> Vector3T<Type>::UnitZ = Vector3T<Type>(0, 0, 1);

template <typename Type>
Vector3T<Type>::Vector3T()
{
}

template <typename Type>
Vector3T<Type>::Vector3T(Type value) :
    x(value),
    y(value),
    z(value)
{
}

template <typename Type>
Vector3T<Type>::Vector3T(Type x, Type y, Type z) :
    x(x),
    y(y),
    z(z)
{
}

template <typename Type>
Vector3T<Type>::Vector3T(Vector2T<Type> v) :
    x(v.x),
    y(v.y)
{
}

template <typename Type>
void Vector3T<Type>::normalize()
{
    Type inv = Type(1) / length();
    *this *= inv;
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::normalized() const
{
    Vector3T v(*this);
    v.normalize();
    return v;
}

template <typename Type>
Type Vector3T<Type>::dot(Vector3T v) const
{
    return x * v.x + y * v.y + z * v.z;
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::cross(Vector3T v) const
{
    return Vector3T(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

template <typename Type>
Radians Vector3T<Type>::angle_from(Vector3T v) const
{
    double radians = static_cast<double>(std::acos(dot(v)));
    return Radians(radians);
}

template <typename Type>
Type Vector3T<Type>::length() const
{
    return static_cast<Type>(std::sqrt(length_squared()));
}

template <typename Type>
Type Vector3T<Type>::length_squared() const
{
    return dot(*this);
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::floor() const
{
    Vector3T<Type> v;
    v.x = static_cast<Type>(std::floor(x));
    v.y = static_cast<Type>(std::floor(y));
    v.z = static_cast<Type>(std::floor(z));
    return v;
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::ceil() const
{
    Vector3T<Type> v;
    v.x = static_cast<Type>(std::ceil(x));
    v.y = static_cast<Type>(std::ceil(y));
    v.z = static_cast<Type>(std::ceil(z));
    return v;
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::min(Vector3T v) const
{
    Vector3T<Type> m;
    m.x = static_cast<Type>(std::min(v.x, x));
    m.y = static_cast<Type>(std::min(v.y, y));
    m.z = static_cast<Type>(std::min(v.z, z));
    return m;
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::max(Vector3T v) const
{
    Vector3T<Type> m;
    m.x = static_cast<Type>(std::max(v.x, x));
    m.y = static_cast<Type>(std::max(v.y, y));
    m.z = static_cast<Type>(std::max(v.z, z));
    return m;
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::abs() const
{
    Vector3T<Type> v;
    v.x = static_cast<Type>(std::abs(x));
    v.y = static_cast<Type>(std::abs(y));
    v.z = static_cast<Type>(std::abs(z));
    return v;
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::step(Vector3T edge) const
{
    Vector3T<Type> v;
    v.x = x < edge.x ? 0 : 1;
    v.y = y < edge.y ? 0 : 1;
    v.z = z < edge.z ? 0 : 1;
    return v;
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::operator+(Vector3T v) const
{
    return Vector3T(x + v.x, y + v.y, z + v.z);
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::operator-(Vector3T v) const
{
    return Vector3T(x - v.x, y - v.y, z - v.z);
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::operator*(Type value) const
{
    return Vector3T(x * value, y * value, z * value);
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::operator*(Vector3T v) const
{
    return Vector3T(x * v.x, y * v.y, z * v.z);
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::operator/(Type value) const
{
    return Vector3T(x / value, y / value, z / value);
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::operator/(Vector3T v) const
{
    return Vector3T(x / v.x, y / v.y, z / v.z);
}

template <typename Type>
Vector3T<Type> Vector3T<Type>::operator-() const
{
    return Vector3T(-x, -y, -z);
}

template <typename Type>
Vector3T<Type>& Vector3T<Type>::operator+=(Vector3T v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

template <typename Type>
Vector3T<Type>& Vector3T<Type>::operator-=(Vector3T v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

template <typename Type>
Vector3T<Type>& Vector3T<Type>::operator*=(Type value)
{
    x *= value;
    y *= value;
    z *= value;
    return *this;
}

template <typename Type>
Vector3T<Type>& Vector3T<Type>::operator*=(Vector3T v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

template <typename Type>
Vector3T<Type>& Vector3T<Type>::operator/=(Type value)
{
    x /= value;
    y /= value;
    z /= value;
    return *this;
}

template <typename Type>
Vector3T<Type>& Vector3T<Type>::operator/=(Vector3T v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}

template <typename Type>
Type& Vector3T<Type>::operator[](size_t i)
{
    assert(i < 3);
    return reinterpret_cast<Type*>(this)[i];
}

template <typename Type>
const Type& Vector3T<Type>::operator[](size_t i) const
{
    assert(i < 3);
    return reinterpret_cast<const Type*>(this)[i];
}

template <typename Type>
bool Vector3T<Type>::operator<(Vector3T v) const
{
    return x < v.x && y < v.y && z < v.z;
}

template <typename Type>
bool Vector3T<Type>::operator==(Vector3T v) const
{
    return x == v.x && y == v.y && z == v.z;
}

template <typename Type>
bool Vector3T<Type>::operator!=(Vector3T v) const
{
    return !(*this == v);
}

template <typename Type>
template <typename U>
Vector3T<Type>::operator Vector3T<U>() const
{
    return Vector3T<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z));
}

template <typename Type>
Encoder& operator<<(Encoder& encoder, Vector3T<Type> v)
{
    encoder << begin_array() << v.x << v.y << v.z << end_array();
    return encoder;
}

template <typename Type>
Decoder& operator>>(Decoder& decoder, Vector3T<Type>& v)
{
    decoder >> begin_array() >> v.x >> v.y >> v.z >> end_array();
    return decoder;
}

}
