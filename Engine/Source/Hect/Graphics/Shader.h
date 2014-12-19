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

#include "Hect/Core/Export.h"
#include "Hect/Core/Sequence.h"
#include "Hect/Core/StringMap.h"
#include "Hect/IO/Asset.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"
#include "Hect/Graphics/BlendMode.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/ShaderModule.h"
#include "Hect/Graphics/Uniform.h"

namespace hect
{

class HECT_EXPORT Shader :
    public Asset,
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
    /// Returns the shader modules.
    ModuleSequence modules();

    ///
    /// Returns the shader sources.
    const ModuleSequence modules() const;

    UniformSequence uniforms();
    const UniformSequence uniforms() const;

    ///
    /// Returns the uniform of the specified name.
    ///
    /// \param name The name of the uniform.
    ///
    /// \throws Error If no parameter with the specified name exists.
    const Uniform& uniform(const std::string& name) const;

    ///
    /// \copydoc Shader::uniform()
    const Uniform& uniform(const char* name) const;

    const BlendMode& blendMode() const;

    bool isDepthTested() const;
    void setDepthTested(bool depthTested);

    bool isOneSided() const;
    void setOneSided(bool oneSided);

    ///
    /// Returns the render priority.
    int priority() const;

    ///
    /// Sets the render priority.
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

    friend HECT_EXPORT Encoder& operator<<(Encoder& encoder, const Shader& shader);
    friend HECT_EXPORT Decoder& operator>>(Decoder& decoder, Shader& shader);

private:
    void resolveUniforms();

    ModuleContainer _modules;
    UniformContainer _uniforms;
    StringMap<size_t> _uniformIndices;

    BlendMode _blendMode;
    bool _depthTested { true };
    bool _oneSided { true };

    int _priority { 0 };
};

}