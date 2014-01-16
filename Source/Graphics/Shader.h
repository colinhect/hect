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
/// A GPU shader program.
class Shader :
    public RendererObject
{
public:

    ///
    /// Constructs a shader without any modules or uniforms.
    Shader();

    ///
    /// Constructs a shader given its modules and uniforms.
    ///
    /// \param name The name.
    /// \param modules The modules.
    /// \param uniforms The uniforms.
    Shader(const std::string& name, const AssetHandle<ShaderModule>::Array& modules, const Uniform::Array& uniforms);

    ///
    /// Destroys the shader if it is uploaded.
    ~Shader();

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Returns the modules.
    const AssetHandle<ShaderModule>::Array& modules() const;

    ///
    /// Returns the uniforms.
    Uniform::Array& uniforms();

    ///
    /// Returns the uniforms.
    const Uniform::Array& uniforms() const;

    ///
    /// Returns the uniform with the given name.
    ///
    /// \param name The name of the uniform.
    ///
    /// \throws Error If no uniform with the given name exists.
    const Uniform& uniformWithName(const std::string& name) const;

private:
    std::string _name;
    AssetHandle<ShaderModule>::Array _modules;
    Uniform::Array _uniforms;
};

}