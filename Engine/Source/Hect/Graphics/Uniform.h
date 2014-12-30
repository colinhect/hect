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

#include "Hect/Core/Export.h"
#include "Hect/Graphics/UniformBinding.h"
#include "Hect/Graphics/UniformValue.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// The index of a uniform within a shader.
typedef size_t UniformIndex;

///
/// The location of a uniform within a natively compiled shader.
typedef int UniformLocation;

///
/// The texture index a uniform corresponds to.
typedef size_t TextureIndex;

///
/// A parameter of a shader.
///
/// \note A uniform must either have a binding or a type.
class HECT_EXPORT Uniform
{
    friend class Shader;
public:

    ///
    /// Constructs a blank uniform.
    Uniform();

    ///
    /// Constructs a uniform of a specific type.
    ///
    /// \param name The name.
    /// \param type The type.
    Uniform(const std::string& name, UniformType type);

    ///
    /// Constructs a uniform bound to a built-in value.
    ///
    /// \param name The name.
    /// \param binding The binding.
    Uniform(const std::string& name, UniformBinding binding);

    ///
    /// Constructs a uniform with a default value.
    ///
    /// \param name The name.
    /// \param value The default value.
    Uniform(const std::string& name, const UniformValue& value);

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Returns the type.
    UniformType type() const;

    ///
    /// Returns the binding.
    UniformBinding binding() const;

    ///
    /// Returns the value.
    const UniformValue& value() const;

    ///
    /// Sets the value.
    ///
    /// \note Setting the value of a bound uniform will remove its binding.
    /// Setting the value can potentially change the uniform's type.
    ///
    /// \param value The new value.
    void setValue(const UniformValue& value);

    ///
    /// Returns the index of the uniform within its shader.
    UniformIndex index() const;

    ///
    /// Returns the associated texture index.
    TextureIndex textureIndex() const;

    ///
    /// Returns the compiled location.
    UniformLocation location() const;

    ///
    /// Sets the compiled location.
    ///
    /// \param location The compiled location.
    void setLocation(UniformLocation location);

    ///
    /// Returns whether the uniform is equivalent to another.
    ///
    /// \param uniform The other uniform.
    bool operator==(const Uniform& uniform) const;

    ///
    /// Returns whether the uniform is different from another.
    ///
    /// \param uniform The other uniform.
    bool operator!=(const Uniform& uniform) const;

    friend HECT_EXPORT Encoder& operator<<(Encoder& encoder, const Uniform& uniform);
    friend HECT_EXPORT Decoder& operator>>(Decoder& decoder, Uniform& uniform);

private:
    void resolveType();

    std::string _name;

    UniformType _type { UniformType_Float };
    UniformBinding _binding { UniformBinding_None };
    UniformValue _value;

    UniformIndex _index { UniformIndex(-1) };
    TextureIndex _textureIndex { TextureIndex(-1) };
    UniformLocation _location { -1 };
};

}