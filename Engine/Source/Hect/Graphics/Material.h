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
#include "Hect/IO/Asset.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"
#include "Hect/Graphics/Shader.h"

namespace hect
{

///
/// Describes how a surface is rendered within the render flow.
class HECT_EXPORT Material :
    public Asset<Material>
{
public:

    ///
    /// Constructs a blank material.
    Material();

    ///
    /// Constructs a blank shader.
    ///
    /// \param name The name of the material.
    Material(const std::string& name);

    ///
    /// Returns the shader.
    const AssetHandle<Shader>& shader() const;

    ///
    /// Sets the shader.
    ///
    /// \param shader The shader.
    void setShader(const AssetHandle<Shader>& shader);

    ///
    /// Returns the cull mode used for surfaces using this material.
    CullMode cullMode() const;

    ///
    /// Sets the cull mode used for surfaces using this material.
    ///
    /// \param cullMode The cull mode.
    void setCullMode(CullMode cullMode);

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

    friend HECT_EXPORT Encoder& operator<<(Encoder& encoder, const Material& material);
    friend HECT_EXPORT Decoder& operator>>(Decoder& decoder, Material& material);

private:
    AssetHandle<Shader> _shader;
    CullMode _cullMode { CullMode_CounterClockwise };
};

}