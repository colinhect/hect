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

using namespace hect;

Shader::Shader()
{
}

Shader::Shader(const std::string& name) :
    Asset(name)
{
}

void Shader::addModule(const ShaderModule& module)
{
    if (isUploaded())
    {
        renderer().destroyShader(*this);
    }
    _modules.push_back(module);
}

Shader::ModuleSequence Shader::modules()
{
    return ModuleSequence(_modules);
}

const Shader::ModuleSequence Shader::modules() const
{
    return ModuleSequence(_modules);
}

void Shader::addParameter(const ShaderParameter& parameter)
{
    if (isUploaded())
    {
        renderer().destroyShader(*this);
    }

    auto it = _parameterIndices.find(parameter.name());
    if (it != _parameterIndices.end())
    {
        throw Error(format("Shader already has parameter with name '%s'", parameter.name().c_str()));
    }

    unsigned textureIndex = nextTextureIndex();

    _parameterIndices[parameter.name()] = _parameters.size();
    _parameters.push_back(parameter);

    // Set the parameter's texture index if needed
    if (parameter.type() == ShaderValueType_Texture)
    {
        _parameters.back().setTextureIndex(textureIndex);
    }
}

Shader::ParameterSequence Shader::parameters()
{
    return ParameterSequence(_parameters);
}

const Shader::ParameterSequence Shader::parameters() const
{
    return ParameterSequence(_parameters);
}

const ShaderParameter& Shader::parameterWithName(const std::string& name) const
{
    auto it = _parameterIndices.find(name);
    if (it == _parameterIndices.end())
    {
        throw Error(format("Shader does not have parameter of name '%s'", name.c_str()));
    }

    return _parameters[it->second];
}

const ShaderParameter& Shader::parameterWithName(const char* name) const
{
    if (_parameterIndicesHashed.size() > 1024)
    {
        // This function is likely being used from dynamic memory locations,
        // avoid indefinitely leaking by clearing the hashed values
        _parameterIndicesHashed.clear();
    }

    const ShaderParameter* parameter = nullptr;
    auto it = _parameterIndices.find(name);
    if (it != _parameterIndices.end())
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
            _parameterIndicesHashed[name] = parameterIt->second;
        }
    }

    if (!parameter)
    {
        throw Error(format("Shader does not have parameter of name '%s'", name));
    }
    else
    {
        return *parameter;
    }
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

    // Parameter count
    if (_parameters.size() != shader._parameters.size())
    {
        return false;
    }

    // Parameters
    size_t parameterCount = _parameters.size();
    for (size_t i = 0; i < parameterCount; ++i)
    {
        if (_parameters[i] != shader._parameters[i])
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

unsigned Shader::nextTextureIndex() const
{
    unsigned maxTextureIndex = 0;

    bool firstTexture = true;
    for (const ShaderParameter& parameter : _parameters)
    {
        if (parameter.type() == ShaderValueType_Texture)
        {
            maxTextureIndex = std::max(maxTextureIndex, parameter.textureIndex());
            firstTexture = false;
        }
    }

    return firstTexture ? 0 : (maxTextureIndex + 1);
}

namespace hect
{

Decoder& operator>>(Decoder& decoder, Shader& shader)
{
    decoder >> beginObject();

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

            shader.addModule(ShaderModule(type, path, source));
        }
        decoder >> endArray();
    }

    // Parameters
    if (decoder.selectMember("parameters"))
    {
        decoder >> beginArray();
        while (decoder.hasMoreElements())
        {
            ShaderParameter parameter;

            decoder >> beginObject()
                    >> decodeValue(parameter)
                    >> endObject();

            shader.addParameter(parameter);
        }
        decoder >> endArray();
    }

    return decoder >> endObject();
}

}
