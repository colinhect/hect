///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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

Shader::Shader()
{
}

Shader::Shader(const std::string& name) :
    Asset(name)
{
}

RenderStage Shader::renderStage() const
{
    return _renderStage;
}

void Shader::setRenderStage(RenderStage renderStage)
{
    _renderStage = renderStage;
}

void Shader::addModule(const ShaderModule& module)
{
    // Destroy the shader if it is uploaded so that any changes the added
    // module contributes to the shader when it is compiled will take effect
    if (isUploaded())
    {
        renderer().destroyShader(*this);
    }

    _modules.push_back(module);
}

const Shader::ModuleSequence Shader::modules() const
{
    return _modules;
}

UniformIndex Shader::addUniform(const Uniform& uniform)
{
    // Destroy the shader if it is uploaded so that the uniforms location can
    // be established when the shader is uploaded
    if (isUploaded())
    {
        renderer().destroyShader(*this);
    }

    _uniforms.push_back(uniform);
    resolveUniforms();

    return _uniforms.size() - 1;
}

Shader::UniformSequence Shader::uniforms()
{
    return _uniforms;
}

const Shader::UniformSequence Shader::uniforms() const
{
    return _uniforms;
}

Uniform& Shader::uniform(UniformIndex index)
{
    if (index >= _uniforms.size())
    {
        throw InvalidOperation(format("Shader does not have a uniform at index %i", index));
    }
    return _uniforms[index];
}

const Uniform& Shader::uniform(UniformIndex index) const
{
    return const_cast<Shader*>(this)->uniform(index);
}

Uniform& Shader::uniform(const std::string& name)
{
    auto it = _uniformIndices.get(name);
    if (!it)
    {
        throw InvalidOperation(format("Shader does not have a uniform named '%s'", name.c_str()));
    }
    return _uniforms[*it];
}

const Uniform& Shader::uniform(const std::string& name) const
{
    return const_cast<Shader*>(this)->uniform(name);
}

Uniform& Shader::uniform(const char* name)
{
    auto it = _uniformIndices.get(name);
    if (!it)
    {
        throw InvalidOperation(format("Shader does not have a uniform named '%s'", name));
    }
    return _uniforms[*it];
}

const Uniform& Shader::uniform(const char* name) const
{
    return const_cast<Shader*>(this)->uniform(name);
}

bool Shader::hasUniform(const std::string& name) const
{
    auto it = _uniformIndices.get(name);
    return static_cast<bool>(it);
}

const BlendMode& Shader::blendMode() const
{
    return _blendMode;
}

void Shader::setBlendMode(const BlendMode& blendMode)
{
    _blendMode = blendMode;
}

bool Shader::isDepthTested() const
{
    return _depthTested;
}

void Shader::setDepthTested(bool depthTested)
{
    _depthTested = depthTested;
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

void Shader::encode(Encoder& encoder) const
{
    encoder << encodeEnum("renderStage", _renderStage)
            << encodeVector("modules", _modules)
            << encodeVector("uniforms", _uniforms)
            << encodeValue("depthTested", _depthTested)
            << encodeValue("blendMode", _blendMode)
            << encodeValue("priority", _priority);
}

void Shader::decode(Decoder& decoder)
{
    // Base
    if (!decoder.isBinaryStream())
    {
        if (decoder.selectMember("base"))
        {
            Path basePath;
            decoder >> decodeValue(basePath);

            try
            {
                AssetDecoder baseDecoder(decoder.assetCache(), basePath);
                baseDecoder >> decodeValue(*this);
            }
            catch (const Exception& exception)
            {
                throw DecodeError(format("Failed to load base shader '%s': %s", basePath.asString().c_str(), exception.what()));
            }
        }
    }

    decoder >> decodeEnum("renderStage", _renderStage)
            >> decodeVector("modules", _modules, true)
            >> decodeVector("uniforms", _uniforms)
            >> decodeValue("depthTested", _depthTested)
            >> decodeValue("blendMode", _blendMode)
            >> decodeValue("priority", _priority);

    resolveUniforms();
}

void Shader::resolveUniforms()
{
    size_t textureIndex = 0;
    for (UniformIndex index = 0; index < _uniforms.size(); ++index)
    {
        Uniform& uniform = _uniforms[index];
        uniform._index = index;

        _uniformIndices.set(uniform._name, index);
        if (uniform.type() == UniformType::Texture2 ||
			uniform.type() == UniformType::TextureCube)
        {
            uniform._textureIndex = textureIndex;
            ++textureIndex;
        }
    }
}
