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
#include "UniformValue.h"

#include "Hect/IO/AssetCache.h"
#include "Hect/IO/Path.h"

using namespace hect;

UniformValue::UniformValue()
{
}

UniformValue::UniformValue(UniformType type) :
    _type(type)
{
    set_default_value();
}

UniformValue::UniformValue(int value) :
    _type(UniformType::Int)
{
    set_value(value);
}

UniformValue::UniformValue(double value) :
    _type(UniformType::Float)
{
    set_value(value);
}

UniformValue::UniformValue(Vector2 value) :
    _type(UniformType::Vector2)
{
    set_value(value);
}

UniformValue::UniformValue(Vector3 value) :
    _type(UniformType::Vector3)
{
    set_value(value);
}

UniformValue::UniformValue(Vector4 value) :
    _type(UniformType::Vector4)
{
    set_value(value);
}

UniformValue::UniformValue(const Matrix4& value) :
    _type(UniformType::Matrix4)
{
    set_value(value);
}

UniformValue::UniformValue(Color value) :
    _type(UniformType::Color)
{
    set_value(value);
}

UniformValue::UniformValue(const Texture2::Handle& value) :
    _type(UniformType::Texture2)
{
    set_value(value);
}

UniformValue::UniformValue(const Texture3::Handle& value) :
    _type(UniformType::Texture3)
{
    set_value(value);
}

UniformValue::UniformValue(const TextureCube::Handle& value) :
    _type(UniformType::TextureCube)
{
    set_value(value);
}

UniformType UniformValue::type() const
{
    return _type;
}

void UniformValue::set_default_value()
{
    switch (_type)
    {
    case UniformType::Int:
        _value = int(0);
        break;
    case UniformType::Float:
        _value = double(0.0);
        break;
    case UniformType::Vector2:
        _value = Vector2();
        break;
    case UniformType::Vector3:
        _value = Vector3();
        break;
    case UniformType::Vector4:
        _value = Vector4();
        break;
    case UniformType::Matrix4:
        _value = Matrix4();
        break;
    case UniformType::Color:
        _value = Color();
        break;
    case UniformType::Texture2:
        _value = Texture2::Handle();
        break;
    case UniformType::Texture3:
        _value = Texture3::Handle();
        break;
    case UniformType::TextureCube:
        _value = TextureCube::Handle();
        break;
    }
}

void UniformValue::set_value(int value)
{
    if (_type != UniformType::Int)
    {
        throw InvalidOperation("Uniform value is not of type 'Int'");
    }

    _value = value;
}

void UniformValue::set_value(double value)
{
    if (_type != UniformType::Float)
    {
        throw InvalidOperation("Uniform value is not of type 'Float'");
    }

    _value = value;
}

void UniformValue::set_value(Vector2 value)
{
    if (_type != UniformType::Vector2)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector2'");
    }

    _value = value;
}

void UniformValue::set_value(Vector3 value)
{
    if (_type != UniformType::Vector3)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector3'");
    }

    _value = value;
}

void UniformValue::set_value(Vector4 value)
{
    if (_type != UniformType::Vector4)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector4'");
    }

    _value = value;
}

void UniformValue::set_value(const Matrix4& value)
{
    if (_type != UniformType::Matrix4)
    {
        throw InvalidOperation("Uniform value is not of type 'Matrix4'");
    }

    _value = value;
}

void UniformValue::set_value(Color value)
{
    if (_type != UniformType::Color)
    {
        throw InvalidOperation("Uniform value is not of type 'Color'");
    }

    _value = value;
}

void UniformValue::set_value(const Texture2::Handle& value)
{
    if (_type != UniformType::Texture2)
    {
        throw InvalidOperation("Uniform value is not of type 'Texture2'");
    }

    _value = value;
}

void UniformValue::set_value(const Texture3::Handle& value)
{
    if (_type != UniformType::Texture3)
    {
        throw InvalidOperation("Uniform value is not of type 'Texture3'");
    }

    _value = value;
}

void UniformValue::set_value(const TextureCube::Handle& value)
{
    if (_type != UniformType::TextureCube)
    {
        throw InvalidOperation("Uniform value is not of type 'TextureCube'");
    }

    _value = value;
}

int UniformValue::as_int() const
{
    if (_type != UniformType::Int)
    {
        throw InvalidOperation("Uniform value is not of type 'Int'");
    }

    return _value.as<int>();
}

double UniformValue::as_double() const
{
    if (_type != UniformType::Float)
    {
        throw InvalidOperation("Uniform value is not of type 'Float'");
    }

    return _value.as<double>();
}

Vector2 UniformValue::as_vector2() const
{
    if (_type != UniformType::Vector2)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector2'");
    }

    return _value.as<Vector2>();
}

Vector3 UniformValue::as_vector3() const
{
    if (_type != UniformType::Vector3)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector3'");
    }

    return _value.as<Vector3>();
}

Vector4 UniformValue::as_vector4() const
{
    if (_type != UniformType::Vector4)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector4'");
    }

    return _value.as<Vector4>();
}

