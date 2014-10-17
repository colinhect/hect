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

#include <map>

#include "Hect/Core/Sequence.h"
#include "Hect/IO/AssetHandle.h"
#include "Hect/Graphics/RenderState.h"
#include "Hect/Graphics/Shader.h"
#include "Hect/Graphics/Texture.h"
#include "Hect/Graphics/ShaderArgument.h"

namespace hect
{

///
/// A step in a material technique.
///
/// \note Contains the render state, textures, shader, and shader arguments
/// used when rendering geometry for this step in the technique.
class Pass
{
    typedef std::vector<AssetHandle<Texture>> TextureContainer;
    typedef std::vector<ShaderArgument> ShaderArgumentContainer;
public:

    ///
    /// A sequence of textures.
    typedef Sequence<AssetHandle<Texture>, TextureContainer> TextureSequence;

    ///
    /// A sequence of shader arguments.
    typedef Sequence<ShaderArgument, ShaderArgumentContainer> ShaderArgumentSequence;

    ///
    /// Prepares a graphics context to begin using this pass.
    ///
    /// \param graphicsContext The graphics context to prepare.
    void prepare(GraphicsContext& graphicsContext);

    ///
    /// Returns the shader.
    const AssetHandle<Shader>& shader() const;

    ///
    /// Sets the shader that the pass will bind.
    ///
    /// \param shader The shader.
    void setShader(const AssetHandle<Shader>& shader);

    ///
    /// Returns the shader arguments.
    const ShaderArgumentSequence shaderArguments() const;

    ///
    /// Adds a shader argument value.
    ///
    /// \param shaderArgument The shader argument to add.
    void addShaderArgument(const ShaderArgument& shaderArgument);

    ///
    /// Returns the textures.
    const TextureSequence textures() const;

    ///
    /// Adds a texture.
    ///
    /// \param texture The texture to add.
    void addTexture(const AssetHandle<Texture>& texture);

    ///
    /// Removes all textures.
    void clearTextures();

    ///
    /// Returns the render state.
    const RenderState& renderState() const;

    ///
    /// Sets the render state that the pass will bind.
    ///
    /// \param renderState The render state.
    void setRenderState(const RenderState& renderState);

    ///
    /// Returns whether the pass is equivalent to another.
    ///
    /// \param pass The other pass.
    bool operator==(const Pass& pass) const;

    ///
    /// Returns whether the pass is different from another.
    ///
    /// \param pass The other pass.
    bool operator!=(const Pass& pass) const;

    friend Encoder& operator<<(Encoder& encoder, const Pass& pass);
    friend Decoder& operator>>(Decoder& decoder, Pass& pass);

private:

    // Resolves which shader parameters the arguments apply to for fast binding
    void resolveShaderParameters(Shader& shader);

    AssetHandle<Shader> _shader;
    ShaderArgumentContainer _shaderArguments;
    TextureContainer _textures;
    RenderState _renderState;

    std::map<const ShaderParameter*, ShaderValue> _resolvedShaderParameters;
    Shader* _resolvedFromShader { nullptr };
};

}