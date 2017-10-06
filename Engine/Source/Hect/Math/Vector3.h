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
#pragma once

#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"
#include "Hect/Math/Vector2.h"
#include "Hect/Units/Angle.h"

namespace hect
{

///
/// A 3-dimensional vector.
template <typename Type>
class Vector3T
{
public:

    ///
    /// A 3-dimensional vector with all components equal to zero.
    static const Vector3T Zero;

    ///
    /// A 3-dimensional vector with all components equal to one.
    static const Vector3T One;

    ///
    /// A 3-dimensional vector with the x component equal to one.
    static const Vector3T UnitX;

    ///
    /// A 3-dimensional vector with the y component equal to one.
    static const Vector3T UnitY;

    ///
    /// A 3-dimensional vector with the z component equal to one.
    static const Vector3T UnitZ;

    ///
    /// Constructs a vector with all components equal to zero.
    Vector3T();

    ///
    /// Constructs a vector with a value for all components.
    ///
    /// \param value The value for each component.
    Vector3T(Type value);

    ///
    /// Constructs a vector with a value for each component.
    ///
    /// \param x The value for the x component.
    /// \param y The value for the y component.
    /// \param z The value for the z component.
    Vector3T(Type x, Type y, Type z);

    ///
    /// Constructs a vector from a 2-dimensional vector.
    ///
    /// \param v The vector.
    Vector3T(Vector2T<Type> v);

    ///
    /// \copydoc hect::Vector2T::normalize()
    void normalize();

    ///
    /// \copydoc hect::Vector2T::normalized()
    Vector3T normalized() const;

    ///
    /// \copydoc hect::Vector2T::dot()
    Type dot(Vector3T v) const;

    ///
    /// Returns the cross product of the vector and another vector.
    ///
    /// \param v The vector to compute the cross product with.
    Vector3T cross(Vector3T v) const;

    ///
    /// \copydoc hect::Vector2T::angle_from()
    Radians angle_from(Vector3T v) const;

    ///
    /// \copydoc hect::Vector2T::length()
    Type length() const;

    ///
    /// \copydoc hect::Vector2T::length_squared()
    Type length_squared() const;

    ///
    /// \copydoc hect::Vector2T::floor()
    Vector3T floor() const;

    ///
    /// \copydoc hect::Vector2T::ceil()
    Vector3T ceil() const;

    ///
    /// \copydoc hect::Vector2T::min()
    Vector3T min(Vector3T v) const;

    ///
    /// \copydoc hect::Vector2T::max()
    Vector3T max(Vector3T v) const;

    ///
    /// \copydoc hect::Vector2T::abs()
    Vector3T abs() const;

    ///
    /// \copydoc hect::Vector2T::step()
    Vector3T step(Vector3T edge) const;

    ///
    /// Returns the sum of the vector and another vector.
    ///
    /// \param v The vector to compute the sum with.
    Vector3T operator+(Vector3T v) const;

    ///
    /// Returns the difference of the vector and another vector.
    ///
    /// \param v The vector to compute the difference with.
    Vector3T operator-(Vector3T v) const;

    ///
    /// Returns the product of the vector and a scalar.
    ///
    /// \param value The scalar to compute the product with.
    Vector3T operator*(Type value) const;

    ///
    /// Returns the product of the vector and another vector.
    ///
    /// \param v The vector to compute the product with.
    Vector3T operator*(Vector3T v) const;

    ///
    /// Returns the quotient of the vector and a scalar.
    ///
    /// \param value The scalar to compute the quotient with.
    Vector3T operator/(Type value) const;

    ///
    /// Returns the quotient of the vector and another vector.
    ///
    /// \param v The vector to compute the quotient with.
    Vector3T operator/(Vector3T v) const;

    ///
    /// Returns a negated copy of the vector.
    Vector3T operator-() const;

    ///
    /// Adds a vector to the vector.
    ///
    /// \param v The vector to add.
    ///
    /// \returns A reference to the vector.
    Vector3T& operator+=(Vector3T v);

    ///
    /// Subtracts a vector from the vector.
    ///
    /// \param v The vector to subtract.
    ///
    /// \returns A reference to the vector.
    Vector3T& operator-=(Vector3T v);

    ///
    /// Multiplies the vector by a scalar.
    ///
    /// \param value The scalar to multiply by.
    ///
    /// \returns A reference to the vector.
    Vector3T& operator*=(Type value);

    ///
    /// Multiplies the vector by another vector.
    ///
    /// \param v The vector to multiply by.
    ///
    /// \returns A reference to the vector.
    Vector3T& operator*=(Vector3T v);

    ///
    /// Divides the vector by a scalar.
    ///
    /// \param value The scalar to divide by.
    ///
    /// \returns A reference to the vector.
    Vector3T& operator/=(Type value);

    ///
    /// Divides the vector by another vector.
    ///
    /// \param v The vector to divide by.
    ///
    /// \returns A reference to the vector.
    Vector3T& operator/=(Vector3T v);

    ///
    /// Returns the ith component of the vertex.
    ///
    /// \param i The index of which component to access.
    Type& operator[](size_t i);

    ///
    /// Returns a constant reference to the ith component of the vertex.
    ///
    /// \param i The index of which component to access.
    const Type& operator[](size_t i) const;

    ///
    /// Returns whether the vector is less than another.
    ///
    /// \param v The other vector.
    bool operator<(Vector3T v) const;

    ///
    /// Returns whether the vector is equivalent to another.
    ///
    /// \param v The other vector.
    bool operator==(Vector3T v) const;

    ///
    /// Returns whether the vector is different from another.
    ///
    /// \param v The other vector.
    bool operator!=(Vector3T v) const;

    ///
    /// Converts to an equivalent vector with a different underlying type.
    template <typename U>
    operator Vector3T<U>() const;

    ///
    /// The x component of the vector.
    Type x { 0 };

    ///
    /// The y component of the vector.
    Type y { 0 };

    ///
    /// The z component of the vector.
    Type z { 0 };
};

///
/// A 3-dimensional vector of doubles.
typedef Vector3T<double> Vector3;

///
/// A 3-dimensional vector of integers.
typedef Vector3T<int> IntVector3;

template <typename Type>
Encoder& operator<<(Encoder& encoder, Vector3T<Type> v);

template <typename Type>
Decoder& operator>>(Decoder& decoder, Vector3T<Type>& v);

}

#include "Vector3.inl"
