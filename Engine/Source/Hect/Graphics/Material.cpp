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
    if (!isFlagActive(MaterialFlag_Blend) && _base)
    {
        return _base->blend();
    }
    else
    {
        return flagValue(MaterialFlag_Blend);
    }
}

void Material::setBlend(bool value)
{
    setFlagValue(MaterialFlag_Blend, value);
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
    if (!isFlagActive(MaterialFlag_NoDepthTest) && _base)
    {
        return _base->depthTest();
    }
    else
    {
        return !flagValue(MaterialFlag_NoDepthTest);
    }
}

void Material::setDepthTest(bool value)
{
    setFlagValue(MaterialFlag_NoDepthTest, !value);
}

bool Material::depthWrite() const
{
    if (!isFlagActive(MaterialFlag_NoDepthWrite) && _base)
    {
        return _base->depthTest();
    }
    else
    {
        return !flagValue(MaterialFlag_NoDepthWrite);
    }
}

void Material::setDepthWrite(bool value)
{
    setFlagValue(MaterialFlag_NoDepthWrite, !value);
}

bool Material::cullBackFace() const
{
    if (!isFlagActive(MaterialFlag_NoCullBackFace) && _base)
    {
        return _base->depthTest();
    }
    else
    {
        return !flagValue(MaterialFlag_NoCullBackFace);
    }
}

