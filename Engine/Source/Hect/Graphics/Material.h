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
#include "Hect/IO/Asset.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"
#include "Hect/Graphics/RenderStage.h"
#include "Hect/Graphics/RenderState.h"
#include "Hect/Graphics/Shader.h"
#include "Hect/Graphics/Texture.h"
#include "Hect/Graphics/ShaderArgument.h"

namespace hect
{

///
/// The manner in which a surface is rendered.
class Material :
    public Asset
{
    typedef std::vector<ShaderArgument> ShaderArgumentContainer;
public:

    ///
    /// A sequence of shader arguments.
    typedef Sequence<ShaderArgument, ShaderArgumentContainer> ShaderArgumentSequence;

    ///
    /// Constructs a material.
    Material();

    ///
    /// Constructs a material.
    ///
    /// \param name The name of the material.
    Material(const std::string& name);

    ///
    /// Prepares a renderer to begin using this material.
    ///
    /// \param renderer The renderer to prepare.
    void prepare(Renderer& renderer);

    ///
    /// Returns the shader.
    const AssetHandle<Shader>& shader() const;

    ///
    /// Sets the shader that the material will bind.
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
    /// Removes all shader arguments.
    void clearShaderArguments();

    ///
    /// Returns the stage when the material is rendered.
    RenderStage renderStage() const;

    ///
    /// Sets the stage when the material is rendered.
    ///
    /// \param renderStage The new render stage.
    void setRenderStage(RenderStage renderStage);

    ///
    /// Returns the render state.
    const RenderState& renderState() const;

    ///
    /// Sets the render state that the material will bind.
    ///
    /// \param renderState The render state.
    void setRenderState(const RenderState& renderState);

    ///
    /// Returns the render priority.
    int priority() const;

    ///
    /// Sets the render priority.
    ///
    /// \param priority The priority.
    void setPriority(int priority);

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

    friend Encoder& operator<<(Encoder& encoder, const Material& material);
    friend Decoder& operator>>(Decoder& decoder, Material& material);

private:

    // Resolves which shader parameters the arguments apply to for fast binding
    void resolveShaderParameters(Shader& shader);

    AssetHandle<Shader> _shader;
    ShaderArgumentContainer _shaderArguments;

    RenderStage _renderStage{ RenderStage_None };
    RenderState _renderState;

    int _priority{ 0 };

    std::map<const ShaderParameter*, ShaderValue> _resolvedShaderParameters;
    Shader* _resolvedFromShader{ nullptr };
};

}