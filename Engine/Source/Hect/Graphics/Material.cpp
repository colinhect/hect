///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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

const Shader::Handle& Material::shader() const
{
    return _shader;
}

void Material::setShader(const Shader::Handle& shader)
{
    clearUniformValues();
    _shader = shader;
}

Material::UniformValueSequence Material::uniformValues()
{
    return _uniformValues;
}

const Material::UniformValueSequence Material::uniformValues() const
{
    return _uniformValues;
}

void Material::setUniformValue(const std::string& name, const UniformValue& value)
{
    if (!_shader)
    {
        throw InvalidOperation("Material does not have a shader set");
    }

    // Ensure the value is the same type as the uniform and is not bound
    const Uniform& uniform = _shader->uniform(name);
    if (uniform.type() != value.type())
    {
        const char* uniformName = uniform.name().data();
        const char* shaderName = _shader->name().data();
        const char* expectedType = Enum::toString(uniform.type()).data();
        const char* actualType = Enum::toString(value.type()).data();
        throw InvalidOperation(format("Uniform '%s' in shader '%s' is of type '%s' and cannot be assigned a value of type '%s'", uniformName, shaderName, expectedType, actualType));
    }
    else if (uniform.binding() != UniformBinding::None)
    {
        const char* uniformName = uniform.name().data();
        const char* shaderName = _shader->name().data();
        throw InvalidOperation(format("Uniform '%s' in shader '%s' is bound and cannot be assigned a value", uniformName, shaderName));
    }

    UniformIndex index = uniform.index();

    // Resize the uniform value vector if needed
    if (index >= _uniformValues.size())
    {
        _uniformValues.resize(index + 1);
    }

    // Set the value
    _uniformValues[index] = value;
}

void Material::clearUniformValues()
{
    _uniformValues.clear();
}

CullMode Material::cullMode() const
{
    return _cullMode;
}

void Material::setCullMode(CullMode cullMode)
{
    _cullMode = cullMode;
}

bool Material::operator==(const Material& material) const
{
    // Shader
    if (_shader != material._shader)
    {
        return false;
    }

    // Uniform value count
    if (_uniformValues.size() != material._uniformValues.size())
    {
        return false;
    }

    // Uniform values
    for (size_t i = 0; i < _uniformValues.size(); ++i)
    {
        if (_uniformValues[i] != material._uniformValues[i])
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


void Material::encode(Encoder& encoder) const
{
    // Shader
    encoder << encodeValue("shader", _shader);
    encoder << encodeEnum("cullMode", _cullMode);

    // Uniform values
    encoder << beginArray("uniformValues");
    for (UniformIndex index = 0; index < _uniformValues.size(); ++index)
    {
        const UniformValue& value = _uniformValues[index];
        if (value)
        {
            if (encoder.isBinaryStream())
            {
                encoder << beginObject()
                        << encodeValue(static_cast<uint8_t>(index));

                value.encode(encoder);

                encoder << endObject();
            }
            else
            {
                const Uniform& uniform = _shader->uniform(index);
                encoder << beginObject()
                        << encodeValue("name", uniform.name());

                value.encode(encoder);

                encoder << endObject();
            }
        }
    }
    encoder << endArray();
}

void Material::decode(Decoder& decoder)
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
                throw DecodeError(format("Failed to load base material '%s': %s", basePath.asString().data(), exception.what()));
            }
        }
    }

    // Shader
    decoder >> decodeValue("shader", _shader);
    decoder >> decodeEnum("cullMode", _cullMode);

    // Uniform values
    if (decoder.selectMember("uniformValues"))
    {
        decoder >> beginArray();
        while (decoder.hasMoreElements())
        {
            try
            {
                std::string name;
                UniformValue value;

                if (decoder.isBinaryStream())
                {
                    uint8_t index = 0;

                    decoder >> beginObject()
                            >> decodeValue(index);

                    value.decode(decoder);

                    decoder>> endObject();

                    const Uniform& uniform = _shader->uniform(index);
                    name = uniform.name();
                }
                else
                {
                    decoder >> beginObject()
                            >> decodeValue("name", name, true);

                    value.decode(decoder);

                    decoder >> endObject();
                }

                setUniformValue(name, value);
            }
            catch (const InvalidOperation& exception)
            {
                throw DecodeError(exception.what());
            }
        }
        decoder >> endArray();
    }
}
