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
#include "Shader.h"

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/IO/Encoders/ShaderEncoder.h"

using namespace hect;

Shader::Shader()
{
}

Shader::Shader(const std::string& name) :
    _name(name)
{
}

Shader::Shader(const Shader& shader) :
    RendererObject(shader),
    _name(shader._name),
    _modules(shader._modules),
    _uniforms(shader._uniforms)
{
}

Shader::Shader(Shader&& shader) :
    RendererObject(shader),
    _name(std::move(shader._name)),
    _modules(std::move(shader._modules)),
    _uniforms(std::move(shader._uniforms))
{
}

Shader::~Shader()
{
    if (isUploaded())
    {
        renderer().destroyShader(*this);
    }
}

const std::string& Shader::name() const
{
    return _name;
}

void Shader::setName(const std::string& name)
{
    _name = name;
}

void Shader::addModule(const AssetHandle<ShaderModule>& module)
{
    if (isUploaded())
    {
        // The shader will need to be re-uploaded for the change to take effect
        renderer().destroyShader(*this);
    }

    _modules.push_back(module);
}

const AssetHandle<ShaderModule>::Array& Shader::modules() const
{
    return _modules;
}

void Shader::addUniform(const Uniform& uniform)
{
    if (isUploaded())
    {
        // The shader will need to be re-uploaded for the change to take effect
        renderer().destroyShader(*this);
    }

    _uniforms.push_back(uniform);
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

void Shader::encode(ObjectEncoder& encoder) const
{
    ShaderEncoder::encode(*this, encoder);
}

void Shader::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    ShaderEncoder::decode(*this, decoder, assetCache);
}

bool Shader::operator==(const Shader& shader) const
{
    // Module count
    if (_modules.size() != shader._modules.size())
    {
        return false;
    }

    // Modules
    size_t moduleCount = _modules.size();
    for (size_t i = 0; i < moduleCount; ++i)
    {
        if (_modules[i] != shader._modules[i])
        {
            return false;
        }
    }

    // Uniform count
    if (_uniforms.size() != shader._uniforms.size())
    {
        return false;
    }

    // Uniforms
    size_t uniformCount = _uniforms.size();
    for (size_t i = 0; i < uniformCount; ++i)
    {
        if (_uniforms[i] != shader._uniforms[i])
        {
            return false;
        }
    }

    return true;
}

bool Shader::operator!=(const Shader& shader) const
{
    return !(*this == shader);
}

Shader& Shader::operator=(const Shader& shader)
{
    if (isUploaded())
    {
        // The shader will need to be re-uploaded for the change to take effect
        renderer().destroyShader(*this);
    }

    RendererObject::operator=(shader);
    
    _name = shader._name;
    _modules = shader._modules;
    _uniforms = shader._uniforms;

    return *this;
}

Shader& Shader::operator=(Shader&& shader)
{
    if (isUploaded())
    {
        // The shader will need to be re-uploaded for the change to take effect
        renderer().destroyShader(*this);
    }

    RendererObject::operator=(shader);
    
    _name = std::move(shader._name);
    _modules = std::move(shader._modules);
    _uniforms = std::move(shader._uniforms);

    return *this;
}