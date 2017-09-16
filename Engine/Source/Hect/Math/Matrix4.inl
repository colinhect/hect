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
#include <cassert>
#include <cstring>

#include "Hect/Math/Quaternion.h"

namespace hect
{

template <typename T>
Matrix4T<T> Matrix4T<T>::from_translation(Vector3T<T> translation)
{
    Matrix4T m;
    m[12] = translation.x;
    m[13] = translation.y;
    m[14] = translation.z;
    return m;
}

template <typename T>
Matrix4T<T> Matrix4T<T>::from_scale(Vector3T<T> scale)
{
    Matrix4T m;
    m[ 0] *= scale.x;
    m[ 5] *= scale.y;
    m[10] *= scale.z;
    return m;
}

template <typename T>
Matrix4T<T> Matrix4T<T>::from_rotation(QuaternionT<T> rotation)
{
    T x = rotation.x;
    T y = rotation.y;
    T z = rotation.z;
    T w = rotation.w;

    Matrix4T m;
    m[ 0] = T(1) - T(2) * y * y - T(2) * z * z;
    m[ 1] = T(2) * x * y - T(2) * w * z;
    m[ 2] = T(2) * x * z + T(2) * w * y;

    m[ 4] = T(2) * x * y + T(2) * w * z;
    m[ 5] = T(1) - T(2) * x * x - T(2) * z * z;
    m[ 6] = T(2) * y * z - T(2) * w * x;

    m[ 8] = T(2) * x * z - T(2) * w * y;
    m[ 9] = T(2) * y * z + T(2) * w * x;
    m[10] = T(1) - T(2) * x * x - T(2) * y * y;
    return m;
}

template <typename T>
Matrix4T<T> Matrix4T<T>::create_view(Vector3T<T> position, Vector3T<T> direction, Vector3T<T> up)
{
    Matrix4T m;

    Vector3T<T> right = direction.cross(up).normalized();

    m[ 0] = right.x;
    m[ 1] = up.x;
    m[ 2] = -direction.x;
    m[ 4] = right.y;
    m[ 5] = up.y;
    m[ 6] = -direction.y;
    m[ 8] = right.z;
    m[ 9] = up.z;
    m[10] = -direction.z;

    Matrix4T position_matrix;
    position_matrix.translate(-position);

    return m * position_matrix;
}

template <typename T>
Matrix4T<T> Matrix4T<T>::create_perspective(Radians field_of_view, T aspect_ratio, T near_clip, T far_clip)
{
    Matrix4T m;

    T h = T(1) / std::tan(field_of_view.value * T(0.5));
    m[ 0] = h / aspect_ratio;
    m[ 5] = h;
    m[10] = (far_clip + near_clip) / (near_clip - far_clip);
    m[11] = -T(1);
    m[14] = (T(2) * near_clip * far_clip) / (near_clip - far_clip);
    m[15] = T(0);

    return m;
}

template <typename T>
Matrix4T<T> Matrix4T<T>::create_orthogonal(T left, T right, T bottom, T top, T near_value, T far_value)
{
    Matrix4T m;

    m[ 0] = T(2) / (right - left);
    m[ 5] = T(2) / (top - bottom);
    m[10] = T(-2) / (far_value - near_value);
    m[12] = -((right + left) / (right - left));
    m[13] = -((top + bottom) / (top - bottom));
    m[14] = -((far_value + near_value) / (far_value - near_value));

    return m;
}

template <typename T>
Matrix4T<T>::Matrix4T()
{
    static const T identity_values[16] =
    {
        T(1), T(0), T(0), T(0),
        T(0), T(1), T(0), T(0),
        T(0), T(0), T(1), T(0),
        T(0), T(0), T(0), T(1)
    };

    std::memcpy(_c, identity_values, sizeof(T) * 16);
}

template <typename T>
void Matrix4T<T>::translate(Vector3T<T> translation)
{
    _c[12] += translation.x;
    _c[13] += translation.y;
    _c[14] += translation.z;
}

template <typename T>
void Matrix4T<T>::scale(Vector3T<T> scale)
{
    _c[ 0] *= scale.x;
    _c[ 5] *= scale.y;
    _c[10] *= scale.z;
}

template <typename T>
void Matrix4T<T>::rotate(QuaternionT<T> rotation)
{
    *this *= Matrix4T<T>::from_rotation(rotation);
}

template <typename T>
Vector3T<T> Matrix4T<T>::operator*(Vector3T<T> v) const
{
    Vector4T<T> v4 = *this * Vector4T<T>(v.x, v.y, v.z, T(0));
    return Vector3T<T>(v4.x, v4.y, v4.z);
}

template <typename T>
Vector4T<T> Matrix4T<T>::operator*(Vector4T<T> v) const
{
    T x = v.x * _c[0] + v.y * _c[4] + v.z * _c[8] + v.w * _c[12];
    T y = v.x * _c[1] + v.y * _c[5] + v.z * _c[9] + v.w * _c[13];
    T z = v.x * _c[2] + v.y * _c[6] + v.z * _c[10] + v.w * _c[14];
    T w = v.x * _c[3] + v.y * _c[7] + v.z * _c[11] + v.w * _c[15];

    return Vector4T<T>(x, y, z, w);
}

template <typename T>
Matrix4T<T> Matrix4T<T>::operator*(const Matrix4T& m) const
{
    Matrix4T result;
    result[0]  = _c[0] * m[0]  + _c[4] * m[1] + _c[8]   * m[2]  + _c[12] * m[3];
    result[1]  = _c[1] * m[0]  + _c[5] * m[1] + _c[9]   * m[2]  + _c[13] * m[3];
    result[2]  = _c[2] * m[0]  + _c[6] * m[1] + _c[10]  * m[2]  + _c[14] * m[3];
    result[3]  = _c[3] * m[0]  + _c[7] * m[1] + _c[11]  * m[2]  + _c[15] * m[3];
    result[4]  = _c[0] * m[4]  + _c[4] * m[5] + _c[8]   * m[6]  + _c[12] * m[7];
    result[5]  = _c[1] * m[4]  + _c[5] * m[5] + _c[9]   * m[6]  + _c[13] * m[7];
    result[6]  = _c[2] * m[4]  + _c[6] * m[5] + _c[10]  * m[6]  + _c[14] * m[7];
    result[7]  = _c[3] * m[4]  + _c[7] * m[5] + _c[11]  * m[6]  + _c[15] * m[7];
    result[8]  = _c[0] * m[8]  + _c[4] * m[9] + _c[8]   * m[10] + _c[12] * m[11];
    result[9]  = _c[1] * m[8]  + _c[5] * m[9] + _c[9]   * m[10] + _c[13] * m[11];
    result[10] = _c[2] * m[8]  + _c[6] * m[9] + _c[10]  * m[10] + _c[14] * m[11];
    result[11] = _c[3] * m[8]  + _c[7] * m[9] + _c[11]  * m[10] + _c[15] * m[11];
    result[12] = _c[0] * m[12] + _c[4] * m[13] + _c[8]  * m[14] + _c[12] * m[15];
    result[13] = _c[1] * m[12] + _c[5] * m[13] + _c[9]  * m[14] + _c[13] * m[15];
    result[14] = _c[2] * m[12] + _c[6] * m[13] + _c[10] * m[14] + _c[14] * m[15];
    result[15] = _c[3] * m[12] + _c[7] * m[13] + _c[11] * m[14] + _c[15] * m[15];
    return result;
}

template <typename T>
Matrix4T<T>& Matrix4T<T>::operator*=(const Matrix4T& m)
{
    return *this = *this * m;
}

template <typename T>
T& Matrix4T<T>::operator[](size_t i)
{
    return _c[i];
}

template <typename T>
const T& Matrix4T<T>::operator[](size_t i) const
{
    return _c[i];
}

template <typename T>
bool Matrix4T<T>::operator==(const Matrix4T& m) const
{
    for (size_t i = 0; i < 16; ++i)
    {
        if (_c[i] != m._c[i])
        {
            return false;
        }
    }

    return true;
}

template <typename T>
bool Matrix4T<T>::operator!=(const Matrix4T& m) const
{
    for (size_t i = 0; i < 16; ++i)
    {
        if (_c[i] == m._c[i])
        {
            return false;
        }
    }

    return true;
}

template <typename T>
template <typename U>
Matrix4T<T>::operator Matrix4T<U>() const
{
    Matrix4T<U> m;
    for (size_t i = 0; i < 16; ++i)
    {
        m[i] = static_cast<U>(_c[i]);
    }
    return m;
}

template <typename T>
Encoder& operator<<(Encoder& encoder, const Matrix4T<T>& m)
{
    encoder << begin_array();
    for (size_t i = 0; i < 16; ++i)
    {
        encoder << m[i];
    }
    encoder << end_array();
    return encoder;
}

template <typename T>
Decoder& operator>>(Decoder& decoder, Matrix4T<T>& m)
{
    decoder >> begin_array();
    for (size_t i = 0; i < 16; ++i)
    {
        decoder >> m[i];
    }
    decoder >> end_array();
    return decoder;
}

}
