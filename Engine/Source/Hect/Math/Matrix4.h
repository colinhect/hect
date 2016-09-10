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
#include "Hect/Math/Angle.h"
#include "Hect/Math/Vector3.h"
#include "Hect/Math/Vector4.h"

namespace hect
{

template <typename T>
class QuaternionT;

///
/// A column-major 4 by 4 matrix.
template <typename T>
class Matrix4T
{
public:

    ///
    /// Constructs a matrix from a translation.
    ///
    /// \param translation The translation of the resulting matrix.
    ///
    /// \returns The matrix.
    static Matrix4T fromTranslation(Vector3T<T> translation);

    ///
    /// Constructs a matrix from a scale.
    ///
    /// \param scale The scale of the resulting matrix.
    ///
    /// \returns The matrix.
    static Matrix4T fromScale(Vector3T<T> scale);

    ///
    /// Constructs a matrix from a rotation.
    ///
    /// \param rotation The rotation of the resulting matrix.
    ///
    /// \returns The matrix.
    static Matrix4T fromRotation(QuaternionT<T> rotation);

    ///
    /// Constructs a view matrix.
    ///
    /// \param position The position of the viewer.
    /// \param direction The direction of the viewer.
    /// \param up The up direction of the view.
    ///
    /// \returns The view matrix.
    static Matrix4T createView(Vector3T<T> position, Vector3T<T> direction, Vector3T<T> up);

    ///
    /// Constructs a perspective projection matrix.
    ///
    /// \param fieldOfView The field of view for the projection matrix.
    /// \param aspectRatio The component ratio for the projection matrix.
    /// \param nearClip The minimum distance from the origin to clip.
    /// \param farClip The maximum distance from the origin to clip.
    ///
    /// \returns The projection matrix.
    static Matrix4T createPerspective(Angle fieldOfView, T aspectRatio, T nearClip, T farClip);

    ///
    /// Constructs an orthogonal projection matrix.
    ///
    /// \param left The left bound of the matrix.
    /// \param right The right bound of the matrix.
    /// \param bottom The bottom bound of the matrix.
    /// \param top The top bound of the matrix.
    /// \param nearValue The near clip value.
    /// \param farValue The far clip value.
    ///
    /// \returns The projection matrix.
    static Matrix4T createOrthogonal(T left, T right, T bottom, T top, T nearValue, T farValue);

    ///
    /// Constructs an identity matrix.
    Matrix4T();

    ///
    /// Translates the matrix.
    ///
    /// \param translation The translation to apply.
    void translate(Vector3T<T> translation);

    ///
    /// Scales the matrix.
    ///
    /// \param scale The scale to apply.
    void scale(Vector3T<T> scale);

    ///
    /// Rotates the matrix.
    ///
    /// \param rotation The rotation to apply.
    void rotate(QuaternionT<T> rotation);

    ///
    /// Returns the given 3-dimensional vector transformed by the matrix.
    ///
    /// \param v The vector to transform.
    Vector3T<T> operator*(Vector3T<T> v) const;

    ///
    /// Returns the given 4-dimensional vector transformed by the matrix.
    ///
    /// \param v The vector to transform.
    Vector4T<T> operator*(Vector4T<T> v) const;

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
    T& operator[](size_t i);

    ///
    /// Returns a constant reference to the ith value of the matrix.
    ///
    /// \param i The index of which component to access.
    const T& operator[](size_t i) const;

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
    T _c[16];
};

typedef Matrix4T<double> Matrix4;

template <typename T>
Encoder& operator<<(Encoder& encoder, const Matrix4T<T>& m);

template <typename T>
Decoder& operator>>(Decoder& decoder, Matrix4T<T>& m);

}

#include "Matrix4.inl"
