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
#pragma once

namespace hect
{

///
/// A 4-dimensional vector.
template <typename T = double>
class Vector4
{
public:

    ///
    /// Constructs a 4-dimensional vector with all components equal to
    /// zero.
    static Vector4 zero();

    ///
    /// Constructs a 4-dimensional vector with all components equal to one.
    static Vector4 one();

    ///
    /// Constructs a 4-dimensional vector with the x component equal to
    /// one.
    static Vector4 unitX();

    ///
    /// Constructs a 4-dimensional vector with the y component equal to
    /// one.
    static Vector4 unitY();

    ///
    /// Constructs a 4-dimensional vector with the z component equal to
    /// one.
    static Vector4 unitZ();

    ///
    /// Constructs a 4-dimensional vector with the w component equal to
    /// one.
    static Vector4 unitW();

    ///
    /// Constructs a vector with all components equal to zero.
    Vector4();

    ///
    /// Constructs a vector with a value for all components.
    ///
    /// \param value The value for each component.
    Vector4(T value);

    ///
    /// Constructs a vector with a value for each component.
    ///
    /// \param x The value for the x component.
    /// \param y The value for the y component.
    /// \param z The value for the z component.
    /// \param w The value for the w component.
    Vector4(T x, T y, T z, T w);

    ///
    /// \copydoc Vector2::normalize()
    void normalize();

    ///
    /// \copydoc Vector2::normalized()
    Vector4 normalized() const;

    ///
    /// \copydoc Vector2::dot()
    T dot(const Vector4& v) const;

    ///
    /// \copydoc Vector2::angleFrom()
    Angle<T> angleFrom(const Vector4& v) const;

    ///
    /// \copydoc Vector2::length()
    T length() const;

    ///
    /// \copydoc Vector2::lengthSquared()
    T lengthSquared() const;

    ///
    /// Returns the sum of the vector and another vector.
    ///
    /// \param v The vector to compute the sum with.
    Vector4 operator+(const Vector4& v) const;

    ///
    /// Returns the difference of the vector and another vector.
    ///
    /// \param v The vector to compute the difference with.
    Vector4 operator-(const Vector4& v) const;

    ///
    /// Returns the product of the vector and a scalar.
    ///
    /// \param value The scalar to compute the product with.
    Vector4 operator*(T value) const;

    ///
    /// Returns the product of the vector and another vector.
    ///
    /// \param v The vector to compute the product with.
    Vector4 operator*(const Vector4& v) const;

    ///
    /// Returns the quotient of the vector and a scalar.
    ///
    /// \param value The scalar to compute the quotient with.
    Vector4 operator/(T value) const;

    ///
    /// Returns the quotient of the vector and another vector.
    ///
    /// \param v The vector to compute the quotient with.
    Vector4 operator/(const Vector4& v) const;

    ///
    /// Returns a negated copy of the vector.
    Vector4 operator-() const;

    /// Adds a vector to the vector.
    ///
    /// \param v The vector to add.
    ///
    /// \returns A reference to the vector.
    Vector4& operator+=(const Vector4& v);

    /// Subtracts a vector from the vector.
    ///
    /// \param v The vector to subtract.
    ///
    /// \returns A reference to the vector.
    Vector4& operator-=(const Vector4& v);

    /// Multiplies the vector by a scalar.
    ///
    /// \param value The scalar to multiply by.
    ///
    /// \returns A reference to the vector.
    Vector4& operator*=(T value);

    /// Multiplies the vector by another vector.
    ///
    /// \param v The vector to multiply by.
    ///
    /// \returns A reference to the vector.
    Vector4& operator*=(const Vector4& v);

    /// Divides the vector by a scalar.
    ///
    /// \param value The scalar to divide by.
    ///
    /// \returns A reference to the vector.
    Vector4& operator/=(T value);

    /// Divides the vector by another vector.
    ///
    /// \param v The vector to divide by.
    ///
    /// \returns A reference to the vector.
    Vector4& operator/=(const Vector4& v);

    ///
    /// Returns the ith component of the vertex.
    ///
    /// \param i The index of which component to access.
    T& operator[](size_t i);

    ///
    /// Returns a constant reference to the ith component of the vertex.
    ///
    /// \param i The index of which component to access.
    const T& operator[](size_t i) const;

    ///
    /// Converts to an equivalent vector with a different underlying type.
    template <typename U>
    operator Vector4<U>() const
    {
        return Vector4<U>((U)x, (U)y, (U)z, (U)w);
    }

    ///
    /// The x component of the vector.
    T x;

    ///
    /// The y component of the vector.
    T y;

    ///
    /// The z component of the vector.
    T z;

    ///
    /// The w component of the vector.
    T w;
};

}

#include "Vector4.inl"