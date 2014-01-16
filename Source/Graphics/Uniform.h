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

namespace hect
{

///
/// A binding from a shader uniform variable to a built-in value in the
/// rendering pipeline.
enum class UniformBinding
{
    ///
    /// No binding.
    None,

    ///
    /// Bound to the size of the active render target.
    RenderTargetSize,

    ///
    /// Bound to the world-space position of the active camera.
    CameraPosition,

    ///
    /// Bound to the world-space up direction of the active camera.
    CameraUp,

    ///
    /// Bound to the view matrix.
    ViewMatrix,

    ///
    /// Bound to the projection matrix.
    ProjectionMatrix,

    ///
    /// Bound to the product of the view and projection matrices.
    ViewProjectionMatrix,

    ///
    /// Bound to the model matrix.
    ModelMatrix,

    ///
    /// Bound to the product of the model and the view matrix.
    ModelViewMatrix,

    ///
    /// Bound to the product of the model, view, and projection matrices.
    ModelViewProjectionMatrix
};

///
/// A global variable of a shader which serves as parameter.
class Uniform
{
public:

    ///
    /// An array of uniforms.
    typedef std::vector<Uniform> Array;

    ///
    /// Constructs a uniform given its name and type.
    ///
    /// \param name The name.
    /// \param type The type.
    Uniform(const std::string& name, UniformType type);

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
    /// is bound (see Renderer::bindShader()).
    Uniform(const std::string& name, const UniformValue& defaultValue);

    ///
    /// Returns the uniform type.
    UniformType type() const;

    ///
    /// Returns the uniform binding.
    UniformBinding binding() const;

    ///
    /// Returns whether the uniform has a binding.
    bool hasBinding() const;

    ///
    /// Returns the default value.
    const UniformValue& defaultValue() const;

    ///
    /// Returns whether the uniform has a default value.
    bool hasDefaultValue() const;

    ///
    /// Returns the name of the uniform.
    const std::string& name() const;

    ///
    /// Returns the compiled location.
    int location() const;

    ///
    /// Sets the compiled location.
    ///
    /// \param location The compiled location.
    void setLocation(int location);

private:
    std::string _name;

    UniformType _type;

    UniformBinding _binding;

    bool _defaultValueSet;
    UniformValue _defaultValue;

    int _location;
};

}