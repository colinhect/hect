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
#include <unordered_map>

#include "Hect/Core/Sequence.h"
#include "Hect/IO/Asset.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"
#include "Hect/Graphics/MaterialArgument.h"
#include "Hect/Graphics/MaterialParameter.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/RenderStage.h"
#include "Hect/Graphics/RenderState.h"
#include "Hect/Graphics/ShaderModule.h"
#include "Hect/Graphics/Texture.h"

namespace hect
{

///
/// The manner in which a surface is rendered.
class Material :
    public Asset,
    public Renderer::Object<Material>
{
    typedef std::vector<ShaderModule> ShaderModuleContainer;
    typedef std::vector<MaterialParameter> ParameterContainer;
    typedef std::vector<MaterialArgument> ArgumentContainer;
public:

    ///
    /// A sequence of shader modules.
    typedef Sequence<ShaderModule, ShaderModuleContainer> ShaderModuleSequence;

    ///
    /// A sequence of material parameters.
    typedef Sequence<MaterialParameter, ParameterContainer> ParameterSequence;

    ///
    /// A sequence of material arguments.
    typedef Sequence<MaterialArgument, ArgumentContainer> ArgumentSequence;

    ///
    /// Constructs a material.
    Material();

    ///
    /// Constructs a material.
    ///
    /// \param name The name of the material.
    Material(const std::string& name);

    ///
    /// Returns the base material.
    AssetHandle<Material> base() const;

    ///
    /// Sets the base material.
    ///
    /// \note If the material is uploaded to a renderer then it will be
    /// destroyed before the module is added.
    ///
    /// \param base The base material.
    void setBase(const AssetHandle<Material>& base);

    ///
    /// Adds a shader module to the material.
    ///
    /// \note If the material is uploaded to a renderer then it will be
    /// destroyed before the module is added.
    ///
    /// \param module The module to add.
    void addShaderModule(const ShaderModule& shaderModule);

    ///
    /// Returns the shader modules.
    ShaderModuleSequence shaderModules();

    ///
    /// Returns the shader modules.
    const ShaderModuleSequence shaderModules() const;

    ///
    /// Adds a parameter to the material.
    ///
    /// \note If the material is uploaded to a renderer then it will be
    /// destroyed before the parameter is added.
    ///
    /// \param parameter The parameter to add.
    ///
    /// \throws Error if the material already has a parameter of the same name.
    void addParameter(const MaterialParameter& parameter);

    ///
    /// Returns the parameters.
    ParameterSequence parameters();

    ///
    /// Returns the parameters.
    const ParameterSequence parameters() const;

    ///
    /// Returns the parameter with the specified name.
    ///
    /// \param name The name of the parameter.
    ///
    /// \throws Error If no parameter with the specified name exists.
    const MaterialParameter& parameterWithName(const std::string& name) const;

    ///
    /// \copydoc Material::parameterWithName()
    const MaterialParameter& parameterWithName(const char* name) const;

    ///
    /// Returns the material arguments.
    const ArgumentSequence arguments() const;

    ///
    /// Adds a material argument value.
    ///
    /// \param materialArgument The material argument to add.
    void addArgument(const MaterialArgument& argument);

    ///
    /// Removes all material arguments.
    void clearArguments();

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
    unsigned nextTextureIndex() const;

    AssetHandle<Material> _base;

    ArgumentContainer _arguments;

    RenderStage _renderStage{ RenderStage_None };
    RenderState _renderState;

    int _priority{ 0 };

    ShaderModuleContainer _shaderModules;

    ParameterContainer _parameters;

    std::map<std::string, size_t> _parameterIndices;
    mutable std::unordered_map<const char*, size_t> _parameterIndicesHashed;
};

}