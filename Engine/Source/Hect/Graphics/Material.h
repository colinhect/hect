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

#include "Hect/Core/Export.h"
#include "Hect/IO/Asset.h"
#include "Hect/Graphics/Shader.h"

namespace hect
{

///
/// Describes how a surface is rendered within the render flow.
class HECT_EXPORT Material :
    public Asset<Material>
{
    typedef std::vector<UniformValue> UniformValueContainer;
public:

    ///
    /// A sequence of uniform values.
    typedef Sequence<UniformValue, UniformValueContainer> UniformValueSequence;

    ///
    /// Constructs a blank material.
    Material();

    ///
    /// Constructs a blank material.
    ///
    /// \param name The name of the material.
    Material(Name name);

    ///
    /// Returns the shader.
    const AssetHandle<Shader>& shader() const;

    ///
    /// Sets the shader.
    ///
    /// \note Uniform values of the material are cleared before the shader
    /// changes.
    ///
    /// \param shader The shader.
    void set_shader(const AssetHandle<Shader>& shader);

    ///
    /// Returns the values for the shader's uniforms.
    UniformValueSequence uniform_values();

    ///
    /// \copydoc hect::Material::uniform_values()
    const UniformValueSequence uniform_values() const;

    ///
    /// Sets the value for the uniform of the shader of the specified name.
    ///
    /// \param name The name of the uniform to set the value for.
    /// \param value The uniform value.
    ///
    /// \throws InvalidOperation If no shader is set, the shader does not
    /// have a uniform of the specified name, the value is of the wrong
    /// type, or the uniform is bound.
    void set_uniform_value(Name name, const UniformValue& value);

    ///
    /// Clears all set uniform values.
    void clear_uniform_values();

    ///
    /// Returns the cull mode used for surfaces using this material.
    CullMode cull_mode() const;

    ///
    /// Sets the cull mode used for surfaces using this material.
    ///
    /// \param cull_mode The cull mode.
    void set_cull_mode(CullMode cull_mode);

    ///
    /// Returns whether the material is equivalent to another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param material The other material.
    bool operator==(const Material& material) const;

    ///
    /// Returns whether the material is different from another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param material The other material.
    bool operator!=(const Material& material) const;

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    AssetHandle<Shader> _shader;
    UniformValueContainer _uniform_values;
    CullMode _cull_mode { CullMode::CounterClockwise };
};

}
