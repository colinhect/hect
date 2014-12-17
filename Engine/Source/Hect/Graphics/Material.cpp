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

MaterialType Material::type() const
{
    return _type;
}

void Material::setType(MaterialType type)
{
    _type = type;
}

bool Material::blend() const
{
    return _blend;
}

void Material::setBlend(bool value)
{
    _blend = value;
}

void Material::setBlendFactors(BlendFactor source, BlendFactor destination)
{
    _sourceFactor = source;
    _destinationFactor = destination;
}

BlendFactor Material::sourceBlendFactor() const
{
    return _sourceFactor;
}

BlendFactor Material::destinationBlendFactor() const
{
    return _destinationFactor;
}

bool Material::depthTest() const
{
    return _depthTest;
}

void Material::setDepthTest(bool value)
{
    _depthTest = value;
}

bool Material::depthWrite() const
{
    return _depthWrite;
}

void Material::setDepthWrite(bool value)
{
    _depthWrite = value;
}

bool Material::cullBackFace() const
{
    return _cullBackFace;
}

void Material::setCullBackFace(bool value)
{
    _cullBackFace = value;
}

int Material::priority() const
{
    return _priority;
}

void Material::setPriority(int priority)
{
    _priority = priority;
}

Material::ParameterSequence Material::parameters()
{
    return _parameters;
}

const Material::ParameterSequence Material::parameters() const
{
    return _parameters;
}

const MaterialParameter& Material::parameter(const std::string& name) const
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

const MaterialParameter& Material::parameter(const char* name) const
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
        return _parameterValues[index];
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
    if (_type != material._type)
    {
        return false;
    }

    // Compare blending factors
    if (_sourceFactor != material._sourceFactor || _destinationFactor != material._destinationFactor)
    {
        return false;
    }

    // Compare blending
    if (_blend != material._blend)
    {
        return false;
    }

    // Compare depth testing
    if (_depthTest != material._depthTest)
    {
        return false;
    }

    // Compare depth writing
    if (_depthWrite != material._depthWrite)
    {
        return false;
    }

    // Compare back face culling
    if (_cullBackFace != material._cullBackFace)
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
    encoder << beginObject();

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

    // Encode all properties
    encoder << encodeEnum("type", material._type)
            << encodeValue("depthTest", material._depthTest)
            << encodeValue("depthWrite", material._depthWrite)
            << encodeValue("cullBackFace", material._cullBackFace)
            << encodeValue("priority", material._priority)
            << encodeValue("blend", material._blend)
            << beginObject("blendFactors")
            << encodeEnum("source", material.sourceBlendFactor())
            << encodeEnum("destination", material.destinationBlendFactor())
            << endObject();

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
    if (!decoder.isBinaryStream())
    {
        // Decode the path
        Path path;
        decoder >> decodeValue("base", path);

        if (!path.empty())
        {
            material = decoder.assetCache().get<Material>(path);
        }
    }

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

    // Decode all properties
    decoder >> decodeEnum("type", material._type)
            >> decodeValue("depthTest", material._depthTest)
            >> decodeValue("depthWrite", material._depthWrite)
            >> decodeValue("cullBackFace", material._cullBackFace)
            >> decodeValue("priority", material._priority)
            >> decodeValue("blend", material._blend)
            >> beginObject("blendFactors")
            >> decodeEnum("source", material._sourceFactor)
            >> decodeEnum("destination", material._destinationFactor)
            >> endObject();

    decoder >> endObject();
    return decoder;
}

}
