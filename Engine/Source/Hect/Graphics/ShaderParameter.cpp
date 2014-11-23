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
#include "ShaderParameter.h"

using namespace hect;

ShaderParameter::ShaderParameter()
{
}

ShaderParameter::ShaderParameter(const std::string& name, ShaderParameterBinding binding) :
    _name(name),
    _binding(binding),
    _defaultValueSet(false)
{
    setBinding(binding);
}

ShaderParameter::ShaderParameter(const std::string& name, const ShaderValue& defaultValue) :
    _name(name),
    _type(defaultValue.type()),
    _defaultValueSet(true),
    _defaultValue(defaultValue)
{
}

ShaderValueType ShaderParameter::type() const
{
    return _type;
}

void ShaderParameter::setType(ShaderValueType type)
{
    if (hasBinding())
    {
        throw Error("Cannot change the type of a parameter with a binding");
    }

    _type = type;
}

ShaderParameterBinding ShaderParameter::binding() const
{
    return _binding;
}

void ShaderParameter::setBinding(ShaderParameterBinding binding)
{
    switch (binding)
    {
    case ShaderParameterBinding_None:
        break;
    case ShaderParameterBinding_CameraExposure:
    case ShaderParameterBinding_CameraOneOverGamma:
        _type = ShaderValueType_Float;
        break;
    case ShaderParameterBinding_RenderTargetSize:
        _type = ShaderValueType_Vector2;
        break;
    case ShaderParameterBinding_CameraPosition:
    case ShaderParameterBinding_CameraFront:
    case ShaderParameterBinding_CameraUp:
        _type = ShaderValueType_Vector3;
        break;
    case ShaderParameterBinding_ViewMatrix:
    case ShaderParameterBinding_ProjectionMatrix:
    case ShaderParameterBinding_ViewProjectionMatrix:
    case ShaderParameterBinding_ModelMatrix:
    case ShaderParameterBinding_ModelViewMatrix:
    case ShaderParameterBinding_ModelViewProjectionMatrix:
        _type = ShaderValueType_Matrix4;
        break;
    }

    _binding = binding;

    if (_binding != ShaderParameterBinding_None)
    {
        _defaultValueSet = false;
    }
}

bool ShaderParameter::hasBinding() const
{
    return _binding != ShaderParameterBinding_None;
}

const ShaderValue& ShaderParameter::defaultValue() const
{
    return _defaultValue;
}

void ShaderParameter::setDefaultValue(const ShaderValue& defaultValue)
{
    _defaultValueSet = true;
    _binding = ShaderParameterBinding_None;

    _type = defaultValue.type();
    _defaultValue = defaultValue;
}

bool ShaderParameter::hasDefaultValue() const
{
    return _defaultValueSet;
}

const std::string& ShaderParameter::name() const
{
    return _name;
}

void ShaderParameter::setName(const std::string& name)
{
    _name = name;
}

int ShaderParameter::location() const
{
    return _location;
}

void ShaderParameter::setLocation(int location)
{
    _location = location;
}

bool ShaderParameter::operator==(const ShaderParameter& shaderParameter) const
{
    // Name
    if (_name != shaderParameter._name)
    {
        return false;
    }

    // Type
    if (_type != shaderParameter._type)
    {
        return false;
    }

    // Binding
    if (_binding != shaderParameter._binding)
    {
        return false;
    }

    // Has default value
    if (_defaultValueSet != shaderParameter._defaultValueSet)
    {
        return false;
    }

    // Default value
    if (_defaultValue != shaderParameter._defaultValue)
    {
        return false;
    }

    return true;
}

bool ShaderParameter::operator!=(const ShaderParameter& shaderParameter) const
{
    return !(*this == shaderParameter);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const ShaderParameter& shaderParameter)
{
    encoder << beginObject();

    // Name
    encoder << encodeValue("name", shaderParameter.name());

    // We need an extra hint in binary
    if (encoder.isBinaryStream())
    {
        WriteStream& stream = encoder.binaryStream();
        stream << shaderParameter.hasDefaultValue();
    }

    // Default value
    if (shaderParameter.hasDefaultValue())
    {
        encoder << shaderParameter.defaultValue();
    }

    // Binding
    else if (shaderParameter.hasBinding())
    {
        encoder << encodeEnum("binding", shaderParameter.binding());
    }
    else
    {
        throw Error("The parameter does not have a default value or a binding");
    }

    return encoder << endObject();
}

Decoder& operator>>(Decoder& decoder, ShaderParameter& shaderParameter)
{
    decoder >> beginObject();

    // Name
    if (decoder.selectMember("name"))
    {
        std::string name;
        decoder >> decodeValue(name);
        shaderParameter.setName(name);
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
        ShaderValue defaultValue;
        decoder >> decodeValue(defaultValue);
        shaderParameter.setDefaultValue(defaultValue);
    }

    // Binding
    else if (decoder.selectMember("binding"))
    {
        ShaderParameterBinding binding;
        decoder >> decodeEnum(binding);
        shaderParameter.setBinding(binding);
    }
    else
    {
        throw Error("No default value or binding specified");
    }

    return decoder >> endObject();
}

}