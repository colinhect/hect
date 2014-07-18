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

#include "Hect/Core/CollectionAccessor.h"
#include "Hect/IO/AssetHandle.h"
#include "Hect/Graphics/PassUniformValue.h"
#include "Hect/Graphics/RenderState.h"
#include "Hect/Graphics/Shader.h"
#include "Hect/Graphics/Texture.h"

namespace hect
{

///
/// A step in a material technique.
///
/// \note Contains the render state, textures, shader, and uniform values
/// used when rendering geometry for this step in the technique.
class Pass
{
public:

    ///
    /// Constructs an empty pass.
    Pass();

    ///
    /// Prepares a renderer to begin using this pass.
    ///
    /// \param renderer The renderer to prepare.
    void prepare(Renderer& renderer);
    
    ///
    /// Returns the render state.
    const RenderState& renderState() const;

    ///
    /// Sets the render state that the pass will bind.
    ///
    /// \param renderState The render state.
    void setRenderState(const RenderState& renderState);

    ///
    /// Returns the textures.
    const CollectionAccessor<AssetHandle<Texture>> textures() const;

    ///
    /// Adds a texture.
    ///
    /// \param texture The texture to add.
    void addTexture(const AssetHandle<Texture>& texture);

    ///
    /// Removes all textures.
    void clearTextures();

    ///
    /// Returns the shader.
    const AssetHandle<Shader>& shader() const;

    ///
    /// Sets the shader that the pass will bind.
    ///
    /// \param shader The shader.
    void setShader(const AssetHandle<Shader>& shader);

    ///
    /// Returns the uniform values.
    const CollectionAccessor<PassUniformValue> uniformValues() const;

    ///
    /// Adds a uniform value.
    ///
    /// \param name The name of the uniform.
    /// \param value The value.
    void addUniformValue(const std::string& name, const UniformValue& uniformValue);

private:

    // Resolves which uniforms the uniform values apply to for fast
    // binding
    void _resolvePassUniformValues(Shader& shader);

    RenderState _renderState;
    std::vector<AssetHandle<Texture>> _textures;
    AssetHandle<Shader> _shader;

    std::vector<PassUniformValue> _uniformValues;
    std::map<const Uniform*, UniformValue> _resolvedUniformValues;
    Shader* _resolvedFromShader;
};

}