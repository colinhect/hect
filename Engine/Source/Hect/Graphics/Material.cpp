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

AssetHandle<Material> Material::base() const
{
    return _base;
}

void Material::setBase(const AssetHandle<Material>& base)
{
    _base = base;
}

void Material::addModule(const ShaderModule& module)
{
    if (isUploaded())
    {
        renderer().destroyMaterial(*this);
    }
    _modules.push_back(module);
}

Material::ModuleSequence Material::modules()
{
    return _modules;
}

const Material::ModuleSequence Material::modules() const
{
    return _modules;
}

void Material::addParameter(const MaterialParameter& parameter)
{
    if (_base)
    {
        throw Error("Cannot add parameter to a material with a base");
    }

    if (isUploaded())
    {
        renderer().destroyMaterial(*this);
    }

    auto it = _parameterIndices.find(parameter.name());
    if (it != _parameterIndices.end())
    {
        throw Error(format("Material already has parameter with name '%s'", parameter.name().c_str()));
    }

    unsigned textureIndex = nextTextureIndex();

    _parameterIndices[parameter.name()] = _parameters.size();
    _parameters.push_back(parameter);

    // Set the parameter's texture index if needed
    if (parameter.type() == MaterialValueType_Texture)
    {
        _parameters.back().setTextureIndex(textureIndex);
    }
}

Material::ParameterSequence Material::parameters()
{
    if (_base)
    {
        return _base->parameters();
    }
    else
    {
        return _parameters;
    }
}

const Material::ParameterSequence Material::parameters() const
{
    if (_base)
    {
        return _base->parameters();
    }
    else
    {
        return _parameters;
    }
}

const MaterialParameter& Material::parameterWithName(const std::string& name) const
{
    if (_base)
    {
        return _base->parameterWithName(name);
    }

    auto it = _parameterIndices.find(name);
    if (it == _parameterIndices.end())
    {
        throw Error(format("Material does not have parameter of name '%s'", name.c_str()));
    }

    return _parameters[it->second];
}

const MaterialParameter& Material::parameterWithName(const char* name) const
{
    if (_base)
    {
        return _base->parameterWithName(name);
    }

    if (_parameterIndicesHashed.size() > 1024)
    {
        // This function is likely being used from dynamic memory locations,
        // avoid indefinitely leaking by clearing the hashed values
        _parameterIndicesHashed.clear();
    }

    const MaterialParameter* parameter = nullptr;
    auto it = _parameterIndicesHashed.find(name);
    if (it != _parameterIndicesHashed.end())
    {
        parameter = &_parameters[it->second];
    }
    else
    {
        // The parameter was not hashed for this specific string, so attempt
        // to hash it if it exists
        auto parameterIt = _parameterIndices.find(name);
        if (parameterIt != _parameterIndices.end())
        {
            parameter = &_parameters[parameterIt->second];
            _parameterIndicesHashed[name] = parameterIt->second;
        }
    }

    if (!parameter)
    {
        throw Error(format("Material does not have parameter of name '%s'", name));
    }
    else
    {
        return *parameter;
    }
}

const Material::ArgumentSequence Material::arguments() const
{
    return _arguments;
}

void Material::addArgument(const MaterialArgument& argument)
{
    _arguments.push_back(argument);
}

void Material::clearArguments()
{
    _arguments.clear();
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

bool Material::operator==(const Material& material) const
{
    // Base
    if (_base != material._base)
    {
        return false;
    }

    // Module count
    if (_modules.size() != material._modules.size())
    {
        return false;
    }

    // Modules
    size_t moduleCount = _modules.size();
    for (size_t i = 0; i < moduleCount; ++i)
    {
        if (_modules[i] != material._modules[i])
        {
            return false;
        }
    }

    // Parameter count
    if (_parameters.size() != material._parameters.size())
    {
        return false;
    }

    // Parameters
    size_t parameterCount = _parameters.size();
    for (size_t i = 0; i < parameterCount; ++i)
    {
        if (_parameters[i] != material._parameters[i])
        {
            return false;
        }
    }

    // Argument count
    if (_arguments.size() != material._arguments.size())
    {
        return false;
    }

    // Arguments
    for (size_t i = 0; i < _arguments.size(); ++i)
    {
        if (_arguments[i] != material._arguments[i])
        {
            return false;
        }
    }

    // Render stage
    if (_renderStage != material._renderStage)
    {
        return false;
    }

    // Render state
    if (_renderState != material._renderState)
    {
        return false;
    }

    // Priority
    if (_priority != material._priority)
    {
        return false;
    }

    return true;
}

bool Material::operator!=(const Material& material) const
{
    return !(*this == material);
}

unsigned Material::nextTextureIndex() const
{
    unsigned maxTextureIndex = 0;

    bool firstTexture = true;
    for (const MaterialParameter& parameter : _parameters)
    {
        if (parameter.type() == MaterialValueType_Texture)
        {
            maxTextureIndex = std::max(maxTextureIndex, parameter.textureIndex());
            firstTexture = false;
        }
    }

    return firstTexture ? 0 : (maxTextureIndex + 1);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Material& material)
{
    return encoder << beginObject()
        << encodeVector("parameters", material._parameters)
        << encodeVector("arguments", material._arguments)
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

            // Load the base material
            material._base = decoder.assetCache().getHandle<Material>(basePath);
        }
    }

    if (!material._base)
    {
        // Modules
        if (decoder.selectMember("modules"))
        {
            decoder >> beginArray();
            while (decoder.hasMoreElements())
            {
                ShaderModuleType type;
                Path path;

                decoder >> beginObject()
                    >> decodeEnum("type", type)
                    >> decodeValue("path", path)
                    >> endObject();

                AssetCache& assetCache = decoder.assetCache();
                path = assetCache.resolvePath(path);

                auto stream = assetCache.fileSystem().openFileForRead(path);
                std::string source = stream->readAllToString();

                material._modules.push_back(ShaderModule(type, path, source));
            }
            decoder >> endArray();
        }

        // Parameters
        if (decoder.selectMember("parameters"))
        {
            decoder >> beginArray();
            while (decoder.hasMoreElements())
            {
                MaterialParameter parameter;
                decoder >> decodeValue(parameter);
                material.addParameter(parameter);
            }
            decoder >> endArray();
        }
    }

    // Arguments
    if (decoder.selectMember("arguments"))
    {
        decoder >> beginArray();
        while (decoder.hasMoreElements())
        {
            MaterialArgument argument;
            decoder >> decodeValue(argument);
            material.addArgument(argument);
        }
        decoder >> endArray();
    }

    decoder >> decodeEnum("renderStage", material._renderStage)
        >> decodeValue("renderState", material._renderState)
        >> decodeValue("priority", material._priority);

    return decoder >> endObject();
}

}
