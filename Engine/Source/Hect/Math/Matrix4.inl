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

template <typename Type>
Matrix4T<Type> Matrix4T<Type>::from_translation(Vector3T<Type> translation)
{
    Matrix4T m;
    m[12] = translation.x;
    m[13] = translation.y;
    m[14] = translation.z;
    return m;
}

template <typename Type>
Matrix4T<Type> Matrix4T<Type>::from_scale(Vector3T<Type> scale)
{
    Matrix4T m;
    m[ 0] *= scale.x;
    m[ 5] *= scale.y;
    m[10] *= scale.z;
    return m;
}

template <typename Type>
Matrix4T<Type> Matrix4T<Type>::from_rotation(QuaternionT<Type> rotation)
{
    Type x = rotation.x;
    Type y = rotation.y;
    Type z = rotation.z;
    Type w = rotation.w;

    Matrix4T m;
    m[ 0] = Type(1) - Type(2) * y * y - Type(2) * z * z;
    m[ 1] = Type(2) * x * y - Type(2) * w * z;
    m[ 2] = Type(2) * x * z + Type(2) * w * y;

    m[ 4] = Type(2) * x * y + Type(2) * w * z;
    m[ 5] = Type(1) - Type(2) * x * x - Type(2) * z * z;
    m[ 6] = Type(2) * y * z - Type(2) * w * x;

    m[ 8] = Type(2) * x * z - Type(2) * w * y;
    m[ 9] = Type(2) * y * z + Type(2) * w * x;
    m[10] = Type(1) - Type(2) * x * x - Type(2) * y * y;
    return m;
}

template <typename Type>
Matrix4T<Type> Matrix4T<Type>::create_view(Vector3T<Type> position, Vector3T<Type> direction, Vector3T<Type> up)
{
    Matrix4T m;

    Vector3T<Type> right = direction.cross(up).normalized();

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

template <typename Type>
Matrix4T<Type> Matrix4T<Type>::create_perspective(Radians field_of_view, Type aspect_ratio, Type near_clip, Type far_clip)
{
    Matrix4T m;

    Type h = Type(1) / std::tan(field_of_view.value * Type(0.5));
    m[ 0] = h / aspect_ratio;
    m[ 5] = h;
    m[10] = (far_clip + near_clip) / (near_clip - far_clip);
    m[11] = -Type(1);
    m[14] = (Type(2) * near_clip * far_clip) / (near_clip - far_clip);
    m[15] = Type(0);

    return m;
}

template <typename Type>
Matrix4T<Type> Matrix4T<Type>::create_orthogonal(Type left, Type right, Type bottom, Type top, Type near_value, Type far_value)
{
    Matrix4T m;

    m[ 0] = Type(2) / (right - left);
    m[ 5] = Type(2) / (top - bottom);
    m[10] = Type(-2) / (far_value - near_value);
    m[12] = -((right + left) / (right - left));
    m[13] = -((top + bottom) / (top - bottom));
    m[14] = -((far_value + near_value) / (far_value - near_value));

    return m;
}

template <typename Type>
Matrix4T<Type>::Matrix4T()
{
    static const Type identity_values[16] =
    {
        Type(1), Type(0), Type(0), Type(0),
        Type(0), Type(1), Type(0), Type(0),
        Type(0), Type(0), Type(1), Type(0),
        Type(0), Type(0), Type(0), Type(1)
    };

    std::memcpy(_c, identity_values, sizeof(Type) * 16);
}

template <typename Type>
void Matrix4T<Type>::translate(Vector3T<Type> translation)
{
    _c[12] += translation.x;
    _c[13] += translation.y;
    _c[14] += translation.z;
}

template <typename Type>
void Matrix4T<Type>::scale(Vector3T<Type> scale)
{
    _c[ 0] *= scale.x;
    _c[ 5] *= scale.y;
    _c[10] *= scale.z;
}

template <typename Type>
void Matrix4T<Type>::rotate(QuaternionT<Type> rotation)
{
    *this *= Matrix4T<Type>::from_rotation(rotation);
}

template <typename Type>
Vector3T<Type> Matrix4T<Type>::operator*(Vector3T<Type> v) const
{
    Vector4T<Type> v4 = *this * Vector4T<Type>(v.x, v.y, v.z, Type(0));
    return Vector3T<Type>(v4.x, v4.y, v4.z);
}

template <typename Type>
Vector4T<Type> Matrix4T<Type>::operator*(Vector4T<Type> v) const
{
    Type x = v.x * _c[0] + v.y * _c[4] + v.z * _c[8] + v.w * _c[12];
    Type y = v.x * _c[1] + v.y * _c[5] + v.z * _c[9] + v.w * _c[13];
    Type z = v.x * _c[2] + v.y * _c[6] + v.z * _c[10] + v.w * _c[14];
    Type w = v.x * _c[3] + v.y * _c[7] + v.z * _c[11] + v.w * _c[15];

    return Vector4T<Type>(x, y, z, w);
}

template <typename Type>
Matrix4T<Type> Matrix4T<Type>::operator*(const Matrix4T& m) const
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

template <typename Type>
Matrix4T<Type>& Matrix4T<Type>::operator*=(const Matrix4T& m)
{
    return *this = *this * m;
}

template <typename Type>
Type& Matrix4T<Type>::operator[](size_t i)
{
    return _c[i];
}

template <typename Type>
const Type& Matrix4T<Type>::operator[](size_t i) const
{
    return _c[i];
}

template <typename Type>
bool Matrix4T<Type>::operator==(const Matrix4T& m) const
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

template <typename Type>
bool Matrix4T<Type>::operator!=(const Matrix4T& m) const
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

template <typename Type>
template <typename U>
Matrix4T<Type>::operator Matrix4T<U>() const
{
    Matrix4T<U> m;
    for (size_t i = 0; i < 16; ++i)
    {
        m[i] = static_cast<U>(_c[i]);
    }
    return m;
}

template <typename Type>
Encoder& operator<<(Encoder& encoder, const Matrix4T<Type>& m)
{
    encoder << begin_array();
    for (size_t i = 0; i < 16; ++i)
    {
        encoder << m[i];
    }
    encoder << end_array();
    return encoder;
}

template <typename Type>
Decoder& operator>>(Decoder& decoder, Matrix4T<Type>& m)
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
