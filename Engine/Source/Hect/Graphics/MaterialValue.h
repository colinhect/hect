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
/// A material value type.
enum MaterialValueType
{
    ///
    /// A null value.
    MaterialValueType_Null,

    ///
    /// An integer.
    MaterialValueType_Int,

    ///
    /// A floating point number.
    MaterialValueType_Float,

    ///
    /// A 2-dimensional vector.
    MaterialValueType_Vector2,

    ///
    /// A 3-dimensional vector.
    MaterialValueType_Vector3,

    ///
    /// A 4-dimensional vector.
    MaterialValueType_Vector4,

    ///
    /// A 4 by 4 matrix.
    MaterialValueType_Matrix4,

    ///
    /// A texture.
    MaterialValueType_Texture
};

///
/// A value for a material parameter.
///
/// \note A material value's type cannot change.
class MaterialValue
{
public:

    ///
    /// Constructs a null material value.
    MaterialValue();

    ///
    /// Constructs a default material value.
    ///
    /// \param type The value type.
    MaterialValue(MaterialValueType type);

    ///
    /// Constructs an integer material value;
    ///
    /// \param value The value.
    MaterialValue(int value);

    ///
    /// Constructs a real material value.
    ///
    /// \param value The value.
    MaterialValue(Real value);

    ///
    /// Constructs a 2-dimensional vector material value.
    ///
    /// \param value The value.
    MaterialValue(const Vector2& value);

    ///
    /// Constructs a 3-dimensional vector material value.
    ///
    /// \param value The value.
    MaterialValue(const Vector3& value);

    ///
    /// Constructs a 4-dimensional vector material value.
    ///
    /// \param value The value.
    MaterialValue(const Vector4& value);

    ///
    /// Constructs a 4 by 4 matrix material value.
    ///
    /// \param value The value.
    MaterialValue(const Matrix4& value);

    ///
    /// Constructs a texture material value.
    ///
    /// \param value The texture.
    MaterialValue(const AssetHandle<Texture>& value);

    ///
    /// Returns the type.
    MaterialValueType type() const;

    ///
    /// Sets the value to the default value for the material value's type.
    void setDefaultValue();

    ///
    /// Sets the value of the material value as an integer.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the material value is not an integer or texture.
    void setValue(int value);

    ///
    /// Sets the value of the material value as a real number.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the material value is not a real.
    void setValue(Real value);

    ///
    /// Sets the value of the material value as a 2-dimensional vector.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the material value is not a 2-dimensional vector.
    void setValue(const Vector2& value);

    ///
    /// Sets the value of the material value as a 3-dimensional vector.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the material value is not a 3-dimensional vector.
    void setValue(const Vector3& value);

    ///
    /// Sets the value of the material value as a 4-dimensional vector.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the material value is not a 4-dimensional vector.
    void setValue(const Vector4& value);

    ///
    /// Sets the value of the material value as a 4 by 4 matrix.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the material value is not a 4 by 4 matrix.
    void setValue(const Matrix4& value);

    ///
    /// Sets the value of the material value as a texture.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the material value is not a texture.
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
    /// Returns whether the material value is equivalent to another.
    ///
    /// \param materialValue The other material value.
    bool operator==(const MaterialValue& materialValue) const;

    ///
    /// Returns whether the material value is different from another.
    ///
    /// \param materialValue The other material value.
    bool operator!=(const MaterialValue& materialValue) const;

    friend Encoder& operator<<(Encoder& encoder, const MaterialValue& materialValue);
    friend Decoder& operator>>(Decoder& decoder, MaterialValue& materialValue);

private:

    MaterialValueType _type { MaterialValueType_Null };
    Any _value;
};

}