void Material::setCullBackFace(bool value)
{
    setFlagValue(MaterialFlag_NoCullBackFace, !value);
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

Material::ShaderSourceSequence Material::shaderSources()
{
    return _shaderSources;
}

const Material::ShaderSourceSequence Material::shaderSources() const
{
    return _shaderSources;
}

bool Material::operator==(const Material& material) const
{
    // Compare base material
    if (_base != material._base)
    {
        return false;
    }

    // Compare shader source count
    if (_shaderSources.size() != material._shaderSources.size())
    {
        return false;
    }

    // Compare shader sources
    for (size_t i = 0; i < _shaderSources.size(); ++i)
    {
        if (_shaderSources[i] != material._shaderSources[i])
        {
            return false;
        }
    }

    // Compare parameter count
    if (_parameters.size() != material._parameters.size())
    {
        return false;
    }

    // Compare parameters
    for (size_t i = 0; i < _parameters.size(); ++i)
    {
        if (_parameters[i] != material._parameters[i])
        {
            return false;
        }
    }

    // Compare argument count
    if (_parameterValues.size() != material._parameterValues.size())
    {
        return false;
    }

    // Compare arguments
    for (size_t i = 0; i < _parameterValues.size(); ++i)
    {
        if (_parameterValues[i] != material._parameterValues[i])
        {
            return false;
        }
    }

    // Compare type
    if (_typeSet && _type != material._type)
    {
        return false;
    }

    // Compare blending factors
    if (_blendFactorsSet && (_sourceFactor != material._sourceFactor || _destinationFactor != material._destinationFactor))
    {
        return false;
    }

    // Compare active flags
    if (_activeFlagsMask != material._activeFlagsMask)
    {
        return false;
    }

    // Compare flags
    if (_flags != material._flags)
    {
        return false;
    }

    return true;
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

bool Material::isFlagActive(MaterialFlag flag) const
{
    return (_activeFlagsMask & flag) == flag;
}

void Material::setFlagActive(MaterialFlag flag)
{
    _activeFlagsMask |= flag;
}

bool Material::flagValue(MaterialFlag flag) const
{
    return (_flags & flag) == flag;
}

void Material::setFlagValue(MaterialFlag flag, bool value)
{
    setFlagActive(flag);
    if (value)
    {
        _flags |= flag;
    }
    else
    {
        _flags &= ~flag;
    }
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Material& material)
{
    encoder << beginObject();

    // Encode base material path
    if (material._base || encoder.isBinaryStream())
    {
        Path basePath;
        if (material._base)
        {
            basePath = material._base.path();
        }
        encoder << encodeValue("base", basePath);
    }

    // Encode shader sources
    encoder << beginArray("shaderSources");
    for (const ShaderSource& shaderSource : material._shaderSources)
    {
        encoder << beginObject()
                << encodeEnum("type", shaderSource.type())
                << encodeValue("path", shaderSource.path())
                << endObject();
    }
    encoder << endArray();

    // Encode parameters
    encoder << beginArray("parameters");
    for (const MaterialParameter& parameter : material._parameters)
    {
        encoder << beginObject()
                << encodeValue("name", parameter.name())
                << encodeEnum("type", parameter.type())
                << encodeEnum("binding", parameter.binding())
                << endObject();
    }
    encoder << endArray();

    // Encode arguments
    encoder << beginArray("arguments");
    for (size_t index = 0; index < material._parameterValues.size(); ++index)
    {
        const MaterialValue& value = material._parameterValues[index];
        if (value.type() != MaterialValueType_Null)
        {
            const MaterialParameter& parameter = material._parameters[index];
            encoder << beginObject()
                    << encodeValue("name", parameter.name())
                    << encodeValue("value", value)
                    << endObject();
        }
    }
    encoder << endArray();

    if (encoder.isBinaryStream())
    {
        // Encode all properties
        encoder << encodeEnum(material._type)
                << encodeValue(material._typeSet)
                << encodeValue(material._flags)
                << encodeValue(material._activeFlagsMask)
                << encodeEnum(material._sourceFactor)
                << encodeEnum(material._destinationFactor)
                << encodeValue(material._blendFactorsSet)
                << encodeValue(material._priority)
                << encodeValue(material._prioritySet);
    }
    else
    {
        // Encode type
        if (material._typeSet)
        {
            encoder << encodeEnum("type", material._type);
        }

        // Encode if blending is enabled
        if (material.isFlagActive(Material::MaterialFlag_Blend))
        {
            encoder << encodeValue("blend", material.blend());
        }

        // Encode source and destination blend factors
        if (material._blendFactorsSet)
        {
            encoder << beginObject("blendFactors")
                    << encodeEnum("source", material.sourceBlendFactor())
                    << encodeEnum("destination", material.destinationBlendFactor())
                    << endObject();
        }

        // Encode if depth testing is enabled
        if (material.isFlagActive(Material::MaterialFlag_NoDepthTest))
        {
            encoder << encodeValue("depthTest", material.depthTest());
        }

        // Encode if depth writing is enabled
        if (material.isFlagActive(Material::MaterialFlag_NoDepthWrite))
        {
            encoder << encodeValue("depthWrite", material.depthWrite());
        }

        // Encode if back face culling is enabled
        if (material.isFlagActive(Material::MaterialFlag_NoCullBackFace))
        {
            encoder << encodeValue("cullBackFace", material.cullBackFace());
        }

        // Encode priority
        if (material._prioritySet)
        {
            encoder << encodeValue("priority", material.priority());
        }
    }

    encoder << endObject();
    return encoder;
}

Decoder& operator>>(Decoder& decoder, Material& material)
{
    // Destroy the material if it is already uploaded
    if (material.isUploaded())
    {
        material.renderer().destroyMaterial(material);
    }

    decoder >> beginObject();

    // Decode base material
    if (decoder.selectMember("base"))
    {
        Path basePath;
        decoder >> decodeValue(basePath);

        if (!basePath.empty())
        {
            // Load the base material
            material._base = decoder.assetCache().getHandle<Material>(basePath);
        }
    }

    if (!material._base)
    {
        // Decode shader sources
        if (decoder.selectMember("shaderSources"))
        {
            decoder >> beginArray();
            while (decoder.hasMoreElements())
            {
                // Decode the type and path of the shader source
                ShaderSourceType type;
                Path path;
                decoder >> beginObject()
                        >> decodeEnum("type", type, true)
                        >> decodeValue("path", path, true)
                        >> endObject();

                // Resolve the path
                AssetCache& assetCache = decoder.assetCache();
                path = assetCache.resolvePath(path);

                // Load the shader source to a string
                auto stream = assetCache.fileSystem().openFileForRead(path);
                std::string source = stream->readAllToString();

                // Add the shader source
                material._shaderSources.push_back(ShaderSource(type, path, source));
            }
            decoder >> endArray();
        }

        // Decode parameters
        if (decoder.selectMember("parameters"))
        {
            decoder >> beginArray();
            while (decoder.hasMoreElements())
            {
                decoder >> beginObject();

                // Decode name, type, and/or binding of the parameter
                std::string name;
                MaterialValueType type = MaterialValueType_Null;
                MaterialParameterBinding binding = MaterialParameterBinding_None;
                decoder >> decodeValue("name", name, true)
                        >> decodeEnum("type", type, false)
                        >> decodeEnum("binding", binding, false);

                decoder >> endObject();

                // Add the parameter
                material.addParameter(name, type, binding);
            }
            decoder >> endArray();
        }
    }

    // Decode arguments
    if (decoder.selectMember("arguments"))
    {
        decoder >> beginArray();
        while (decoder.hasMoreElements())
        {
            decoder >> beginObject();

            // Decode the name of the parameter that the argument is for
            std::string name;
            decoder >> decodeValue("name", name, true);

            // Attempt to get the parameter
            const MaterialParameter& parameter = material.parameter(name);

            // Decode the value of the argument
            MaterialValue value(parameter.type());
            decoder >> decodeValue("value", value, true);

            // Set the argumemt value
            material.setArgument(parameter, value);

            decoder >> endObject();
        }
        decoder >> endArray();
    }

    if (decoder.isBinaryStream())
    {
        // Decode all properties
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
        // Decode type
        if (decoder.selectMember("type"))
        {
            MaterialType type;
            decoder >> decodeEnum(type);
            material.setType(type);
        }

        // Decode if blending is enabled
        if (decoder.selectMember("blend"))
        {
            bool value;
            decoder >> decodeValue(value);
            material.setBlend(value);
        }

        // Decode blend factors
        if (decoder.selectMember("blendFactors"))
        {
            decoder >> beginObject();

            // Decode source and destination blend factors
            BlendFactor source = BlendFactor_One;
            BlendFactor destination = BlendFactor_One;
            decoder >> decodeEnum("source", source)
                    >> decodeEnum("destination", destination);

            // Set the blend factors
            material.setBlendFactors(source, destination);

            decoder >> endObject();
        }

        // Decode if depth testing enabled
        if (decoder.selectMember("depthTest"))
        {
            bool value;
            decoder >> decodeValue(value);
            material.setDepthTest(value);
        }

        // Decode if depth writing enabled
        if (decoder.selectMember("depthWrite"))
        {
            bool value;
            decoder >> decodeValue(value);
            material.setDepthWrite(value);
        }

        // Decode if back face culling enabled
        if (decoder.selectMember("cullBackFace"))
        {
            bool value;
            decoder >> decodeValue(value);
            material.setCullBackFace(value);
        }

        // Decode the priority
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
