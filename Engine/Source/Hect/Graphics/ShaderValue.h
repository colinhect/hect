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

#include "Hect/Core/Any.h"
#include "Hect/IO/AssetCache.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"
#include "Hect/Math/Vector2.h"
#include "Hect/Math/Vector3.h"
#include "Hect/Math/Vector4.h"
#include "Hect/Math/Matrix4.h"

namespace hect
{

class Texture;

///
/// A shader value type.
enum ShaderValueType
{
    ///
    /// An integer.
    ShaderValueType_Int,

    ///
    /// A floating point number.
    ShaderValueType_Float,

    ///
    /// A 2-dimensional vector.
    ShaderValueType_Vector2,

    ///
    /// A 3-dimensional vector.
    ShaderValueType_Vector3,

    ///
    /// A 4-dimensional vector.
    ShaderValueType_Vector4,

    ///
    /// A 4 by 4 matrix.
    ShaderValueType_Matrix4,

    ///
    /// A texture.
    ShaderValueType_Texture
};

///
/// A value for a shader parameter.
///
/// \note A shader value's type cannot change.
class ShaderValue
{
public:

    ///
    /// Constructs an empty shader value.
    ShaderValue();

    ///
    /// Constructs an integer shader value;
    ///
    /// \param value The value.
    ShaderValue(int value);

    ///
    /// Constructs a real shader value.
    ///
    /// \param value The value.
    ShaderValue(Real value);

    ///
    /// Constructs a 2-dimensional vector shader value.
    ///
    /// \param value The value.
    ShaderValue(const Vector2& value);

    ///
    /// Constructs a 3-dimensional vector shader value.
    ///
    /// \param value The value.
    ShaderValue(const Vector3& value);

    ///
    /// Constructs a 4-dimensional vector shader value.
    ///
    /// \param value The value.
    ShaderValue(const Vector4& value);

    ///
    /// Constructs a 4 by 4 matrix shader value.
    ///
    /// \param value The value.
    ShaderValue(const Matrix4& value);

    ///
    /// Constructs a texture shader value.
    ///
    /// \param value The texture.
    ShaderValue(const AssetHandle<Texture>& value);

    ///
    /// Returns the type.
    ShaderValueType type() const;

    ///
    /// Sets the type.
    ///
    /// \note The value is reset to the default value for the new type.
    ///
    /// \param type The new type.
    void setType(ShaderValueType type);

    ///
    /// Sets the value to the default value for the shader value's type.
    void setDefaultValue();

    ///
    /// Sets the value of the shader value as an integer.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the shader value is not an integer or texture.
    void setValue(int value);

    ///
    /// Sets the value of the shader value as a real number.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the shader value is not a real.
    void setValue(Real value);

    ///
    /// Sets the value of the shader value as a 2-dimensional vector.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the shader value is not a 2-dimensional vector.
    void setValue(const Vector2& value);

    ///
    /// Sets the value of the shader value as a 3-dimensional vector.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the shader value is not a 3-dimensional vector.
    void setValue(const Vector3& value);

    ///
    /// Sets the value of the shader value as a 4-dimensional vector.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the shader value is not a 4-dimensional vector.
    void setValue(const Vector4& value);

    ///
    /// Sets the value of the shader value as a 4 by 4 matrix.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the shader value is not a 4 by 4 matrix.
    void setValue(const Matrix4& value);

    ///
    /// Sets the value of the shader value as a texture.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the shader value is not a texture.
    void setValue(const AssetHandle<Texture>& value);

    ///
    /// Returns the value as an integer.
    int asInt() const;

    ///
    /// Returns the value as a real.
    Real asReal() const;

    ///
    /// Returns the value as a 2-dimensional vector.
    Vector2 asVector2() const;

    ///
    /// Returns the value as a 3-dimensional vector.
    Vector3 asVector3() const;

    ///
    /// Returns the value as a 4-dimensional vector.
    Vector4 asVector4() const;

    ///
    /// Returns the value as a 4 by 4 matrix.
    Matrix4 asMatrix4() const;

    ///
    /// Returns the value as a texture.
    AssetHandle<Texture> asTexture() const;

    ///
    /// Returns whether the shader value is equivalent to another.
    ///
    /// \param shaderValue The other shader value.
    bool operator==(const ShaderValue& shaderValue) const;

    ///
    /// Returns whether the shader value is different from another.
    ///
    /// \param shaderValue The other shader value.
    bool operator!=(const ShaderValue& shaderValue) const;

    friend Encoder& operator<<(Encoder& encoder, const ShaderValue& shaderValue);
    friend Decoder& operator>>(Decoder& decoder, ShaderValue& shaderValue);

private:
    ShaderValueType _type { ShaderValueType_Float };
    Any _value;
};

}