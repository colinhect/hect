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
/// A column-major 4 by 4 matrix.
template <typename T = double>
class Matrix4
{
public:

    ///
    /// Constructs a matrix from a translation.
    ///
    /// \param translation The translation of the resulting matrix.
    ///
    /// \returns The matrix.
    static Matrix4 fromTranslation(const Vector3<T>& translation);

    ///
    /// Constructs a matrix from a scale.
    ///
    /// \param scale The scale of the resulting matrix.
    ///
    /// \returns The matrix.
    static Matrix4 fromScale(const Vector3<T>& scale);

    ///
    /// Constructs a matrix from a rotation.
    ///
    /// \param rotation The rotation of the resulting matrix.
    ///
    /// \returns The matrix.
    static Matrix4 fromRotation(const Quaternion<T>& rotation);

    ///
    /// Constructs a view matrix.
    ///
    /// \param position The position of the viewer.
    /// \param direction The direction of the viewer.
    /// \param up The up direction of the view.
    ///
    /// \returns The view matrix.
    static Matrix4 createView(const Vector3<T>& position, const Vector3<T>& direction, const Vector3<T>& up);

    ///
    /// Constructs a perspective projection matrix.
    ///
    /// \param fieldOfView The field of view for the projection matrix.
    /// \param aspectRatio The component ratio for the projection matrix.
    /// \param nearClip The minimum distance from the origin to clip.
    /// \param farClip The maximum distance from the origin to clip.
    ///
    /// \returns The projection matrix.
    static Matrix4 createPerspective(Angle<T> fieldOfView, T aspectRatio, T nearClip, T farClip);

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
    static Matrix4 createOrthogonal(T left, T right, T bottom, T top, T nearValue, T farValue);

    ///
    /// Constructs an identity matrix.
    Matrix4();

    ///
    /// Translates the matrix.
    ///
    /// \param translation The translation to apply.
    void translate(const Vector3<T>& translation);

    ///
    /// Scales the matrix.
    ///
    /// \param scale The scale to apply.
    void scale(const Vector3<T>& scale);

    ///
    /// Rotates the matrix.
    ///
    /// \param rotation The rotation to apply.
    void rotate(const Quaternion<T>& rotation);

    ///
    /// Returns the given 3-dimensional vector transformed by the matrix.
    ///
    /// \param v The vector to transform.
    Vector3<T> operator*(const Vector3<T>& v) const;

    ///
    /// Returns the given 4-dimensional vector transformed by the matrix.
    ///
    /// \param v The vector to transform.
    Vector4<T> operator*(const Vector4<T>& v) const;

    ///
    /// Returns the product of the matrix and another matrix.
    ///
    /// \param m The matrix to compute the product with.
    Matrix4 operator*(const Matrix4& m) const;

    ///
    /// Multiplies the matrix by another matrix.
    ///
    /// \param m The matrix to multiply by.
    ///
    /// \returns A reference to the matrix.
    Matrix4& operator*=(const Matrix4& m);

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
    /// Converts to an equivalent matrix with a different underlying type.
    template <typename U>
    operator Matrix4<U>() const
    {
        Matrix4<U> m;
        for (size_t i = 0; i < 16; ++i)
        {
            m[i] = (U)_c[i];
        }
        return m;
    }

private:
    T _c[16];
};

}

#include "Matrix4.inl"