Matrix4 UniformValue::as_matrix4() const
{
    if (_type != UniformType::Matrix4)
    {
        throw InvalidOperation("Uniform value is not of type 'Matrix4'");
    }

    return _value.as<Matrix4>();
}

Color UniformValue::as_color() const
{
    if (_type != UniformType::Color)
    {
        throw InvalidOperation("Uniform value is not of type 'Color'");
    }

    return _value.as<Color>();
}

Texture2::Handle UniformValue::as_texture2() const
{
    if (_type != UniformType::Texture2)
    {
        throw InvalidOperation("Uniform value is not of type 'Texture2'");
    }

    return _value.as<Texture2::Handle>();
}

Texture3::Handle UniformValue::as_texture3() const
{
    if (_type != UniformType::Texture3)
    {
        throw InvalidOperation("Uniform value is not of type 'Texture3'");
    }

    return _value.as<Texture3::Handle>();
}

TextureCube::Handle UniformValue::as_texture_cube() const
{
    if (_type != UniformType::TextureCube)
    {
        throw InvalidOperation("Uniform value is not of type 'TextureCube'");
    }

    return _value.as<TextureCube::Handle>();
}

UniformValue::operator bool() const
{
    return _value.has_value();
}

bool UniformValue::operator==(const UniformValue& uniform_value) const
{
    // Type
    if (_type != uniform_value._type)
    {
        return false;
    }

    // Value
    switch (_type)
    {
    case UniformType::Int:
        return as_int() == uniform_value.as_int();
    case UniformType::Float:
        return as_double() == uniform_value.as_double();
    case UniformType::Vector2:
        return as_vector2() == uniform_value.as_vector2();
    case UniformType::Vector3:
        return as_vector3() == uniform_value.as_vector3();
    case UniformType::Vector4:
        return as_vector4() == uniform_value.as_vector4();
    case UniformType::Matrix4:
        return as_matrix4() == uniform_value.as_matrix4();
    case UniformType::Color:
        return as_color() == uniform_value.as_color();
    case UniformType::Texture2:
        return as_texture2() == uniform_value.as_texture2();
    case UniformType::Texture3:
        return as_texture3() == uniform_value.as_texture3();
    case UniformType::TextureCube:
        return as_texture_cube() == uniform_value.as_texture_cube();
    }

    return false;
}

bool UniformValue::operator!=(const UniformValue& uniform_value) const
{
    return !(*this == uniform_value);
}

void UniformValue::encode(Encoder& encoder) const
{
    // Type
    encoder << encode_enum("type", _type);

    // Value
    switch (_type)
    {
    case UniformType::Int:
        encoder << encode_value("value", as_int());
        break;
    case UniformType::Float:
        encoder << encode_value("value", as_double());
        break;
    case UniformType::Vector2:
        encoder << encode_value("value", as_vector2());
        break;
    case UniformType::Vector3:
        encoder << encode_value("value", as_vector3());
        break;
    case UniformType::Vector4:
        encoder << encode_value("value", as_vector4());
        break;
    case UniformType::Matrix4:
        encoder << encode_value("value", as_matrix4());
        break;
    case UniformType::Color:
        encoder << encode_value("value", as_color());
        break;
    case UniformType::Texture2:
        encoder << encode_value("value", as_texture2());
        break;
    case UniformType::Texture3:
        encoder << encode_value("value", as_texture3());
        break;
    case UniformType::TextureCube:
        encoder << encode_value("value", as_texture_cube());
        break;
    }
}

void UniformValue::decode(Decoder& decoder)
{
    // Type
    decoder >> decode_enum("type", _type);

    // Value
    if (decoder.select_member("value"))
    {
        switch (_type)
        {
        case UniformType::Int:
        {
            int value;
            decoder >> decode_value(value);
            set_value(value);
        }
        break;
        case UniformType::Float:
        {
            double value;
            decoder >> decode_value(value);
            set_value(value);
        }
        break;
        case UniformType::Vector2:
        {
            Vector2 value;
            decoder >> decode_value(value);
            set_value(value);
        }
        break;
        case UniformType::Vector3:
        {
            Vector3 value;
            decoder >> decode_value(value);
            set_value(value);
        }
        break;
        case UniformType::Vector4:
        {
            Vector4 value;
            decoder >> decode_value(value);
            set_value(value);
        }
        break;
        case UniformType::Matrix4:
        {
            Matrix4 value;
            decoder >> decode_value(value);
            set_value(value);
        }
        break;
        case UniformType::Color:
        {
            Color value;
            decoder >> decode_value(value);
            set_value(value);
        }
        break;
        case UniformType::Texture2:
        {
            Texture2::Handle texture;
            decoder >> decode_value(texture);
            set_value(texture);
        }
        break;
        case UniformType::Texture3:
        {
            Texture3::Handle texture;
            decoder >> decode_value(texture);
            set_value(texture);
        }
        break;
        case UniformType::TextureCube:
        {
            TextureCube::Handle texture;
            decoder >> decode_value(texture);
            set_value(texture);
        }
        break;
        default:
            throw DecodeError(format("Unknown uniform type '%s'", Enum::to_string(_type).data()));
        }
    }
    else
    {
        set_default_value();
    }
}
