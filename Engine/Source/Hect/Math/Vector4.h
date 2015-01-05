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
#pragma once

#include "Hect/Core/Real.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"
#include "Hect/Math/Angle.h"

namespace hect
{

///
/// A 4-dimensional vector.
template <typename T>
class Vector4T
{
public:

    ///
    /// Constructs a 4-dimensional vector with all components equal to
    /// zero.
    static Vector4T zero();

    ///
    /// Constructs a 4-dimensional vector with all components equal to one.
    static Vector4T one();

    ///
    /// Constructs a 4-dimensional vector with the x component equal to
    /// one.
    static Vector4T unitX();

    ///
    /// Constructs a 4-dimensional vector with the y component equal to
    /// one.
    static Vector4T unitY();

    ///
    /// Constructs a 4-dimensional vector with the z component equal to
    /// one.
    static Vector4T unitZ();

    ///
    /// Constructs a 4-dimensional vector with the w component equal to
    /// one.
    static Vector4T unitW();

    ///
    /// Constructs a vector with all components equal to zero.
    Vector4T();

    ///
    /// Constructs a vector with a value for all components.
    ///
    /// \param value The value for each component.
    Vector4T(T value);

    ///
    /// Constructs a vector with a value for each component.
    ///
    /// \param x The value for the x component.
    /// \param y The value for the y component.
    /// \param z The value for the z component.
    /// \param w The value for the w component.
    Vector4T(T x, T y, T z, T w);

    ///
    /// \copydoc Vector2T::normalize()
    void normalize();

    ///
    /// \copydoc Vector2T::normalized()
    Vector4T normalized() const;

    ///
    /// \copydoc Vector2T::dot()
    T dot(const Vector4T& v) const;

    ///
    /// \copydoc Vector2T::angleFrom()
    Angle angleFrom(const Vector4T& v) const;

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
    Vector4T operator+(const Vector4T& v) const;

    ///
    /// Returns the difference of the vector and another vector.
    ///
    /// \param v The vector to compute the difference with.
    Vector4T operator-(const Vector4T& v) const;

    ///
    /// Returns the product of the vector and a scalar.
    ///
    /// \param value The scalar to compute the product with.
    Vector4T operator*(T value) const;

    ///
    /// Returns the product of the vector and another vector.
    ///
    /// \param v The vector to compute the product with.
    Vector4T operator*(const Vector4T& v) const;

    ///
    /// Returns the quotient of the vector and a scalar.
    ///
    /// \param value The scalar to compute the quotient with.
    Vector4T operator/(T value) const;

    ///
    /// Returns the quotient of the vector and another vector.
    ///
    /// \param v The vector to compute the quotient with.
    Vector4T operator/(const Vector4T& v) const;

    ///
    /// Returns a negated copy of the vector.
    Vector4T operator-() const;

    /// Adds a vector to the vector.
    ///
    /// \param v The vector to add.
    ///
    /// \returns A reference to the vector.
    Vector4T& operator+=(const Vector4T& v);

    /// Subtracts a vector from the vector.
    ///
    /// \param v The vector to subtract.
    ///
    /// \returns A reference to the vector.
    Vector4T& operator-=(const Vector4T& v);

    /// Multiplies the vector by a scalar.
    ///
    /// \param value The scalar to multiply by.
    ///
    /// \returns A reference to the vector.
    Vector4T& operator*=(T value);

    /// Multiplies the vector by another vector.
    ///
    /// \param v The vector to multiply by.
    ///
    /// \returns A reference to the vector.
    Vector4T& operator*=(const Vector4T& v);

    /// Divides the vector by a scalar.
    ///
    /// \param value The scalar to divide by.
    ///
    /// \returns A reference to the vector.
    Vector4T& operator/=(T value);

    /// Divides the vector by another vector.
    ///
    /// \param v The vector to divide by.
    ///
    /// \returns A reference to the vector.
    Vector4T& operator/=(const Vector4T& v);

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
    /// Returns whether the vector is less than another.
    ///
    /// \param v The other vector.
    bool operator<(const Vector4T& v) const;

    ///
    /// Returns whether the vector is equivalent to another.
    ///
    /// \param v The other vector.
    bool operator==(const Vector4T& v) const;

    ///
    /// Returns whether the vector is different from another.
    ///
    /// \param v The other vector.
    bool operator!=(const Vector4T& v) const;

    ///
    /// Converts to an equivalent vector with a different underlying type.
    template <typename U>
    operator Vector4T<U>() const;

    ///
    /// The x component of the vector.
    T x { 0 };

    ///
    /// The y component of the vector.
    T y { 0 };

    ///
    /// The z component of the vector.
    T z { 0 };

    ///
    /// The w component of the vector.
    T w { 0 };
};

///
/// A 4-dimensional vector of real numbers.
typedef Vector4T<Real> Vector4;

///
/// A 4-dimensional vector of integers.
typedef Vector4T<int> IntVector4;

template <typename T>
Encoder& operator<<(Encoder& encoder, const Vector4T<T>& v);

template <typename T>
Decoder& operator>>(Decoder& decoder, Vector4T<T>& v);

}

#include "Vector4.inl"