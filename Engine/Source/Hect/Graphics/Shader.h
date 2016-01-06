///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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

#include "Hect/Core/Export.h"
#include "Hect/Core/Sequence.h"
#include "Hect/Core/StringMap.h"
#include "Hect/IO/Asset.h"
#include "Hect/Graphics/BlendMode.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/ShaderModule.h"
#include "Hect/Graphics/RenderStage.h"
#include "Hect/Graphics/Uniform.h"

namespace hect
{

///
/// A shader program.
///
/// Shaders are programs executed directly on the GPU.  The input to a shader
/// is the combination of the selected mesh's vertices and the values set for
/// shader's uniforms.  The output from a shader is the output to the selected
/// render target.
class HECT_EXPORT Shader :
    public Asset<Shader>,
    public Renderer::Object<Shader>
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
    /// Constructs a blank shader.
    Shader();

    ///
    /// Constructs a blank shader.
    ///
    /// \param name The name of the shader.
    Shader(const std::string& name);

    ///
    /// Returns the render stage.
    RenderStage renderStage() const;

    ///
    /// Sets the render stage.
    ///
    /// \param renderStage The new render stage.
    void setRenderStage(RenderStage renderStage);

    ///
    /// Adds a module to the shader.
    ///
    /// \note If the shader is uploaded then it will be destroyed before the
    /// the module is added.
    ///
    /// \param module The module to add.
    void addModule(const ShaderModule& module);

    ///
    /// Returns the shader sources.
    const ModuleSequence modules() const;

    ///
    /// Adds a uniform to the shader.
    ///
    /// \note If the shader is uploaded then it will be destroyed before the
    /// the uniform is added.
    ///
    /// \param uniform The uniform to add.
    ///
    /// \returns The index of the added uniform.
    UniformIndex addUniform(const Uniform& uniform);

    ///
    /// Returns the uniforms.
    UniformSequence uniforms();

    ///
    /// Returns the uniforms.
    const UniformSequence uniforms() const;

    ///
    /// Returns the uniform at the specified index.
    ///
    /// \param index The index of the uniform.
    ///
    /// \throws InvalidOperation If no uniform exists at the specified index.
    Uniform& uniform(UniformIndex index);

    ///
    /// \copydoc hect::Shader::uniform(UniformIndex)
    const Uniform& uniform(UniformIndex index) const;

    ///
    /// Returns the uniform of the specified name.
    ///
    /// \param name The name of the uniform.
    ///
    /// \throws InvalidOperation If no uniform with the specified name exists.
    Uniform& uniform(const std::string& name);

    ///
    /// \copydoc hect::Shader::uniform(const std::string&)
    const Uniform& uniform(const std::string& name) const;

    ///
    /// Returns the uniform of the specified name.
    ///
    /// \param name The name of the uniform.
    ///
    /// \throws InvalidOperation If no uniform with the specified name exists.
    Uniform& uniform(const char* name);

    ///
    /// \copydoc hect::Shader::uniform(const char*)
    const Uniform& uniform(const char* name) const;

    ///
    /// Returns whether the shader has a uniform of the specified name.
    ///
    /// \param name The name of the uniform.
    ///
    /// \returns True if the shader has a uniform of the specified name; false
    /// otherwise.
    bool hasUniform(const std::string& name) const;

    ///
    /// Returns the blend mode that the shader uses.
    const BlendMode& blendMode() const;

    ///
    /// Sets the blend mode that the shader uses.
    ///
    /// \param blendMode The blend mode.
    void setBlendMode(const BlendMode& blendMode);

    ///
    /// Returns whether the shader uses depth testing.
    bool isDepthTested() const;

    ///
    /// Sets whether the shader uses depth testing.
    ///
    /// \param depthTested True if depth testing is used; false otherwise.
    void setDepthTested(bool depthTested);

    ///
    /// Returns the render priority affecting the order in which the shader is
    /// rendered within a render flow.
    int priority() const;

    ///
    /// Sets the render priority affecting the order in which the shader is
    /// rendered within a render flow.
    ///
    /// \param priority The priority.
    void setPriority(int priority);

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

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    void resolveUniforms();

    RenderStage _renderStage { RenderStage::None };

    ModuleContainer _modules;
    UniformContainer _uniforms;
    StringMap<UniformIndex> _uniformIndices;

    BlendMode _blendMode;
    bool _depthTested { true };

    int _priority { 0 };
};

}