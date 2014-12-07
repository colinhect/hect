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
#include "Hect/Core/StringMap.h"
#include "Hect/IO/Asset.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"
#include "Hect/Graphics/MaterialParameter.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/RenderState.h"
#include "Hect/Graphics/ShaderModule.h"
#include "Hect/Graphics/Texture.h"

namespace hect
{

///
/// A stage within the rendering flow.
enum MaterialType
{
    ///
    /// No specific stage.
    MaterialType_None,

    ///
    /// The stage for rendering opaque geometry to the geometry buffer.
    MaterialType_OpaqueGeometry,

    ///
    /// The stage for rendering lights to the light accumulation buffer.
    MaterialType_LightAccumulation,
};

///
/// The manner in which a surface is rendered.
class Material :
    public Asset,
    public Renderer::Object<Material>
{
    typedef std::vector<ShaderModule> ShaderModuleContainer;
    typedef std::vector<MaterialParameter> ParameterContainer;
public:

    ///
    /// A sequence of shader modules.
    typedef Sequence<ShaderModule, ShaderModuleContainer> ShaderModuleSequence;

    ///
    /// A sequence of material parameters.
    typedef Sequence<MaterialParameter, ParameterContainer> ParameterSequence;

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
    /// Returns the material type.
    MaterialType type() const;

    ///
    /// Returns the shader modules.
    ShaderModuleSequence shaderModules();

    ///
    /// Returns the shader modules.
    const ShaderModuleSequence shaderModules() const;

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
    /// Sets the argument for the specified argument.
    ///
    /// \param parameter The parameter to set the argument for.
    /// \param value The value of the argument.
    void setArgument(const MaterialParameter& parameter, MaterialValue value);

    ///
    /// Sets the argument for the specified argument.
    ///
    /// \param parameterName The name of the parameter to set the argument for.
    /// \param value The value of the argument.
    void setArgument(const std::string& parameterName, MaterialValue value);

    ///
    /// Sets the argument for the specified argument.
    ///
    /// \param parameterName The name of the parameter to set the argument for.
    /// \param value The value of the argument.
    void setArgument(const char* parameterName, MaterialValue value);

    ///
    /// Gets the argument value for the specified parameter.
    ///
    /// \param parameter The parameter to get the argument for.
    const MaterialValue& argumentForParameter(const MaterialParameter& parameter) const;

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
    void addParameter(const std::string& name, MaterialValueType type, MaterialParameterBinding binding);
    unsigned nextTextureIndex() const;

    AssetHandle<Material> _base;

    MaterialType _type { MaterialType_None };

    RenderState _renderState;

    int _priority { 0 };

    ShaderModuleContainer _shaderModules;

    ParameterContainer _parameters;
    StringMap<size_t> _parameterIndices;
    std::vector<MaterialValue> _parameterValues;
};

}