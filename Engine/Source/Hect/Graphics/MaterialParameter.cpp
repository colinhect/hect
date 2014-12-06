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
#include "MaterialParameter.h"

using namespace hect;

MaterialParameter::MaterialParameter()
{
}

MaterialParameter::MaterialParameter(const std::string& name, MaterialParameterBinding binding) :
    _name(name),
    _binding(binding),
    _defaultValueSet(false)
{
    setBinding(binding);
}

MaterialParameter::MaterialParameter(const std::string& name, const MaterialValue& defaultValue) :
    _name(name),
    _type(defaultValue.type()),
    _defaultValueSet(true),
    _defaultValue(defaultValue)
{
}

MaterialValueType MaterialParameter::type() const
{
    return _type;
}

void MaterialParameter::setType(MaterialValueType type)
{
    if (hasBinding())
    {
        throw Error("Cannot change the type of a parameter with a binding");
    }

    _type = type;
}

MaterialParameterBinding MaterialParameter::binding() const
{
    return _binding;
}

void MaterialParameter::setBinding(MaterialParameterBinding binding)
{
    switch (binding)
    {
    case MaterialParameterBinding_None:
        break;
    case MaterialParameterBinding_CameraExposure:
    case MaterialParameterBinding_CameraOneOverGamma:
        _type = MaterialValueType_Float;
        break;
    case MaterialParameterBinding_RenderTargetSize:
        _type = MaterialValueType_Vector2;
        break;
    case MaterialParameterBinding_CameraPosition:
    case MaterialParameterBinding_CameraFront:
    case MaterialParameterBinding_CameraUp:
        _type = MaterialValueType_Vector3;
        break;
    case MaterialParameterBinding_ViewMatrix:
    case MaterialParameterBinding_ProjectionMatrix:
    case MaterialParameterBinding_ViewProjectionMatrix:
    case MaterialParameterBinding_ModelMatrix:
    case MaterialParameterBinding_ModelViewMatrix:
    case MaterialParameterBinding_ModelViewProjectionMatrix:
        _type = MaterialValueType_Matrix4;
        break;
    }

    _binding = binding;

    if (_binding != MaterialParameterBinding_None)
    {
        _defaultValueSet = false;
    }
}

bool MaterialParameter::hasBinding() const
{
    return _binding != MaterialParameterBinding_None;
}

const MaterialValue& MaterialParameter::defaultValue() const
{
    return _defaultValue;
}

void MaterialParameter::setDefaultValue(const MaterialValue& defaultValue)
{
    _defaultValueSet = true;
    _binding = MaterialParameterBinding_None;

    _type = defaultValue.type();
    _defaultValue = defaultValue;

    // If the default value is an empty texture then consider the default unset
    if (_type == MaterialValueType_Texture && !defaultValue.asTexture())
    {
        _defaultValueSet = false;
    }
}

bool MaterialParameter::hasDefaultValue() const
{
    return _defaultValueSet;
}

const std::string& MaterialParameter::name() const
{
    return _name;
}

void MaterialParameter::setName(const std::string& name)
{
    _name = name;
}

int MaterialParameter::location() const
{
    return _location;
}

void MaterialParameter::setLocation(int location)
{
    _location = location;
}

unsigned MaterialParameter::textureIndex() const
{
    if (_type != MaterialValueType_Texture)
    {
        throw Error("Parameter is not a texture");
    }

    return _textureIndex;
}

void MaterialParameter::setTextureIndex(unsigned textureIndex)
{
    if (_type != MaterialValueType_Texture)
    {
        throw Error("Parameter is not a texture");
    }

    _textureIndex = textureIndex;
}

bool MaterialParameter::operator==(const MaterialParameter& materialParameter) const
{
    // Name
    if (_name != materialParameter._name)
    {
        return false;
    }

    // Type
    if (_type != materialParameter._type)
    {
        return false;
    }

    // Binding
    if (_binding != materialParameter._binding)
    {
        return false;
    }

    // Has default value
    if (_defaultValueSet != materialParameter._defaultValueSet)
    {
        return false;
    }

    // Default value
    if (_defaultValue != materialParameter._defaultValue)
    {
        return false;
    }

    return true;
}

bool MaterialParameter::operator!=(const MaterialParameter& materialParameter) const
{
    return !(*this == materialParameter);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const MaterialParameter& materialParameter)
{
    encoder << beginObject();

    // Name
    encoder << encodeValue("name", materialParameter.name());

    // We need an extra hint in binary
    if (encoder.isBinaryStream())
    {
        WriteStream& stream = encoder.binaryStream();
        stream << materialParameter.hasDefaultValue();
    }

    // Default value
    if (materialParameter.hasDefaultValue())
    {
        encoder << materialParameter.defaultValue();
    }

    // Binding
    else if (materialParameter.hasBinding())
    {
        encoder << encodeEnum("binding", materialParameter.binding());
    }
    else
    {
        throw Error("The parameter does not have a default value or a binding");
    }

    return encoder << endObject();
}

Decoder& operator>>(Decoder& decoder, MaterialParameter& materialParameter)
{
    decoder >> beginObject();

    // Name
    if (decoder.selectMember("name"))
    {
        std::string name;
        decoder >> decodeValue(name);
        materialParameter.setName(name);
    }
    else
    {
        throw Error("No parameter name specified");
    }

    // Detect if the parameter has a default value or a binding
    bool hasDefaultValue;
    if (decoder.isBinaryStream())
    {
        ReadStream& stream = decoder.binaryStream();
        stream >> hasDefaultValue;
    }
    else
    {
        hasDefaultValue = decoder.selectMember("type");
    }

    // Default value
    if (hasDefaultValue)
    {
        MaterialValue defaultValue;
        decoder >> decodeValue(defaultValue);
        materialParameter.setDefaultValue(defaultValue);
    }

    // Binding
    else if (decoder.selectMember("binding"))
    {
        MaterialParameterBinding binding;
        decoder >> decodeEnum(binding);
        materialParameter.setBinding(binding);
    }
    else
    {
        throw Error("No default value or binding specified");
    }

    return decoder >> endObject();
}

}