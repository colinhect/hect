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
    _name("<unnamed>")
{
}

Uniform::Uniform(const std::string& name, UniformType type) :
    _name(name),
    _type(type)
{
}

Uniform::Uniform(const std::string& name, UniformBinding binding) :
    _name(name),
    _binding(binding)
{
    resolveType();
}

Uniform::Uniform(const std::string& name, const UniformValue& value) :
    _name(name),
    _value(value)
{
    resolveType();
}

const std::string& Uniform::name() const
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

void Uniform::setValue(const UniformValue& value)
{
    _binding = UniformBinding_None;
    _type = value.type();
    _value = value;
}

TextureIndex Uniform::textureIndex() const
{
    return _textureIndex;
}

void Uniform::setTextureIndex(TextureIndex textureIndex)
{
    _textureIndex = textureIndex;
}

UniformLocation Uniform::location() const
{
    return _location;
}

void Uniform::setLocation(UniformLocation location)
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
    if (_binding == UniformBinding_None && _value != uniform._value)
    {
        return false;
    }

    return true;
}

bool Uniform::operator!=(const Uniform& uniform) const
{
    return !(*this == uniform);
}

void Uniform::resolveType()
{
    switch (_binding)
    {
    case UniformBinding_None:
        _type = _value.type();
        break;
    case UniformBinding_CameraExposure:
    case UniformBinding_CameraOneOverGamma:
        _type = UniformType_Float;
        break;
    case UniformBinding_RenderTargetSize:
        _type = UniformType_Vector2;
        break;
    case UniformBinding_CameraPosition:
    case UniformBinding_CameraFront:
    case UniformBinding_CameraUp:
    case UniformBinding_PrimaryLightDirection:
    case UniformBinding_PrimaryLightColor:
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
    case UniformBinding_LightProbeCubeMap:
    case UniformBinding_SkyBoxCubeMap:
    case UniformBinding_DiffuseBuffer:
    case UniformBinding_MaterialBuffer:
    case UniformBinding_PositionBuffer:
    case UniformBinding_NormalBuffer:
    case UniformBinding_BackBuffer:
        _type = UniformType_Texture;
        break;
    }
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Uniform& uniform)
{
    bool isBound = uniform._binding != UniformBinding_None;

    encoder << beginObject();

    if (encoder.isBinaryStream())
    {
        encoder << encodeValue(isBound);
    }

    if (isBound)
    {
        encoder << encodeValue("name", uniform._name)
                << encodeEnum("binding", uniform._binding);
    }
    else
    {
        encoder << encodeValue("name", uniform._name)
                << encodeValue(uniform._value);
    }

    encoder << endObject();
    return encoder;
}

Decoder& operator>>(Decoder& decoder, Uniform& uniform)
{
    decoder >> beginObject();

    if (decoder.isBinaryStream())
    {
        bool isBound = false;
        decoder >> decodeValue(isBound);

        if (isBound)
        {
            decoder >> decodeValue("name", uniform._name)
                    >> decodeEnum("binding", uniform._binding);
        }
        else
        {
            decoder >> decodeValue("name", uniform._name)
                    >> decodeValue(uniform._value);
        }
    }
    else
    {
        decoder >> decodeValue("name", uniform._name)
                >> decodeEnum("binding", uniform._binding)
                >> decodeValue(uniform._value);
    }
    uniform.resolveType();

    decoder >> endObject();

    return decoder;
}

}