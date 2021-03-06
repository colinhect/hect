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
#include "Uniform.h"

using namespace hect;

Uniform::Uniform() :
    _name(Name::Unnamed)
{
}

Uniform::Uniform(Name name, UniformType type) :
    _name(name),
    _type(type)
{
}

Uniform::Uniform(Name name, UniformBinding binding) :
    _name(name),
    _binding(binding)
{
    resolve_type();
}

Uniform::Uniform(Name name, const UniformValue& value) :
    _name(name),
    _value(value)
{
    resolve_type();
}

Name Uniform::name() const
{
    return _name;
}

UniformType Uniform::type() const
{
    return _type;
}

UniformBinding Uniform::binding() const
{
    return _binding;
}

const UniformValue& Uniform::value() const
{
    return _value;
}

void Uniform::set_value(const UniformValue& value)
{
    _binding = UniformBinding::None;
    _type = value.type();
    _value = value;
}

UniformIndex Uniform::index() const
{
    return _index;
}

TextureIndex Uniform::texture_index() const
{
    return _texture_index;
}

UniformLocation Uniform::location() const
{
    return _location;
}

void Uniform::set_location(UniformLocation location)
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

    // Value
    if (_binding == UniformBinding::None && _value != uniform._value)
    {
        return false;
    }

    return true;
}

bool Uniform::operator!=(const Uniform& uniform) const
{
    return !(*this == uniform);
}

void Uniform::encode(Encoder& encoder) const
{
    bool is_bound = _binding != UniformBinding::None;

    if (encoder.is_binary_stream())
    {
        encoder << encode_value(is_bound);
    }

    if (is_bound)
    {
        encoder << encode_value("name", _name)
                << encode_enum("binding", _binding);
    }
    else
    {
        encoder << encode_value("name", _name);
        _value.encode(encoder);
    }
}

void Uniform::decode(Decoder& decoder)
{
    if (decoder.is_binary_stream())
    {
        bool is_bound = false;
        decoder >> decode_value(is_bound);

        if (is_bound)
        {
            decoder >> decode_value("name", _name)
                    >> decode_enum("binding", _binding);
        }
        else
        {
            decoder >> decode_value("name", _name);
            _value.decode(decoder);
        }
    }
    else
    {
        decoder >> decode_value("name", _name)
                >> decode_enum("binding", _binding);

        _value.decode(decoder);
    }
    resolve_type();
}

void Uniform::resolve_type()
{
    switch (_binding)
    {
    case UniformBinding::None:
        _type = _value.type();
        break;
    case UniformBinding::CameraExposure:
    case UniformBinding::CameraOneOverGamma:
        _type = UniformType::Float;
        break;
    case UniformBinding::RenderTargetSize:
        _type = UniformType::Vector2;
        break;
    case UniformBinding::CameraPosition:
    case UniformBinding::CameraFront:
    case UniformBinding::CameraUp:
    case UniformBinding::PrimaryLightDirection:
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
    case UniformBinding::PrimaryLightColor:
        _type = UniformType::Color;
        break;
    case UniformBinding::LightProbeTexture:
    case UniformBinding::SkyBoxTexture:
        _type = UniformType::TextureCube;
        break;
    case UniformBinding::DiffuseBuffer:
    case UniformBinding::MaterialBuffer:
    case UniformBinding::PositionBuffer:
    case UniformBinding::NormalBuffer:
    case UniformBinding::BackBuffer:
        _type = UniformType::Texture2;
        break;
    }
}
