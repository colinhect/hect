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

#include <vector>

#include "Hect/Core/Sequence.h"
#include "Hect/Core/StringMap.h"
#include "Hect/IO/Asset.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"
#include "Hect/Graphics/BlendFactor.h"
#include "Hect/Graphics/MaterialParameter.h"
#include "Hect/Graphics/MaterialType.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/ShaderSource.h"
#include "Hect/Graphics/Texture.h"

namespace hect
{

///
/// The manner in which a surface is rendered and how it is used within the
/// render flow.
class Material :
    public Asset,
    public Renderer::Object<Material>
{
    typedef std::vector<ShaderSource> ShaderSourceContainer;
    typedef std::vector<MaterialParameter> ParameterContainer;
public:

    ///
    /// A sequence of shader sources.
    typedef Sequence<ShaderSource, ShaderSourceContainer> ShaderSourceSequence;

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
    /// Returns the material type.
    MaterialType type() const;

    ///
    /// Sets the material type.
    ///
    /// \param type The new material type.
    void setType(MaterialType type);

    ///
    /// Returns whether blending is enabled.
    bool blend() const;

    ///
    /// Sets whether blending is enabled.
    ///
    /// \param value True to enable blending; false to disable.
    void setBlend(bool value);

    ///
    /// Sets the source and destination blend factors.
    ///
    /// \param source The source factor.
    /// \param destination The destination factor.
    void setBlendFactors(BlendFactor source, BlendFactor destination);

    ///
    /// Returns the source blend factor.
    BlendFactor sourceBlendFactor() const;

    ///
    /// Returns the destination blend factor.
    BlendFactor destinationBlendFactor() const;

    ///
    /// Returns whether depth testing is enabled.
    bool depthTest() const;

    ///
    /// Sets whether depth testing is enabled.
    ///
    /// \param value True to enable depth testing; false to disable.
    void setDepthTest(bool value);

    ///
    /// Returns whether depth writing is enabled.
    bool depthWrite() const;

    ///
    /// Sets whether depth writing is enabled.
    ///
    /// \param value True to enable depth writing; false to disable.
    void setDepthWrite(bool value);

    ///
    /// Returns whether back face culling is enabled.
    bool cullBackFace() const;

    ///
    /// Sets whether back face culling is enabled.
    ///
    /// \param value True to enable back face culling; false to disable.
    void setCullBackFace(bool value);

    ///
    /// Returns the render priority.
    int priority() const;

    ///
    /// Sets the render priority.
    ///
    /// \param priority The priority.
    void setPriority(int priority);

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
    const MaterialParameter& parameter(const std::string& name) const;

    ///
    /// \copydoc Material::parameter()
    const MaterialParameter& parameter(const char* name) const;

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
    /// Returns the shader sources.
    ShaderSourceSequence shaderSources();

    ///
    /// Returns the shader sources.
    const ShaderSourceSequence shaderSources() const;

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

    MaterialType _type { MaterialType_None };

    bool _blend { false };
    bool _depthTest{ true };
    bool _depthWrite{ true };
    bool _cullBackFace{ true };

    BlendFactor _sourceFactor { BlendFactor_One };
    BlendFactor _destinationFactor { BlendFactor_One };

    int _priority { 0 };

    ShaderSourceContainer _shaderSources;

    ParameterContainer _parameters;
    StringMap<size_t> _parameterIndices;
    std::vector<MaterialValue> _parameterValues;
};

}