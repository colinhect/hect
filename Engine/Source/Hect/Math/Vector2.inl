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
const Vector2T<Type> Vector2T<Type>::Zero = Vector2T<Type>(0);

template <typename Type>
const Vector2T<Type> Vector2T<Type>::One = Vector2T<Type>(1);

template <typename Type>
const Vector2T<Type> Vector2T<Type>::UnitX = Vector2T<Type>(1, 0);

template <typename Type>
const Vector2T<Type> Vector2T<Type>::UnitY = Vector2T<Type>(0, 1);

template <typename Type>
Vector2T<Type>::Vector2T()
{
}

template <typename Type>
Vector2T<Type>::Vector2T(Type value) :
    x(value),
    y(value)
{
}

template <typename Type>
Vector2T<Type>::Vector2T(Type x, Type y) :
    x(x),
    y(y)
{
}

template <typename Type>
void Vector2T<Type>::normalize()
{
    Type inv = Type(1) / length();
    *this *= inv;
}

template <typename Type>
Vector2T<Type> Vector2T<Type>::normalized() const
{
    Vector2T v(*this);
    v.normalize();
    return v;
}

template <typename Type>
Type Vector2T<Type>::dot(Vector2T v) const
{
    return x * v.x + y * v.y;
}

template <typename Type>
Radians Vector2T<Type>::angle_from(Vector2T v) const
{
    double radians = static_cast<double>(std::acos(dot(v)));
    return Radians(radians);
}

template <typename Type>
Type Vector2T<Type>::length() const
{
    return static_cast<Type>(std::sqrt(length_squared()));
}

template <typename Type>
Type Vector2T<Type>::length_squared() const
{
    return dot(*this);
}

template <typename Type>
Vector2T<Type> Vector2T<Type>::floor() const
{
    Vector2T<Type> v;
    v.x = static_cast<Type>(std::floor(x));
    v.y = static_cast<Type>(std::floor(y));
    return v;
}

template <typename Type>
Vector2T<Type> Vector2T<Type>::ceil() const
{
    Vector2T<Type> v;
    v.x = static_cast<Type>(std::ceil(x));
    v.y = static_cast<Type>(std::ceil(y));
    return v;
}

template <typename Type>
Vector2T<Type> Vector2T<Type>::min(Vector2T v) const
{
    Vector2T<Type> m;
    m.x = static_cast<Type>(std::min(v.x, x));
    m.y = static_cast<Type>(std::min(v.y, y));
    return m;
}

template <typename Type>
Vector2T<Type> Vector2T<Type>::max(Vector2T v) const
{
    Vector2T<Type> m;
    m.x = static_cast<Type>(std::max(v.x, x));
    m.y = static_cast<Type>(std::max(v.y, y));
    return m;
}

template <typename Type>
Vector2T<Type> Vector2T<Type>::abs() const
{
    Vector2T<Type> v;
    v.x = static_cast<Type>(std::abs(x));
    v.y = static_cast<Type>(std::abs(y));
    return v;
}

template <typename Type>
Vector2T<Type> Vector2T<Type>::step(Vector2T edge) const
{
    Vector2T<Type> v;
    v.x = x < edge.x ? 0 : 1;
    v.y = y < edge.y ? 0 : 1;
    return v;
}

template <typename Type>
Vector2T<Type> Vector2T<Type>::operator+(Vector2T v) const
{
    return Vector2T(x + v.x, y + v.y);
}

template <typename Type>
Vector2T<Type> Vector2T<Type>::operator-(Vector2T v) const
{
    return Vector2T(x - v.x, y - v.y);
}

template <typename Type>
Vector2T<Type> Vector2T<Type>::operator*(Type value) const
{
    return Vector2T(x * value, y * value);
}

template <typename Type>
Vector2T<Type> Vector2T<Type>::operator*(Vector2T v) const
{
    return Vector2T(x * v.x, y * v.y);
}

template <typename Type>
Vector2T<Type> Vector2T<Type>::operator/(Type value) const
{
    return Vector2T(x / value, y / value);
}

template <typename Type>
Vector2T<Type> Vector2T<Type>::operator/(Vector2T v) const
{
    return Vector2T(x / v.x, y / v.y);
}

template <typename Type>
Vector2T<Type> Vector2T<Type>::operator-() const
{
    return Vector2T(-x, -y);
}

template <typename Type>
Vector2T<Type>& Vector2T<Type>::operator+=(Vector2T v)
{
    x += v.x;
    y += v.y;
    return *this;
}

template <typename Type>
Vector2T<Type>& Vector2T<Type>::operator-=(Vector2T v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

template <typename Type>
Vector2T<Type>& Vector2T<Type>::operator*=(Type value)
{
    x *= value;
    y *= value;
    return *this;
}

template <typename Type>
Vector2T<Type>& Vector2T<Type>::operator*=(Vector2T v)
{
    x *= v.x;
    y *= v.y;
    return *this;
}

template <typename Type>
Vector2T<Type>& Vector2T<Type>::operator/=(Type value)
{
    x /= value;
    y /= value;
    return *this;
}

template <typename Type>
Vector2T<Type>& Vector2T<Type>::operator/=(Vector2T v)
{
    x /= v.x;
    y /= v.y;
    return *this;
}

template <typename Type>
Type& Vector2T<Type>::operator[](size_t i)
{
    assert(i < 2);
    return reinterpret_cast<Type*>(this)[i];
}

template <typename Type>
const Type& Vector2T<Type>::operator[](size_t i) const
{
    assert(i < 2);
    return reinterpret_cast<const Type*>(this)[i];
}

template <typename Type>
bool Vector2T<Type>::operator<(Vector2T v) const
{
    return x < v.x && y < v.y;
}

template <typename Type>
bool Vector2T<Type>::operator==(Vector2T v) const
{
    return x == v.x && y == v.y;
}

template <typename Type>
bool Vector2T<Type>::operator!=(Vector2T v) const
{
    return !(*this == v);
}

template <typename Type>
template <typename U>
Vector2T<Type>::operator Vector2T<U>() const
{
    return Vector2T<U>(static_cast<U>(x), static_cast<U>(y));
}

template <typename Type>
Encoder& operator<<(Encoder& encoder, Vector2T<Type> v)
{
    encoder << begin_array() << v.x << v.y << end_array();
    return encoder;
}

template <typename Type>
Decoder& operator>>(Decoder& decoder, Vector2T<Type>& v)
{
    decoder >> begin_array() >> v.x >> v.y >> end_array();
    return decoder;
}

}
