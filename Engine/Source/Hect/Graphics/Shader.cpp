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

using namespace hect;

Shader::ModuleSequence Shader::modules()
{
    return _modules;
}

const Shader::ModuleSequence Shader::modules() const
{
    return _modules;
}

Shader::UniformSequence Shader::uniforms()
{
    return _uniforms;
}

const Shader::UniformSequence Shader::uniforms() const
{
    return _uniforms;
}

const Uniform& Shader::uniform(const std::string& name) const
{
    auto it = _uniformIndices.get(name);
    if (!it)
    {
        throw Error(format("Shader does not have a uniform named '%s'", name.c_str()));
    }
    return _uniforms[*it];
}

const Uniform& Shader::uniform(const char* name) const
{
    auto it = _uniformIndices.get(name);
    if (!it)
    {
        throw Error(format("Shader does not have a uniform named '%s'", name));
    }
    return _uniforms[*it];
}

const BlendMode& Shader::blendMode() const
{
    return _blendMode;
}

bool Shader::isDepthTested() const
{
    return _depthTested;
}

void Shader::setDepthTested(bool depthTested)
{
    _depthTested = depthTested;
}

bool Shader::isOneSided() const
{
    return _oneSided;
}

void Shader::setOneSided(bool oneSided)
{
    _oneSided = oneSided;
}

int Shader::priority() const
{
    return _priority;
}

void Shader::setPriority(int priority)
{
    _priority = priority;
}

bool Shader::operator==(const Shader& shader) const
{
    // Module count
    if (_modules.size() != shader._modules.size())
    {
        return false;
    }

    // Modules
    for (size_t i = 0; i < _modules.size(); ++i)
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
    for (size_t i = 0; i < _uniforms.size(); ++i)
    {
        if (_uniforms[i] != shader._uniforms[i])
        {
            return false;
        }
    }

    // Blend mode
    if (_blendMode != shader._blendMode)
    {
        return false;
    }

    // Depth tested
    if (_depthTested != shader._depthTested)
    {
        return false;
    }

    // One sided
    if (_oneSided != shader._oneSided)
    {
        return false;
    }

    // Priority
    if (_priority != shader._priority)
    {
        return false;
    }

    return true;
}

bool Shader::operator!=(const Shader& shader) const
{
    return !(*this == shader);
}

void Shader::resolveUniforms()
{
    size_t textureIndex = 0;
    for (size_t i = 0; i < _uniforms.size(); ++i)
    {
        Uniform& uniform = _uniforms[i];
        _uniformIndices.set(uniform.name(), i);
        if (uniform.type() == UniformType_Texture)
        {
            uniform.setTextureIndex(textureIndex);
            ++textureIndex;
        }
    }
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Shader& shader)
{
    encoder << beginObject()
            << encodeVector("modules", shader._modules)
            << encodeVector("uniforms", shader._uniforms)
            << encodeValue("depthTested", shader._depthTested)
            << encodeValue("oneSided", shader._oneSided)
            << encodeValue("blendMode", shader._blendMode)
            << encodeValue("priority", shader._priority)
            << endObject();

    return encoder;
}

Decoder& operator>>(Decoder& decoder, Shader& shader)
{
    decoder >> beginObject()
            >> decodeVector("modules", shader._modules, true)
            >> decodeVector("uniforms", shader._uniforms)
            >> decodeValue("depthTested", shader._depthTested)
            >> decodeValue("oneSided", shader._oneSided)
            >> decodeValue("blendMode", shader._blendMode)
            >> decodeValue("priority", shader._priority)
            >> endObject();

    shader.resolveUniforms();

    return decoder;
}

}

    