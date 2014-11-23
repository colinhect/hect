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

#include "Hect/Graphics/ShaderValue.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// A binding from a shader parameter to a built-in value in the rendering
/// pipeline.
enum ShaderParameterBinding
{
    ///
    /// No binding.
    ShaderParameterBinding_None,

    ///
    /// Bound to the size of the active render target.
    ShaderParameterBinding_RenderTargetSize,

    ///
    /// Bound to the world-space position of the active camera.
    ShaderParameterBinding_CameraPosition,

    ///
    /// Bound to the world-space front direction of the active camera.
    ShaderParameterBinding_CameraFront,

    ///
    /// Bound to the world-space up direction of the active camera.
    ShaderParameterBinding_CameraUp,

    ///
    /// Bound to the exposure of the active camera.
    ShaderParameterBinding_CameraExposure,

    ///
    /// Bound to the one divided by the gamma of the active camera.
    ShaderParameterBinding_CameraOneOverGamma,

    ///
    /// Bound to the view matrix.
    ShaderParameterBinding_ViewMatrix,

    ///
    /// Bound to the projection matrix.
    ShaderParameterBinding_ProjectionMatrix,

    ///
    /// Bound to the product of the view and projection matrices.
    ShaderParameterBinding_ViewProjectionMatrix,

    ///
    /// Bound to the model matrix.
    ShaderParameterBinding_ModelMatrix,

    ///
    /// Bound to the product of the model and the view matrix.
    ShaderParameterBinding_ModelViewMatrix,

    ///
    /// Bound to the product of the model, view, and projection matrices.
    ShaderParameterBinding_ModelViewProjectionMatrix
};

///
/// A parameter of a shader.
///
/// \note A shader parameter must either have a default value or a binding.
/// The type is determined by those.
class ShaderParameter
{
public:

    ///
    /// Constructs a shader parameter.
    ShaderParameter();

    ///
    /// Constructs a shader parameter.
    ///
    /// \param name The name.
    /// \param binding The binding.
    ShaderParameter(const std::string& name, ShaderParameterBinding binding);

    ///
    /// Constructs a shader parameter.
    ///
    /// \param name The name.
    /// \param defaultValue The default value which will be set when the shader
    /// is bound (see Renderer::bindShader()).
    ShaderParameter(const std::string& name, const ShaderValue& defaultValue);

    ///
    /// Returns the value type the parameter accepts.
    ShaderValueType type() const;

    ///
    /// Sets the value type the parameter accepts.
    ///
    /// \param type The new type.
    ///
    /// \throws Error If the parameter has a binding.
    void setType(ShaderValueType type);

    ///
    /// Returns the binding.
    ShaderParameterBinding binding() const;

    ///
    /// Sets the binding.
    ///
    /// \note The value type is changed to reflect the new binding.
    ///
    /// \param binding The new binding.
    void setBinding(ShaderParameterBinding binding);

    ///
    /// Returns whether the parameter has a binding.
    bool hasBinding() const;

    ///
    /// Returns the default value.
    const ShaderValue& defaultValue() const;

    ///
    /// Sets the default value.
    ///
    /// \note The value type is changed to reflect the new value.
    ///
    /// \param defaultValue The new default value.
    void setDefaultValue(const ShaderValue& defaultValue);

    ///
    /// Returns whether the parameter has a default value.
    bool hasDefaultValue() const;

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Sets the name.
    ///
    /// \param name The new name.
    void setName(const std::string& name);

    ///
    /// Returns the compiled location.
    int location() const;

    ///
    /// Sets the compiled location.
    ///
    /// \param location The compiled location.
    void setLocation(int location);

    ///
    /// Returns whether the shader parameter is equivalent to another.
    ///
    /// \param shaderParameter The other shader parameter.
    bool operator==(const ShaderParameter& shaderParameter) const;

    ///
    /// Returns whether the shdaer parameter is different from another.
    ///
    /// \param shaderParameter The other shader parameter.
    bool operator!=(const ShaderParameter& shaderParameter) const;

    friend Encoder& operator<<(Encoder& encoder, const ShaderParameter& shaderParameter);
    friend Decoder& operator>>(Decoder& decoder, ShaderParameter& shaderParameter);

private:
    std::string _name;

    ShaderValueType _type { ShaderValueType_Float };

    ShaderParameterBinding _binding { ShaderParameterBinding_None };

    bool _defaultValueSet { true };
    ShaderValue _defaultValue;

    int _location { -1 };
};

}