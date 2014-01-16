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
#include "Hect.h"

using namespace hect;

Shader::Shader()
{
}

Shader::Shader(const std::string& name, const AssetHandle<ShaderModule>::Array& modules, const Uniform::Array& uniforms) :
    _name(name),
    _modules(modules),
    _uniforms(uniforms)
{
}

Shader::~Shader()
{
    if (isUploaded())
    {
        renderer()->destroyShader(*this);
    }
}

const std::string& Shader::name() const
{
    return _name;
}

const AssetHandle<ShaderModule>::Array& Shader::modules() const
{
    return _modules;
}

Uniform::Array& Shader::uniforms()
{
    return _uniforms;
}

const Uniform::Array& Shader::uniforms() const
{
    return _uniforms;
}

const Uniform& Shader::uniformWithName(const std::string& name) const
{
    for (const Uniform& uniform : _uniforms)
    {
        if (uniform.name() == name)
        {
            return uniform;
        }
    }

    throw Error(format("Shader does not have uniform '%s'", name.c_str()));
}
