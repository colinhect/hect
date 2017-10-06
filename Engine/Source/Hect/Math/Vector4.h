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
#include "Hect/Math/Vector3.h"
#include "Hect/Units/Angle.h"

namespace hect
{

///
/// A 4-dimensional vector.
template <typename Type>
class Vector4T
{
public:

    ///
    /// A 4-dimensional vector with all components equal to zero.
    static const Vector4T Zero;

    ///
    /// A 4-dimensional vector with all components equal to one.
    static const Vector4T One;

    ///
    /// A 4-dimensional vector with the x component equal to one.
    static const Vector4T UnitX;

    ///
    /// A 4-dimensional vector with the y component equal to one.
    static const Vector4T UnitY;

    ///
    /// A 4-dimensional vector with the z component equal to one.
    static const Vector4T UnitZ;

    ///
    /// A 4-dimensional vector with the w component equal to one.
    static const Vector4T UnitW;

    ///
    /// Constructs a vector with all components equal to zero.
    Vector4T();

    ///
    /// Constructs a vector with a value for all components.
    ///
    /// \param value The value for each component.
    Vector4T(Type value);

    ///
    /// Constructs a vector with a value for each component.
    ///
    /// \param x The value for the x component.
    /// \param y The value for the y component.
    /// \param z The value for the z component.
    /// \param w The value for the w component.
    Vector4T(Type x, Type y, Type z, Type w);

    ///
    /// Constructs a vector from a 2-dimensional vector.
    ///
    /// \param v The vector.
    Vector4T(Vector2T<Type> v);

    ///
    /// Constructs a vector from a 3-dimensional vector.
    ///
    /// \param v The vector.
    Vector4T(Vector3T<Type> v);

    ///
    /// \copydoc hect::Vector2T::normalize()
    void normalize();

    ///
    /// \copydoc hect::Vector2T::normalized()
    Vector4T normalized() const;

    ///
    /// \copydoc hect::Vector2T::dot()
    Type dot(Vector4T v) const;

    ///
    /// \copydoc hect::Vector2T::angle_from()
    Radians angle_from(Vector4T v) const;

    ///
    /// \copydoc hect::Vector2T::length()
    Type length() const;

    ///
    /// \copydoc hect::Vector2T::length_squared()
    Type length_squared() const;

    ///
    /// \copydoc hect::Vector2T::floor()
    Vector4T floor() const;

    ///
    /// \copydoc hect::Vector2T::ceil()
    Vector4T ceil() const;

    ///
    /// \copydoc hect::Vector2T::min()
    Vector4T min(Vector4T v) const;

    ///
    /// \copydoc hect::Vector2T::max()
    Vector4T max(Vector4T v) const;

    ///
    /// \copydoc hect::Vector2T::abs()
    Vector4T abs() const;

    ///
    /// \copydoc hect::Vector2T::step()
    Vector4T step(Vector4T edge) const;

    ///
    /// Returns the sum of the vector and another vector.
    ///
    /// \param v The vector to compute the sum with.
    Vector4T operator+(Vector4T v) const;

    ///
    /// Returns the difference of the vector and another vector.
    ///
    /// \param v The vector to compute the difference with.
    Vector4T operator-(Vector4T v) const;

    ///
    /// Returns the product of the vector and a scalar.
    ///
    /// \param value The scalar to compute the product with.
    Vector4T operator*(Type value) const;

    ///
    /// Returns the product of the vector and another vector.
    ///
    /// \param v The vector to compute the product with.
    Vector4T operator*(Vector4T v) const;

    ///
    /// Returns the quotient of the vector and a scalar.
    ///
    /// \param value The scalar to compute the quotient with.
    Vector4T operator/(Type value) const;

    ///
    /// Returns the quotient of the vector and another vector.
    ///
    /// \param v The vector to compute the quotient with.
    Vector4T operator/(Vector4T v) const;

    ///
    /// Returns a negated copy of the vector.
    Vector4T operator-() const;

    ///
    /// Adds a vector to the vector.
    ///
    /// \param v The vector to add.
    ///
    /// \returns A reference to the vector.
    Vector4T& operator+=(Vector4T v);

    ///
    /// Subtracts a vector from the vector.
    ///
    /// \param v The vector to subtract.
    ///
    /// \returns A reference to the vector.
    Vector4T& operator-=(Vector4T v);

    ///
    /// Multiplies the vector by a scalar.
    ///
    /// \param value The scalar to multiply by.
    ///
    /// \returns A reference to the vector.
    Vector4T& operator*=(Type value);

    ///
    /// Multiplies the vector by another vector.
    ///
    /// \param v The vector to multiply by.
    ///
    /// \returns A reference to the vector.
    Vector4T& operator*=(Vector4T v);

    ///
    /// Divides the vector by a scalar.
    ///
    /// \param value The scalar to divide by.
    ///
    /// \returns A reference to the vector.
    Vector4T& operator/=(Type value);

    ///
    /// Divides the vector by another vector.
    ///
    /// \param v The vector to divide by.
    ///
    /// \returns A reference to the vector.
    Vector4T& operator/=(Vector4T v);

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
    bool operator<(Vector4T v) const;

    ///
    /// Returns whether the vector is equivalent to another.
    ///
    /// \param v The other vector.
    bool operator==(Vector4T v) const;

    ///
    /// Returns whether the vector is different from another.
    ///
    /// \param v The other vector.
    bool operator!=(Vector4T v) const;

    ///
    /// Converts to an equivalent vector with a different underlying type.
    template <typename U>
    operator Vector4T<U>() const;

    ///
    /// The x component of the vector.
    Type x { 0 };

    ///
    /// The y component of the vector.
    Type y { 0 };

    ///
    /// The z component of the vector.
    Type z { 0 };

    ///
    /// The w component of the vector.
    Type w { 0 };
};

///
/// A 4-dimensional vector of doubles.
typedef Vector4T<double> Vector4;

///
/// A 4-dimensional vector of integers.
typedef Vector4T<int> IntVector4;

template <typename Type>
Encoder& operator<<(Encoder& encoder, Vector4T<Type> v);

template <typename Type>
Decoder& operator>>(Decoder& decoder, Vector4T<Type>& v);

}

#include "Vector4.inl"
