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

#include <cmath>

#include "Core/Real.h"
#include "Math/Angle.h"

namespace hect
{

///
/// A 3-dimensional vector.
template <typename T>
class Vector3T
{
public:

    ///
    /// Constructs a 3-dimensional vector with all components equal to
    /// zero.
    static Vector3T zero();

    ///
    /// Constructs a 3-dimensional vector with all components equal to one.
    static Vector3T one();

    ///
    /// Constructs a 3-dimensional vector with the x component equal to
    /// one.
    static Vector3T unitX();

    ///
    /// Constructs a 3-dimensional vector with the y component equal to
    /// one.
    static Vector3T unitY();

    ///
    /// Constructs a 3-dimensional vector with the z component equal to
    /// one.
    static Vector3T unitZ();

    ///
    /// Constructs a vector with all components equal to zero.
    Vector3T();

    ///
    /// Constructs a vector with a value for all components.
    ///
    /// \param value The value for each component.
    Vector3T(T value);

    ///
    /// Constructs a vector with a value for each component.
    ///
    /// \param x The value for the x component.
    /// \param y The value for the y component.
    /// \param z The value for the z component.
    Vector3T(T x, T y, T z);

    ///
    /// \copydoc Vector2T::normalize()
    void normalize();

    ///
    /// \copydoc Vector2T::normalized()
    Vector3T normalized() const;

    ///
    /// \copydoc Vector2T::dot()
    T dot(const Vector3T& v) const;

    ///
    /// Returns the cross product of the vector and another vector.
    ///
    /// \param v The vector to compute the cross product with.
    Vector3T cross(const Vector3T& v) const;

    ///
    /// \copydoc Vector2T::angleFrom()
    Angle angleFrom(const Vector3T& v) const;

    ///
    /// \copydoc Vector2T::length()
    T length() const;

    ///
    /// \copydoc Vector2T::lengthSquared()
    T lengthSquared() const;

    ///
    /// Returns the sum of the vector and another vector.
    ///
    /// \param v The vector to compute the sum with.
    Vector3T operator+(const Vector3T& v) const;

    ///
    /// Returns the difference of the vector and another vector.
    ///
    /// \param v The vector to compute the difference with.
    Vector3T operator-(const Vector3T& v) const;

    ///
    /// Returns the product of the vector and a scalar.
    ///
    /// \param value The scalar to compute the product with.
    Vector3T operator*(T value) const;

    ///
    /// Returns the product of the vector and another vector.
    ///
    /// \param v The vector to compute the product with.
    Vector3T operator*(const Vector3T& v) const;

    ///
    /// Returns the quotient of the vector and a scalar.
    ///
    /// \param value The scalar to compute the quotient with.
    Vector3T operator/(T value) const;

    ///
    /// Returns the quotient of the vector and another vector.
    ///
    /// \param v The vector to compute the quotient with.
    Vector3T operator/(const Vector3T& v) const;

    ///
    /// Returns a negated copy of the vector.
    Vector3T operator-() const;

    /// Adds a vector to the vector.
    ///
    /// \param v The vector to add.
    ///
    /// \returns A reference to the vector.
    Vector3T& operator+=(const Vector3T& v);

    /// Subtracts a vector from the vector.
    ///
    /// \param v The vector to subtract.
    ///
    /// \returns A reference to the vector.
    Vector3T& operator-=(const Vector3T& v);

    /// Multiplies the vector by a scalar.
    ///
    /// \param value The scalar to multiply by.
    ///
    /// \returns A reference to the vector.
    Vector3T& operator*=(T value);

    /// Multiplies the vector by another vector.
    ///
    /// \param v The vector to multiply by.
    ///
    /// \returns A reference to the vector.
    Vector3T& operator*=(const Vector3T& v);

    /// Divides the vector by a scalar.
    ///
    /// \param value The scalar to divide by.
    ///
    /// \returns A reference to the vector.
    Vector3T& operator/=(T value);

    /// Divides the vector by another vector.
    ///
    /// \param v The vector to divide by.
    ///
    /// \returns A reference to the vector.
    Vector3T& operator/=(const Vector3T& v);

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
    operator Vector3T<U>() const
    {
        return Vector3T<U>((U)x, (U)y, (U)z);
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
};

typedef Vector3T<Real> Vector3;
typedef Vector3T<int> IntVector3;

}

#include "Vector3.inl"