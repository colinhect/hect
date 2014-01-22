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

#include "Hect/Core/Real.h"
#include "Hect/Math/Angle.h"
#include "Hect/Math/Vector3.h"
#include "Hect/Math/Vector4.h"

namespace hect
{

///
/// A rotation in 3-dimensional space.
template <typename T>
class QuaternionT
{
public:

    ///
    /// Constructs a quaternion from an axis and an angle.
    ///
    /// \param axis The axis that the angle is relative to.
    /// \param angle The angle relative to the axis.
    static QuaternionT fromAxisAngle(const Vector3T<T>& axis, Angle angle);

    ///
    /// Constructs an identity quaternion.
    QuaternionT();

    ///
    /// Constructs a quaternion with a value for each component.
    ///
    /// \param x The value of the x component.
    /// \param y The value of the y component.
    /// \param z The value of the z component.
    /// \param w The value of the w component.
    QuaternionT(T x, T y, T z, T w);

    ///
    /// Constructs a quaternion with a vector for the x, y, and z
    /// components and a value for the w component.
    ///
    /// \param v The value for the x, y, and z components.
    /// \param w The value for the w component.
    QuaternionT(const Vector3T<T>& v, T w);

    ///
    /// Constructs a quaternion with a vector for the x, y, z, and w
    /// components.
    ///
    /// \param v The value for the x, y, z, and w components.
    QuaternionT(const Vector4T<T>& v);

    ///
    /// Scales the quaternion to unit length.
    void normalize();

    /// Returns a normalized copy of the quaternion.
    QuaternionT normalized() const;

    ///
    /// Returns the dot product of the quaternion another quaternion.
    ///
    /// \param q The quaternion to compute the dot product with.
    T dot(const QuaternionT& q) const;

    ///
    /// Returns the length of the quaternion.
    T length() const;

    ///
    /// Returns the squared length of the quaternion.
    ///
    /// \remarks Useful for comparing quaternion lengths while avoiding
    /// computation of a square root.
    T lengthSquared() const;

    ///
    /// Returns the conjugate of the quaternion.
    QuaternionT conjugate() const;

    ///
    /// Returns the inverse of the quaternion.
    QuaternionT inverse() const;

    ///
    /// Gets the axis and angle of the quaternion as a rotation.
    ///
    /// \param axis The resulting axis.
    /// \param angle The resulting angle.
    void toAxisAngle(Vector3T<T>& axis, Angle& angle) const;

    ///
    /// Returns the product of the quaternion and a scalar.
    ///
    /// \param value The scalar to compute the product with.
    QuaternionT operator*(T value) const;

    ///
    /// Returns the given vector rotated by the quaternion.
    ///
    /// \param v The vector to rotate.
    Vector3T<T> operator*(const Vector3T<T>& v) const;

    ///
    /// Returns the product of the quaternion and another quaternion.
    ///
    /// \param q The quaternion to compute the product with.
    QuaternionT operator*(const QuaternionT& q) const;

    ///
    /// Returns a negated copy of the quaternion.
    QuaternionT operator-() const;

    /// Multiplies the quaternion by a scalar.
    ///
    /// \param value The scalar to multiply by.
    ///
    /// \returns A reference to the quaternion.
    QuaternionT& operator*=(T value);

    /// Multiplies the quaternion by another quaternion.
    ///
    /// \param q The quaternion to multiply by.
    ///
    /// \returns A reference to the vector.
    QuaternionT& operator*=(const QuaternionT& q);

    ///
    /// Returns the ith component of the quaternion.
    ///
    /// \param i The index of which component to access.
    T& operator[](size_t i);

    ///
    /// Returns a constant reference to the ith component of the
    /// quaternion.
    ///
    /// \param i The index of which component to access.
    const T& operator[](size_t i) const;

    ///
    /// Converts to an equivalent quaternion with a different underlying
    /// type.
    template <typename U>
    operator QuaternionT<U>() const
    {
        return QuaternionT<U>((U)x, (U)y, (U)z, (U)w);
    }

    ///
    /// Converts to a 4-dimensional vector.
    operator Vector4T<T>() const;

    ///
    /// The x component of the quaternion.
    T x;

    ///
    /// The y component of the quaternion.
    T y;

    ///
    /// The z component of the quaternion.
    T z;

    ///
    /// The w component of the quaternion.
    T w;
};

typedef QuaternionT<Real> Quaternion;

}

#include "Quaternion.inl"