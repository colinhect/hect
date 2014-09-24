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

#include "Hect/Core/Sequence.h"
#include "Hect/IO/AssetHandle.h"
#include "Hect/IO/Encodable.h"
#include "Hect/Graphics/Uniform.h"
#include "Hect/Graphics/ShaderModule.h"

namespace hect
{

///
/// A GPU shader program.
class Shader :
    public RendererObject,
    public Encodable
{
public:

    ///
    /// Constructs a shader without any modules or uniforms.
    Shader();

    ///
    /// Constructs a shader without any modules or uniforms.
    ///
    /// \param name The name of the shader.
    Shader(const std::string& name);

    ///
    /// Constructs a shader as a copy of another.
    ///
    /// \param shader The shader to copy.
    Shader(const Shader& shader);

    ///
    /// Constructs a shader moved from another.
    ///
    /// \param shader The shader to move.
    Shader(Shader&& shader);

    ///
    /// Destroys the shader if it is uploaded.
    ~Shader();

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Sets the name.
    ///
    /// \param name The new name.
    void setName(const std::string& name);

    ///
    /// Adds a module to the shader.
    ///
    /// \note If the shader is uploaded to a renderer then it will be
    /// destroyed before the module is added.
    ///
    /// \param module The module to add.
    void addModule(const AssetHandle<ShaderModule>& module);

    ///
    /// Returns the modules.
    ConstSequence<AssetHandle<ShaderModule>> modules() const;

    ///
    /// Adds a uniform to the shader.
    ///
    /// \note If the shader is uploaded to a renderer then it will be
    /// destroyed before the uniform is added.
    ///
    /// \param uniform The uniform to add.
    void addUniform(const Uniform& uniform);

    ///
    /// Returns the uniforms.
    Sequence<Uniform> uniforms();

    ///
    /// Returns the uniforms.
    ConstSequence<Uniform> uniforms() const;

    ///
    /// Returns the uniform with the given name.
    ///
    /// \param name The name of the uniform.
    ///
    /// \throws Error If no uniform with the given name exists.
    const Uniform& uniformWithName(const std::string& name) const;

    void encode(Encoder& encoder) const;
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

    ///
    /// Returns whether the shader is equivalent to another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param shader The other shader.
    bool operator==(const Shader& shader) const;

    ///
    /// Returns whether the shader is different from another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param shader The other shader.
    bool operator!=(const Shader& shader) const;

    ///
    /// Replaces the shader with a copy of another.
    ///
    /// \note If the shader was uploaded to a renderer then it will be destroyed
    /// before assigned.
    ///
    /// \param shader The shader to copy.
    ///
    /// \returns A reference to the shader.
    Shader& operator=(const Shader& shader);

    ///
    /// Replaces the shader by moving another.
    ///
    /// \note If the shader was uploaded to a renderer then it will be destroyed
    /// before assigned.
    ///
    /// \param shader The shader to move.
    ///
    /// \returns A reference to the shader.
    Shader& operator=(Shader&& shader);

private:
    std::string _name;
    std::vector<AssetHandle<ShaderModule>> _modules;
    std::vector<Uniform> _uniforms;
};

}