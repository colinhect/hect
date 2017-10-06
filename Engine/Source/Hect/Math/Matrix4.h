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
#include "Hect/Math/Vector3.h"
#include "Hect/Math/Vector4.h"
#include "Hect/Units/Angle.h"

namespace hect
{

template <typename Type>
class QuaternionT;

///
/// A column-major 4 by 4 matrix.
template <typename Type>
class Matrix4T
{
public:

    ///
    /// Constructs a matrix from a translation.
    ///
    /// \param translation The translation of the resulting matrix.
    ///
    /// \returns The matrix.
    static Matrix4T from_translation(Vector3T<Type> translation);

    ///
    /// Constructs a matrix from a scale.
    ///
    /// \param scale The scale of the resulting matrix.
    ///
    /// \returns The matrix.
    static Matrix4T from_scale(Vector3T<Type> scale);

    ///
    /// Constructs a matrix from a rotation.
    ///
    /// \param rotation The rotation of the resulting matrix.
    ///
    /// \returns The matrix.
    static Matrix4T from_rotation(QuaternionT<Type> rotation);

    ///
    /// Constructs a view matrix.
    ///
    /// \param position The position of the viewer.
    /// \param direction The direction of the viewer.
    /// \param up The up direction of the view.
    ///
    /// \returns The view matrix.
    static Matrix4T create_view(Vector3T<Type> position, Vector3T<Type> direction, Vector3T<Type> up);

    ///
    /// Constructs a perspective projection matrix.
    ///
    /// \param field_of_view The field of view for the projection matrix.
    /// \param aspect_ratio The component ratio for the projection matrix.
    /// \param near_clip The minimum distance from the origin to clip.
    /// \param far_clip The maximum distance from the origin to clip.
    ///
    /// \returns The projection matrix.
    static Matrix4T create_perspective(Radians field_of_view, Type aspect_ratio, Type near_clip, Type far_clip);

    ///
    /// Constructs an orthogonal projection matrix.
    ///
    /// \param left The left bound of the matrix.
    /// \param right The right bound of the matrix.
    /// \param bottom The bottom bound of the matrix.
    /// \param top The top bound of the matrix.
    /// \param near_value The near clip value.
    /// \param far_value The far clip value.
    ///
    /// \returns The projection matrix.
    static Matrix4T create_orthogonal(Type left, Type right, Type bottom, Type top, Type near_value, Type far_value);

    ///
    /// Constructs an identity matrix.
    Matrix4T();

    ///
    /// Translates the matrix.
    ///
    /// \param translation The translation to apply.
    void translate(Vector3T<Type> translation);

    ///
    /// Scales the matrix.
    ///
    /// \param scale The scale to apply.
    void scale(Vector3T<Type> scale);

    ///
    /// Rotates the matrix.
    ///
    /// \param rotation The rotation to apply.
    void rotate(QuaternionT<Type> rotation);

    ///
    /// Returns the given 3-dimensional vector transformed by the matrix.
    ///
    /// \param v The vector to transform.
    Vector3T<Type> operator*(Vector3T<Type> v) const;

    ///
    /// Returns the given 4-dimensional vector transformed by the matrix.
    ///
    /// \param v The vector to transform.
    Vector4T<Type> operator*(Vector4T<Type> v) const;

    ///
    /// Returns the product of the matrix and another matrix.
    ///
    /// \param m The matrix to compute the product with.
    Matrix4T operator*(const Matrix4T& m) const;

    ///
    /// Multiplies the matrix by another matrix.
    ///
    /// \param m The matrix to multiply by.
    ///
    /// \returns A reference to the matrix.
    Matrix4T& operator*=(const Matrix4T& m);

    ///
    /// Returns the ith value of the matrix.
    ///
    /// \param i The index of which component to access.
    Type& operator[](size_t i);

    ///
    /// Returns a constant reference to the ith value of the matrix.
    ///
    /// \param i The index of which component to access.
    const Type& operator[](size_t i) const;

    ///
    /// Returns whether the matrix is equivalent to another.
    ///
    /// \param m The other matrix.
    bool operator==(const Matrix4T& m) const;

    ///
    /// Returns whether the matrix is different from another.
    ///
    /// \param m The other matrix.
    bool operator!=(const Matrix4T& m) const;

    ///
    /// Converts to an equivalent matrix with a different underlying type.
    template <typename U>
    operator Matrix4T<U>() const;

private:
    Type _c[16];
};

typedef Matrix4T<double> Matrix4;

template <typename Type>
Encoder& operator<<(Encoder& encoder, const Matrix4T<Type>& m);

template <typename Type>
Decoder& operator>>(Decoder& decoder, Matrix4T<Type>& m);

}

#include "Matrix4.inl"
