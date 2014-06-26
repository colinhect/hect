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
#include "Hect/Math/Vector2.h"
#include "Hect/Math/Vector3.h"
#include "Hect/Math/Vector4.h"
#include "Hect/Math/Matrix4.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

///
/// A uniform value type.
namespace UniformType
{
enum Enum : uint8_t
{
    ///
    /// An integer.
    Int,

    ///
    /// A floating point number.
    Float,

    ///
    /// A 2-dimensional vector.
    Vector2,

    ///
    /// A 3-dimensional vector.
    Vector3,

    ///
    /// A 4-dimensional vector.
    Vector4,

    ///
    /// A 4 by 4 matrix.
    Matrix4,

    ///
    /// A texture (the index of the texture).
    Texture
};
}

///
/// A value for a uniform.
///
/// \note A uniform's type cannot change.
class UniformValue :
    public Encodable
{
public:

    ///
    /// Constructs an empty uniform value.
    UniformValue();

    ///
    /// Constructs an integer based uniform value given its the integer value
    /// and its type.
    ///
    /// \param value The integer value.
    /// \param type The type.
    UniformValue(int value, UniformType::Enum type);

    ///
    /// Constructs a real uniform value.
    ///
    /// \param value The value.
    UniformValue(Real value);

    ///
    /// Constructs a 2-dimensional vector uniform value.
    ///
    /// \param value The value.
    UniformValue(const Vector2& value);

    ///
    /// Constructs a 3-dimensional vector uniform value.
    ///
    /// \param value The value.
    UniformValue(const Vector3& value);

    ///
    /// Constructs a 4-dimensional vector uniform value.
    ///
    /// \param value The value.
    UniformValue(const Vector4& value);

    ///
    /// Constructs a 4 by 4 matrix uniform value.
    ///
    /// \param value The value.
    UniformValue(const hect::Matrix4& value);

    ///
    /// Returns the type.
    UniformType::Enum type() const;

    ///
    /// Sets the type.
    ///
    /// \note The value is reset to the default value for the new type.
    ///
    /// \param type The new type.
    void setType(UniformType::Enum type);

    ///
    /// Returns the raw data.
    const void* data() const;

    ///
    /// Sets the value to the default value for the uniform value's type.
    void setDefaultValue();

    ///
    /// Sets the value of the uniform value as an integer.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the uniform value is not an integer or texture.
    void setValue(int value);

    ///
    /// Sets the value of the uniform value as a real number.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the uniform value is not a real.
    void setValue(Real value);

    ///
    /// Sets the value of the uniform value as a 2-dimensional vector.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the uniform value is not a 2-dimensional vector.
    void setValue(const Vector2& value);

    ///
    /// Sets the value of the uniform value as a 3-dimensional vector.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the uniform value is not a 3-dimensional vector.
    void setValue(const Vector3& value);

    ///
    /// Sets the value of the uniform value as a 4-dimensional vector.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the uniform value is not a 4-dimensional vector.
    void setValue(const Vector4& value);

    ///
    /// Sets the value of the uniform value as a 4 by 4 matrix.
    ///
    /// \param value The value.
    ///
    /// \throws Error If the uniform value is not a 4 by 4 matrix.
    void setValue(const Matrix4& value);

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
    /// Encodes the uniform value.
    ///
    /// \param encoder The encoder to use.
    void encode(ObjectEncoder& encoder) const;

    ///
    /// Decodes the uniform value.
    ///
    /// \param decoder The decoder to use.
    /// \param assetCache The asset cache to get referenced assets from.
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

    ///
    /// Returns whether the uniform value is equivalent to another.
    ///
    /// \param uniformValue The other uniform value.
    bool operator==(const UniformValue& uniformValue) const;

    ///
    /// Returns whether the uniform value is different from another.
    ///
    /// \param uniformValue The other uniform value.
    bool operator!=(const UniformValue& uniformValue) const;

private:

    UniformType::Enum _type;

    Any _value;
};

}