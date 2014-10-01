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
#include "Uniform.h"

using namespace hect;

Uniform::Uniform() :
    _type(UniformType_Float),
    _binding(UniformBinding_None),
    _defaultValueSet(true),
    _defaultValue((Real)0),
    _location(-1)
{
}

Uniform::Uniform(const std::string& name, UniformBinding binding) :
    _name(name),
    _type(UniformType_Float),
    _binding(binding),
    _defaultValueSet(false),
    _location(-1)
{
    setBinding(binding);
}

Uniform::Uniform(const std::string& name, const UniformValue& defaultValue) :
    _name(name),
    _type(defaultValue.type()),
    _binding(UniformBinding_None),
    _defaultValueSet(true),
    _defaultValue(defaultValue),
    _location(-1)
{
}

UniformType Uniform::type() const
{
    return _type;
}

void Uniform::setType(UniformType type)
{
    if (hasBinding())
    {
        throw Error("Cannot change the type of a uniform with a binding");
    }

    _type = type;
}

UniformBinding Uniform::binding() const
{
    return _binding;
}

void Uniform::setBinding(UniformBinding binding)
{
    switch (binding)
    {
    case UniformBinding_RenderTargetSize:
        _type = UniformType_Vector2;
        break;
    case UniformBinding_CameraPosition:
    case UniformBinding_CameraUp:
        _type = UniformType_Vector3;
        break;
    case UniformBinding_ViewMatrix:
    case UniformBinding_ProjectionMatrix:
    case UniformBinding_ViewProjectionMatrix:
    case UniformBinding_ModelMatrix:
    case UniformBinding_ModelViewMatrix:
    case UniformBinding_ModelViewProjectionMatrix:
        _type = UniformType_Matrix4;
        break;
    }

    _binding = binding;

    if (_binding != UniformBinding_None)
    {
        _defaultValueSet = false;
    }
}

bool Uniform::hasBinding() const
{
    return _binding != UniformBinding_None;
}

const UniformValue& Uniform::defaultValue() const
{
    return _defaultValue;
}

void Uniform::setDefaultValue(const UniformValue& defaultValue)
{
    _defaultValueSet = true;
    _binding = UniformBinding_None;

    _type = defaultValue.type();
    _defaultValue = defaultValue;
}

bool Uniform::hasDefaultValue() const
{
    return _defaultValueSet;
}

const std::string& Uniform::name() const
{
    return _name;
}

void Uniform::setName(const std::string& name)
{
    _name = name;
}

int Uniform::location() const
{
    return _location;
}

void Uniform::setLocation(int location)
{
    _location = location;
}

bool Uniform::operator==(const Uniform& uniform) const
{
    // Name
    if (_name != uniform._name)
    {
        return false;
    }

    // Type
    if (_type != uniform._type)
    {
        return false;
    }

    // Binding
    if (_binding != uniform._binding)
    {
        return false;
    }

    // Has default value
    if (_defaultValueSet != uniform._defaultValueSet)
    {
        return false;
    }

    // Default value
    if (_defaultValue != uniform._defaultValue)
    {
        return false;
    }

    return true;
}

bool Uniform::operator!=(const Uniform& uniform) const
{
    return !(*this == uniform);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Uniform& uniform)
{
    encoder << beginObject();

    // Name
    encoder << encodeValue("name", uniform.name());

    // We need an extra hint in binary
    if (encoder.isBinaryStream())
    {
        WriteStream& stream = encoder.binaryStream();
        stream << uniform.hasDefaultValue();
    }

    // Default value
    if (uniform.hasDefaultValue())
    {
        encoder << uniform.defaultValue();
    }

    // Binding
    else if (uniform.hasBinding())
    {
        encoder << encodeEnum("binding", uniform.binding());
    }
    else
    {
        throw Error("The uniform does not have a default value or a binding");
    }

    return encoder << endObject();
}

Decoder& operator>>(Decoder& decoder, Uniform& uniform)
{
    decoder >> beginObject();

    // Name
    if (decoder.selectMember("name"))
    {
        std::string name;
        decoder >> decodeValue(name);
        uniform.setName(name);
    }
    else
    {
        throw Error("No uniform name specified");
    }

    // Detect if the uniform has a default value or a binding
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
        UniformValue defaultValue;
        decoder >> decodeValue(defaultValue);
        uniform.setDefaultValue(defaultValue);
    }

    // Binding
    else if (decoder.selectMember("binding"))
    {
        UniformBinding binding;
        decoder >> decodeEnum(binding);
        uniform.setBinding(binding);
    }
    else
    {
        throw Error("No default value or binding specified");
    }

    return decoder >> endObject();
}

}