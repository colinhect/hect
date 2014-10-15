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

#include "Hect/Graphics/UniformValue.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// A binding from a shader uniform variable to a built-in value in the
/// rendering pipeline.
enum UniformBinding
{
    ///
    /// No binding.
    UniformBinding_None,

    ///
    /// Bound to the size of the active render target.
    UniformBinding_RenderTargetSize,

    ///
    /// Bound to the world-space position of the active camera.
    UniformBinding_CameraPosition,

    ///
    /// Bound to the world-space front direction of the active camera.
    UniformBinding_CameraFront,

    ///
    /// Bound to the world-space up direction of the active camera.
    UniformBinding_CameraUp,

    ///
    /// Bound to the view matrix.
    UniformBinding_ViewMatrix,

    ///
    /// Bound to the projection matrix.
    UniformBinding_ProjectionMatrix,

    ///
    /// Bound to the product of the view and projection matrices.
    UniformBinding_ViewProjectionMatrix,

    ///
    /// Bound to the model matrix.
    UniformBinding_ModelMatrix,

    ///
    /// Bound to the product of the model and the view matrix.
    UniformBinding_ModelViewMatrix,

    ///
    /// Bound to the product of the model, view, and projection matrices.
    UniformBinding_ModelViewProjectionMatrix
};

///
/// A variable of a shader which serves as parameter.
///
/// \note A uniform must either have a default value or a binding.  The type
/// is determined by those.
class Uniform
{
public:

    ///
    /// Constructs an empty uniform.
    Uniform();

    ///
    /// Constructs a uniform given its name and binding
    ///
    /// \param name The name.
    /// \param binding The binding.
    Uniform(const std::string& name, UniformBinding binding);

    ///
    /// Constructs a uniform given its name and default value.
    ///
    /// \param name The name.
    /// \param defaultValue The default value which will be set when the shader
    /// is bound (see GraphicsContext::bindShader()).
    Uniform(const std::string& name, const UniformValue& defaultValue);

    ///
    /// Returns the uniform type.
    UniformType type() const;

    ///
    /// Sets the uniform type.
    ///
    /// \param type The new type.
    ///
    /// \throws Error If the uniform has a binding.
    void setType(UniformType type);

    ///
    /// Returns the uniform binding.
    UniformBinding binding() const;

    ///
    /// Sets the uniform binding.
    ///
    /// \note The uniform type is changed to reflect the new binding.
    ///
    /// \param binding The new binding.
    void setBinding(UniformBinding binding);

    ///
    /// Returns whether the uniform has a binding.
    bool hasBinding() const;

    ///
    /// Returns the default value.
    const UniformValue& defaultValue() const;

    ///
    /// Sets the default value.
    ///
    /// \note The uniform type is changed to reflect the new value.
    ///
    /// \param defaultValue The new default value.
    void setDefaultValue(const UniformValue& defaultValue);

    ///
    /// Returns whether the uniform has a default value.
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
    /// Returns whether the uniform is equivalent to another.
    ///
    /// \param uniform The other uniform.
    bool operator==(const Uniform& uniform) const;

    ///
    /// Returns whether the uniform is different from another.
    ///
    /// \param uniform The other uniform.
    bool operator!=(const Uniform& uniform) const;

    friend Encoder& operator<<(Encoder& encoder, const Uniform& uniform);
    friend Decoder& operator>>(Decoder& decoder, Uniform& uniform);

private:
    std::string _name;

    UniformType _type { UniformType_Float };

    UniformBinding _binding { UniformBinding_None };

    bool _defaultValueSet { true };
    UniformValue _defaultValue;

    int _location { -1 };
};

}