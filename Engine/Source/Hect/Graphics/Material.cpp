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
    if (isUploaded())
    {
        renderer().destroyMaterial(*this);
    }
    _base = base;
}

MaterialType Material::type() const
{
    if (!_typeSet && _base)
    {
        return _base->type();
    }
    else
    {
        return _type;
    }
}

void Material::setType(MaterialType type)
{
    _typeSet = true;
    _type = type;
}

bool Material::blend() const
{
    if ((_activeFlagsMask & MaterialFlag_Blend) == 0 && _base)
    {
        return _base->blend();
    }
    else
    {
        return (_flags & MaterialFlag_Blend) == MaterialFlag_Blend;
    }
}

void Material::setBlend(bool value)
{
    _activeFlagsMask |= MaterialFlag_Blend;
    if (value)
    {
        _flags |= MaterialFlag_Blend;
    }
    else
    {
        _flags &= ~MaterialFlag_Blend;
    }
}

void Material::setBlendFactors(BlendFactor source, BlendFactor destination)
{
    _blendFactorsSet = true;
    _sourceFactor = source;
    _destinationFactor = destination;
}

BlendFactor Material::sourceBlendFactor() const
{
    if (!_blendFactorsSet && _base)
    {
        return _base->sourceBlendFactor();
    }
    else
    {
        return _sourceFactor;
    }
}

BlendFactor Material::destinationBlendFactor() const
{
    if (!_blendFactorsSet && _base)
    {
        return _base->destinationBlendFactor();
    }
    else
    {
        return _destinationFactor;
    }
}

bool Material::depthTest() const
{
    if ((_activeFlagsMask & MaterialFlag_NoDepthTest) == 0 && _base)
    {
        return _base->depthTest();
    }
    else
    {
        return (_flags & MaterialFlag_NoDepthTest) != MaterialFlag_NoDepthTest;
    }
}

void Material::setDepthTest(bool value)
{
    _activeFlagsMask |= MaterialFlag_NoDepthTest;
    if (value)
    {
        _flags &= ~MaterialFlag_NoDepthTest;
    }
    else
    {
        _flags |= MaterialFlag_NoDepthTest;
    }
}

bool Material::depthWrite() const
{
    if ((_activeFlagsMask & MaterialFlag_DepthWrite) == 0 && _base)
    {
        return _base->depthTest();
    }
    else
    {
        return (_flags & MaterialFlag_DepthWrite) == MaterialFlag_DepthWrite;
    }
}

void Material::setDepthWrite(bool value)
{
    _activeFlagsMask |= MaterialFlag_DepthWrite;
    if (value)
    {
        _flags |= MaterialFlag_DepthWrite;
    }
    else
    {
        _flags &= ~MaterialFlag_DepthWrite;
    }
}

bool Material::cullBackFace() const
{
    if ((_activeFlagsMask & MaterialFlag_NoCullBackFace) == 0 && _base)
    {
        return _base->depthTest();
    }
    else
    {
        return (_flags & MaterialFlag_NoCullBackFace) != MaterialFlag_NoCullBackFace;
    }
}

void Material::setCullBackFace(bool value)
{
    _activeFlagsMask |= MaterialFlag_NoCullBackFace;
    if (value)
    {
        _flags &= ~MaterialFlag_NoCullBackFace;
    }
    else
    {
        _flags |= MaterialFlag_NoCullBackFace;
    }
}

int Material::priority() const
{
    if (!_prioritySet && _base)
    {
        return _base->priority();
    }
    else
    {
        return _priority;
    }
}

