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

#include "Hect/IO/Encoders/UniformEncoder.h"

using namespace hect;

Uniform::Uniform() :
    _type(UniformType::Float),
    _binding(UniformBinding::None),
    _defaultValueSet(true),
    _defaultValue((Real)0),
    _location(-1)
{
}

Uniform::Uniform(const std::string& name, UniformBinding::Enum binding) :
    _name(name),
    _type(UniformType::Float),
    _binding(binding),
    _defaultValueSet(false),
    _location(-1)
{
    setBinding(binding);
}

Uniform::Uniform(const std::string& name, const UniformValue& defaultValue) :
    _name(name),
    _type(defaultValue.type()),
    _binding(UniformBinding::None),
    _defaultValueSet(true),
    _defaultValue(defaultValue),
    _location(-1)
{
}

UniformType::Enum Uniform::type() const
{
    return _type;
}

void Uniform::setType(UniformType::Enum type)
{
    if (hasBinding())
    {
        throw Error("Cannot change the type of a uniform with a binding");
    }

    _type = type;
}

UniformBinding::Enum Uniform::binding() const
{
    return _binding;
}

void Uniform::setBinding(UniformBinding::Enum binding)
{
    switch (binding)
    {
    case UniformBinding::RenderTargetSize:
        _type = UniformType::Vector2;
        break;
    case UniformBinding::CameraPosition:
    case UniformBinding::CameraUp:
        _type = UniformType::Vector3;
        break;
    case UniformBinding::ViewMatrix:
    case UniformBinding::ProjectionMatrix:
    case UniformBinding::ViewProjectionMatrix:
    case UniformBinding::ModelMatrix:
    case UniformBinding::ModelViewMatrix:
    case UniformBinding::ModelViewProjectionMatrix:
        _type = UniformType::Matrix4;
        break;
    }

    _binding = binding;

    if (_binding != UniformBinding::None)
    {
        _defaultValueSet = false;
    }
}

bool Uniform::hasBinding() const
{
    return _binding != UniformBinding::None;
}

const UniformValue& Uniform::defaultValue() const
{
    return _defaultValue;
}

void Uniform::setDefaultValue(const UniformValue& defaultValue)
{
    _defaultValueSet = true;
    _binding = UniformBinding::None;

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

void Uniform::encode(ObjectEncoder& encoder) const
{
    UniformEncoder::encode(*this, encoder);
}

void Uniform::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    assetCache;
    UniformEncoder::decode(*this, decoder);
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