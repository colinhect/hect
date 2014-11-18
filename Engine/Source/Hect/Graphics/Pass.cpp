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
#include "Pass.h"

#include "Hect/Graphics/Renderer.h"

using namespace hect;

void Pass::prepare(Renderer& renderer)
{
    if (!_shader)
    {
        throw Error("Pass does not have a shader");
    }

    // If the shader changed or the shader parameters have not yet been
    // resolved
    Shader& shader = *_shader;
    if (_resolvedFromShader != &shader)
    {
        resolveShaderParameters(shader);
    }

    // Bind the render state
    renderer.bindState(_renderState);

    // Bind the textures in the pass
    unsigned textureIndex = 0;
    for (const AssetHandle<Texture>& texture : _textures)
    {
        renderer.bindTexture(*texture, textureIndex);
        ++textureIndex;
    }

    // Bind the shader
    renderer.bindShader(shader);

    // Set the shader arguments values
    for (auto& pair : _resolvedShaderParameters)
    {
        const ShaderParameter& parameter = *pair.first;
        const ShaderValue& value = pair.second;
        renderer.bindShaderParameter(parameter, value);
    }
}

const AssetHandle<Shader>& Pass::shader() const
{
    return _shader;
}

void Pass::setShader(const AssetHandle<Shader>& shader)
{
    _shader = shader;
}

const Pass::ShaderArgumentSequence Pass::shaderArguments() const
{
    return ShaderArgumentSequence(_shaderArguments);
}

void Pass::addShaderArgument(const ShaderArgument& shaderArgument)
{
    _shaderArguments.push_back(shaderArgument);
}

const Pass::TextureSequence Pass::textures() const
{
    return TextureSequence(_textures);
}

void Pass::addTexture(const AssetHandle<Texture>& texture)
{
    _textures.push_back(texture);
}

void Pass::clearTextures()
{
    _textures.clear();
}

const RenderState& Pass::renderState() const
{
    return _renderState;
}

void Pass::setRenderState(const RenderState& renderState)
{
    _renderState = renderState;
}

void Pass::resolveShaderParameters(Shader& shader)
{
    _resolvedShaderParameters.clear();

    // Resolve the parameters that the arguments refer to (this would be
    // invalidated if the shader changes)
    for (const ShaderArgument& argument : _shaderArguments)
    {
        const ShaderParameter& parameter = shader.parameterWithName(argument.name());
        _resolvedShaderParameters[&parameter] = argument.value();
    }

    _resolvedFromShader = &shader;
}

bool Pass::operator==(const Pass& pass) const
{
    if (_renderState != pass._renderState)
    {
        return false;
    }

    if (_textures.size() != pass._textures.size())
    {
        return false;
    }

    for (size_t i = 0; i < _textures.size(); ++i)
    {
        if (_textures[i] != pass._textures[i])
        {
            return false;
        }
    }

    if (_shader != pass._shader)
    {
        return false;
    }

    if (_shaderArguments.size() != pass._shaderArguments.size())
    {
        return false;
    }

    for (size_t i = 0; i < _shaderArguments.size(); ++i)
    {
        if (_shaderArguments[i] != pass._shaderArguments[i])
        {
            return false;
        }
    }

    return true;
}

bool Pass::operator!=(const Pass& pass) const
{
    return !(*this == pass);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Pass& pass)
{
    return encoder << beginObject()
           << beginObject("shader")
           << encodeValue("path", pass.shader())
           << encodeVector("arguments", pass._shaderArguments)
           << endObject()
           << encodeVector("textures", pass._textures)
           << encodeValue("renderState", pass._renderState)
           << endObject();
}

Decoder& operator>>(Decoder& decoder, Pass& pass)
{
    return decoder >> beginObject()
           >> beginObject("shader")
           >> decodeValue("path", pass._shader, true)
           >> decodeVector("arguments", pass._shaderArguments)
           >> endObject()
           >> decodeVector("textures", pass._textures)
           >> decodeValue("renderState", pass._renderState)
           >> endObject();
}

}