void Material::setPriority(int priority)
{
    _prioritySet = true;
    _priority = priority;
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

const MaterialParameter& Material::parameter(const std::string& name) const
{
    if (_base)
    {
        return _base->parameter(name);
    }
    else
    {
        auto it = _parameterIndices.get(name);
        if (it)
        {
            return _parameters[*it];
        }
        else
        {
            throw Error(format("Material does not have parameter of name '%s'", name.c_str()));
        }
    }
}

const MaterialParameter& Material::parameter(const char* name) const
{
    if (_base)
    {
        return _base->parameter(name);
    }
    else
    {
        auto it = _parameterIndices.get(name);
        if (it)
        {
            return _parameters[*it];
        }
        else
        {
            throw Error(format("Material does not have parameter of name '%s'", name));
        }
    }
}

void Material::setArgument(const MaterialParameter& parameter, MaterialValue value)
{
    size_t index = parameter._index;
    if (index >= _parameterValues.size())
    {
        _parameterValues.resize(index + 1);
    }
    _parameterValues[index] = value;
}

void Material::setArgument(const std::string& parameterName, MaterialValue value)
{
    const MaterialParameter& parameter = this->parameter(parameterName);
    setArgument(parameter, value);
}

void Material::setArgument(const char* parameterName, MaterialValue value)
{
    const MaterialParameter& parameter = this->parameter(parameterName);
    setArgument(parameter, value);
}

const MaterialValue& Material::argumentForParameter(const MaterialParameter& parameter) const
{
    size_t index = parameter._index;
    if (index < _parameterValues.size())
    {
        const MaterialValue& value = _parameterValues[index];
        if (value.type() == MaterialValueType_Null && _base)
        {
            return _base->argumentForParameter(parameter);
        }
        else
        {
            return value;
        }
    }
    else if (_base)
    {
        return _base->argumentForParameter(parameter);
    }

    throw Error("No material parameter at the specified index");
}

Material::ShaderModuleSequence Material::shaderModules()
{
    return _shaderModules;
}

const Material::ShaderModuleSequence Material::shaderModules() const
{
    return _shaderModules;
}

bool Material::operator==(const Material& material) const
{
    (void)material;
    return false;
}

bool Material::operator!=(const Material& material) const
{
    return !(*this == material);
}

void Material::addParameter(const std::string& name, MaterialValueType type, MaterialParameterBinding binding)
{
    if (_base)
    {
        throw Error("Cannot add a parameter to a material with a base");
    }

    if (isUploaded())
    {
        renderer().destroyMaterial(*this);
    }

    auto it = _parameterIndices.get(name);
    if (it)
    {
        throw Error(format("Material already has parameter with name '%s'", name.c_str()));
    }

    size_t index = _parameters.size();
    unsigned textureIndex = nextTextureIndex();

    _parameters.push_back(MaterialParameter(index, textureIndex, name, type, binding));
    _parameterValues.push_back(MaterialValue());
    _parameterIndices.set(name, index);
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
    (void)encoder;
    (void)material;
    throw Error("Unsupported");
}

Decoder& operator>>(Decoder& decoder, Material& material)
{
    // Destroy the material if it is already uploaded
    if (material.isUploaded())
    {
        material.renderer().destroyMaterial(material);
    }

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

                material._shaderModules.push_back(ShaderModule(type, path, source));
            }
            decoder >> endArray();
        }

        // Parameters
        if (decoder.selectMember("parameters"))
        {
            decoder >> beginArray();
            while (decoder.hasMoreElements())
            {
                decoder >> beginObject();

                std::string name;
                MaterialValueType type = MaterialValueType_Null;
                MaterialParameterBinding binding = MaterialParameterBinding_None;

                decoder >> decodeValue("name", name, true)
                        >> decodeEnum("type", type, false)
                        >> decodeEnum("binding", binding, false);

                decoder >> endObject();

                material.addParameter(name, type, binding);
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
            decoder >> beginObject();

            std::string name;
            decoder >> decodeValue("name", name, true);

            const MaterialParameter& parameter = material.parameter(name);

            MaterialValue value(parameter.type());
            decoder >> decodeValue(value);
            material.setArgument(parameter, value);

            decoder >> endObject();
        }
        decoder >> endArray();
    }

    if (decoder.isBinaryStream())
    {
        decoder >> decodeEnum(material._type)
                >> decodeValue(material._typeSet)
                >> decodeValue(material._flags)
                >> decodeValue(material._activeFlagsMask)
                >> decodeEnum(material._sourceFactor)
                >> decodeEnum(material._destinationFactor)
                >> decodeValue(material._blendFactorsSet)
                >> decodeValue(material._priority)
                >> decodeValue(material._prioritySet);
    }
    else
    {
        // Type
        if (decoder.selectMember("type"))
        {
            MaterialType type;
            decoder >> decodeEnum(type);
            material.setType(type);
        }

        // Blend enabled
        if (decoder.selectMember("blend"))
        {
            bool value;
            decoder >> decodeValue(value);
            material.setBlend(value);
        }

        // Blend factors
        if (decoder.selectMember("blendFactors"))
        {
            decoder >> beginObject();

            BlendFactor source = BlendFactor_One;
            BlendFactor destination = BlendFactor_One;

            decoder >> decodeEnum("source", source)
                    >> decodeEnum("destination", destination);

            material.setBlendFactors(source, destination);

            decoder >> endObject();
        }

        // Depth testing enabled
        if (decoder.selectMember("depthTest"))
        {
            bool value;
            decoder >> decodeValue(value);
            material.setDepthTest(value);
        }

        // Depth writing enabled
        if (decoder.selectMember("depthWrite"))
        {
            bool value;
            decoder >> decodeValue(value);
            material.setDepthWrite(value);
        }

        // Back face culling enabled
        if (decoder.selectMember("cullBackFace"))
        {
            bool value;
            decoder >> decodeValue(value);
            material.setCullBackFace(value);
        }

        // Priority
        if (decoder.selectMember("priority"))
        {
            int value;
            decoder >> decodeValue(value);
            material.setPriority(value);
        }
    }

    decoder >> endObject();
    return decoder;
}

}
