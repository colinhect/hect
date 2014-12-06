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
#include "Material.h"

using namespace hect;

Material::Material()
{
}

Material::Material(const std::string& name) :
    Asset(name)
{
}


void Material::prepare(Renderer& renderer)
{
    if (!_shader)
    {
        throw Error("Material does not have a shader");
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

const AssetHandle<Shader>& Material::shader() const
{
    return _shader;
}

void Material::setShader(const AssetHandle<Shader>& shader)
{
    _shader = shader;
}

const Material::ShaderArgumentSequence Material::shaderArguments() const
{
    return ShaderArgumentSequence(_shaderArguments);
}

void Material::addShaderArgument(const ShaderArgument& shaderArgument)
{
    _shaderArguments.push_back(shaderArgument);
}

void Material::clearShaderArguments()
{
    _shaderArguments.clear();
    _resolvedShaderParameters.clear();
    _resolvedFromShader = nullptr;
}

RenderStage Material::renderStage() const
{
    return _renderStage;
}

void Material::setRenderStage(RenderStage renderStage)
{
    _renderStage = renderStage;
}

const RenderState& Material::renderState() const
{
    return _renderState;
}

void Material::setRenderState(const RenderState& renderState)
{
    _renderState = renderState;
}

int Material::priority() const
{
    return _priority;
}

void Material::setPriority(int priority)
{
    _priority = priority;
}

void Material::resolveShaderParameters(Shader& shader)
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

bool Material::operator==(const Material& material) const
{
    if (_renderStage != material._renderStage)
    {
        return false;
    }

    if (_renderState != material._renderState)
    {
        return false;
    }

    if (_priority != material._priority)
    {
        return false;
    }

    if (_shader != material._shader)
    {
        return false;
    }

    if (_shaderArguments.size() != material._shaderArguments.size())
    {
        return false;
    }

    for (size_t i = 0; i < _shaderArguments.size(); ++i)
    {
        if (_shaderArguments[i] != material._shaderArguments[i])
        {
            return false;
        }
    }

    return true;
}

bool Material::operator!=(const Material& material) const
{
    return !(*this == material);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Material& material)
{
    return encoder << beginObject()
        << beginObject("shader")
        << encodeValue("path", material._shader)
        << encodeVector("arguments", material._shaderArguments)
        << endObject()
        << encodeEnum("renderStage", material._renderStage)
        << encodeValue("renderState", material._renderState)
        << encodeValue("priority", material._priority)
        << endObject();
}

Decoder& operator>>(Decoder& decoder, Material& material)
{
    decoder >> beginObject();

    if (!decoder.isBinaryStream())
    {
        // Base material
        if (decoder.selectMember("base"))
        {
            Path basePath;
            decoder >> decodeValue(basePath);

            // Start from the base material
            Material& baseMaterial = decoder.assetCache().get<Material>(basePath);
            material = baseMaterial;
        }
    }


    // Shader
    if (!decoder.isBinaryStream())
    {
        if (decoder.selectMember("shader"))
        {
            decoder >> beginObject();
            decoder >> decodeValue("path", material._shader);

            // Arguments
            if (decoder.selectMember("arguments"))
            {
                decoder >> beginArray();
                while (decoder.hasMoreElements())
                {
                    // Decode the argument
                    ShaderArgument argument;
                    decoder >> decodeValue(argument);

                    // Attempt to replace an existing argument by name
                    bool existed = false;
                    for (ShaderArgument& existingArgument : material._shaderArguments)
                    {
                        if (existingArgument.name() == argument.name())
                        {
                            material._resolvedFromShader = nullptr;

                            existingArgument = argument;
                            existed = true;
                            break;
                        }
                    }

                    // Otherwise add it as a new argument
                    if (!existed)
                    {
                        material._shaderArguments.push_back(argument);
                    }
                }

                decoder >> endArray();
            }

            decoder >> endObject();
        }
    }
    else
    {
        decoder >> beginObject()
            >> decodeValue("path", material._shader)
            >> decodeVector("arguments", material._shaderArguments)
            >> endObject();
    }

    decoder >> decodeEnum("renderStage", material._renderStage)
        >> decodeValue("renderState", material._renderState)
        >> decodeValue("priority", material._priority);

    return decoder >> endObject();
}

}
