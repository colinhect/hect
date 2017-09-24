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

#include "Hect/Core/Any.h"
#include "Hect/Core/Export.h"
#include "Hect/Graphics/Color.h"
#include "Hect/Graphics/UniformType.h"
#include "Hect/Graphics/Texture2.h"
#include "Hect/Graphics/Texture3.h"
#include "Hect/Graphics/TextureCube.h"
#include "Hect/IO/AssetCache.h"
#include "Hect/IO/Encodable.h"
#include "Hect/Math/Vector2.h"
#include "Hect/Math/Vector3.h"
#include "Hect/Math/Vector4.h"
#include "Hect/Math/Matrix4.h"

namespace hect
{

class Texture2;

///
/// A value for a uniform.
///
/// \note A uniform value's type cannot change.
class HECT_EXPORT UniformValue :
    public Encodable
{
public:

    ///
    /// Constructs a null uniform value.
    UniformValue();

    ///
    /// Constructs a default uniform value.
    ///
    /// \param type The value type.
    UniformValue(UniformType type);

    ///
    /// Constructs an integer uniform value;
    ///
    /// \param value The value.
    UniformValue(int value);

    ///
    /// Constructs a floating point uniform value.
    ///
    /// \param value The value.
    UniformValue(double value);

    ///
    /// Constructs a 2-dimensional vector uniform value.
    ///
    /// \param value The value.
    UniformValue(Vector2 value);

    ///
    /// Constructs a 3-dimensional vector uniform value.
    ///
    /// \param value The value.
    UniformValue(Vector3 value);

    ///
    /// Constructs a 4-dimensional vector uniform value.
    ///
    /// \param value The value.
    UniformValue(Vector4 value);

    ///
    /// Constructs a 4 by 4 matrix uniform value.
    ///
    /// \param value The value.
    UniformValue(const Matrix4& value);

    ///
    /// Constructs a color uniform value.
    ///
    /// \param value The value.
    UniformValue(Color value);

    ///
    /// Constructs a 2-dimensional texture uniform value.
    ///
    /// \param value The texture.
    UniformValue(const AssetHandle<Texture2>& value);

    ///
    /// Constructs a 3-dimensional texture uniform value.
    ///
    /// \param value The texture.
    UniformValue(const AssetHandle<Texture3>& value);

    ///
    /// Constructs a cubic texture uniform value.
    ///
    /// \param value The texture.
    UniformValue(const AssetHandle<TextureCube>& value);

    ///
    /// Returns the type.
    UniformType type() const;

    ///
    /// Sets the value to the default value for the uniform value's type.
    void set_default_value();

    ///
    /// Sets the value of the uniform value as an integer.
    ///
    /// \param value The value.
    ///
    /// \throws InvalidOperation If the uniform value is not an integer.
    void set_value(int value);

    ///
    /// Sets the value of the uniform value as a double.
    ///
    /// \param value The value.
    ///
    /// \throws InvalidOperation If the uniform value is not a floating point.
    void set_value(double value);

    ///
    /// Sets the value of the uniform value as a 2-dimensional vector.
    ///
    /// \param value The value.
    ///
    /// \throws InvalidOperation If the uniform value is not a 2-dimensional
    /// vector.
    void set_value(Vector2 value);

    ///
    /// Sets the value of the uniform value as a 3-dimensional vector.
    ///
    /// \param value The value.
    ///
    /// \throws InvalidOperation If the uniform value is not a 3-dimensional
    /// vector.
    void set_value(Vector3 value);

    ///
    /// Sets the value of the uniform value as a 4-dimensional vector.
    ///
    /// \param value The value.
    ///
    /// \throws InvalidOperation If the uniform value is not a 4-dimensional
    /// vector.
    void set_value(Vector4 value);

    ///
    /// Sets the value of the uniform value as a 4 by 4 matrix.
    ///
    /// \param value The value.
    ///
    /// \throws InvalidOperation If the uniform value is not a 4 by 4 matrix.
    void set_value(const Matrix4& value);

    ///
    /// Sets the value of the uniform value as a color.
    ///
    /// \param value The value.
    ///
    /// \throws InvalidOperation If the uniform value is not a color.
    void set_value(Color value);

    ///
    /// Sets the value of the uniform value as a 2-dimensional texture.
    ///
    /// \param value The value.
    ///
    /// \throws InvalidOperation If the uniform value is not a texture.
    void set_value(const AssetHandle<Texture2>& value);

    ///
    /// Sets the value of the uniform value as a 3-dimensional texture.
    ///
    /// \param value The value.
    ///
    /// \throws InvalidOperation If the uniform value is not a texture.
    void set_value(const AssetHandle<Texture3>& value);

    ///
    /// Sets the value of the uniform value as a cubic texture.
    ///
    /// \param value The value.
    ///
    /// \throws InvalidOperation If the uniform value is not a texture.
    void set_value(const AssetHandle<TextureCube>& value);

    ///
    /// Returns the value as an integer.
    int as_int() const;

    ///
    /// Returns the value as a double.
    double as_double() const;

    ///
    /// Returns the value as a 2-dimensional vector.
    Vector2 as_vector2() const;

    ///
    /// Returns the value as a 3-dimensional vector.
    Vector3 as_vector3() const;

    ///
    /// Returns the value as a 4-dimensional vector.
    Vector4 as_vector4() const;

    ///
    /// Returns the value as a 4 by 4 matrix.
    Matrix4 as_matrix4() const;

    ///
    /// Returns the value as a color.
    Color as_color() const;

    ///
    /// Returns the value as a 2-dimensional texture.
    AssetHandle<Texture2> as_texture2() const;

    ///
    /// Returns the value as a 3-dimensional texture.
    AssetHandle<Texture3> as_texture3() const;

    ///
    /// Returns the value as a cubic texture.
    AssetHandle<TextureCube> as_texture_cube() const;

    ///
    /// Returns whether the value is not null.
    operator bool() const;

    ///
    /// Returns whether the uniform value is equivalent to another.
    ///
    /// \param uniform_value The other uniform value.
    bool operator==(const UniformValue& uniform_value) const;

    ///
    /// Returns whether the uniform value is different from another.
    ///
    /// \param uniform_value The other uniform value.
    bool operator!=(const UniformValue& uniform_value) const;

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    UniformType _type { UniformType::Float };
    Any _value;
};

}