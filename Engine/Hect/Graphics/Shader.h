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
#include "Hect/IO/Asset.h"
#include "Hect/IO/AssetCache.h"
#include "Hect/IO/Decoder.h"
#include "Hect/Graphics/Uniform.h"
#include "Hect/Graphics/GraphicsContext.h"
#include "Hect/Graphics/ShaderModule.h"

namespace hect
{

///
/// A GPU shader program.
class Shader :
    public Asset,
    public GraphicsContext::Object<Shader>
{
    typedef std::vector<ShaderModule> ModuleContainer;
    typedef std::vector<Uniform> UniformContainer;
public:

    ///
    /// A sequence of shader modules.
    typedef Sequence<ShaderModule, ModuleContainer> ModuleSequence;

    ///
    /// A sequence of uniforms.
    typedef Sequence<Uniform, UniformContainer> UniformSequence;

    ///
    /// Constructs a shader without any modules or uniforms.
    Shader();

    ///
    /// Constructs a shader without any modules or uniforms.
    ///
    /// \param name The name of the shader.
    Shader(const std::string& name);

    ///
    /// Adds a module to the shader.
    ///
    /// \note If the shader is uploaded to a graphics context then it will be
    /// destroyed before the module is added.
    ///
    /// \param module The module to add.
    void addModule(const ShaderModule& module);

    ///
    /// Returns the modules.
    ModuleSequence modules();

    ///
    /// Returns the modules.
    const ModuleSequence modules() const;

    ///
    /// Adds a uniform to the shader.
    ///
    /// \note If the shader is uploaded to a graphics context then it will be
    /// destroyed before the uniform is added.
    ///
    /// \param uniform The uniform to add.
    void addUniform(const Uniform& uniform);

    ///
    /// Returns the uniforms.
    UniformSequence uniforms();

    ///
    /// Returns the uniforms.
    const UniformSequence uniforms() const;

    ///
    /// Returns the uniform with the given name.
    ///
    /// \param name The name of the uniform.
    ///
    /// \throws Error If no uniform with the given name exists.
    const Uniform& uniformWithName(const std::string& name) const;

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

    friend Decoder& operator>>(Decoder& decoder, Shader& shader);

private:
    ModuleContainer _modules;
    UniformContainer _uniforms;
};

}