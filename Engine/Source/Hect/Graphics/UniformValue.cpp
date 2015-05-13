///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
    setDefaultValue();
}

UniformValue::UniformValue(int value) :
    _type(UniformType::Int)
{
    setValue(value);
}

UniformValue::UniformValue(double value) :
    _type(UniformType::Float)
{
    setValue(value);
}

UniformValue::UniformValue(const Vector2& value) :
    _type(UniformType::Vector2)
{
    setValue(value);
}

UniformValue::UniformValue(const Vector3& value) :
    _type(UniformType::Vector3)
{
    setValue(value);
}

UniformValue::UniformValue(const Vector4& value) :
    _type(UniformType::Vector4)
{
    setValue(value);
}

UniformValue::UniformValue(const Matrix4& value) :
    _type(UniformType::Matrix4)
{
    setValue(value);
}

UniformValue::UniformValue(const Color& value) :
    _type(UniformType::Color)
{
    setValue(value);
}

UniformValue::UniformValue(const Texture2::Handle& value) :
	_type(UniformType::Texture2)
{
	setValue(value);
}

UniformValue::UniformValue(const TextureCube::Handle& value) :
	_type(UniformType::TextureCube)
{
	setValue(value);
}

UniformType UniformValue::type() const
{
    return _type;
}

void UniformValue::setDefaultValue()
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
	case UniformType::TextureCube:
		_value = TextureCube::Handle();
		break;
    }
}

void UniformValue::setValue(int value)
{
    if (_type != UniformType::Int)
    {
        throw InvalidOperation("Uniform value is not of type 'Int'");
    }

    _value = value;
}

void UniformValue::setValue(double value)
{
    if (_type != UniformType::Float)
    {
        throw InvalidOperation("Uniform value is not of type 'Float'");
    }

    _value = value;
}

void UniformValue::setValue(const Vector2& value)
{
    if (_type != UniformType::Vector2)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector2'");
    }

    _value = value;
}

void UniformValue::setValue(const Vector3& value)
{
    if (_type != UniformType::Vector3)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector3'");
    }

    _value = value;
}

void UniformValue::setValue(const Vector4& value)
{
    if (_type != UniformType::Vector4)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector4'");
    }

    _value = value;
}

void UniformValue::setValue(const Matrix4& value)
{
    if (_type != UniformType::Matrix4)
    {
        throw InvalidOperation("Uniform value is not of type 'Matrix4'");
    }

    _value = value;
}

void UniformValue::setValue(const Color& value)
{
    if (_type != UniformType::Color)
    {
        throw InvalidOperation("Uniform value is not of type 'Color'");
    }

    _value = value;
}

void UniformValue::setValue(const Texture2::Handle& value)
{
	if (_type != UniformType::Texture2)
	{
		throw InvalidOperation("Uniform value is not of type 'Texture2'");
	}

	_value = value;
}

void UniformValue::setValue(const TextureCube::Handle& value)
{
	if (_type != UniformType::TextureCube)
	{
		throw InvalidOperation("Uniform value is not of type 'TextureCube'");
	}

	_value = value;
}

int UniformValue::asInt() const
{
    if (_type != UniformType::Int)
    {
        throw InvalidOperation("Uniform value is not of type 'Int'");
    }

    return _value.as<int>();
}

double UniformValue::asDouble() const
{
    if (_type != UniformType::Float)
    {
        throw InvalidOperation("Uniform value is not of type 'Float'");
    }

    return _value.as<double>();
}

Vector2 UniformValue::asVector2() const
{
    if (_type != UniformType::Vector2)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector2'");
    }

    return _value.as<Vector2>();
}

Vector3 UniformValue::asVector3() const
{
    if (_type != UniformType::Vector3)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector3'");
    }

    return _value.as<Vector3>();
}

Vector4 UniformValue::asVector4() const
{
    if (_type != UniformType::Vector4)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector4'");
    }

    return _value.as<Vector4>();
}

Matrix4 UniformValue::asMatrix4() const
{
    if (_type != UniformType::Matrix4)
    {
        throw InvalidOperation("Uniform value is not of type 'Matrix4'");
    }

    return _value.as<Matrix4>();
}

Color UniformValue::asColor() const
{
    if (_type != UniformType::Color)
    {
        throw InvalidOperation("Uniform value is not of type 'Color'");
    }

    return _value.as<Color>();
}

Texture2::Handle UniformValue::asTexture2() const
{
	if (_type != UniformType::Texture2)
	{
		throw InvalidOperation("Uniform value is not of type 'Texture2'");
	}

	return _value.as<Texture2::Handle>();
}

TextureCube::Handle UniformValue::asTextureCube() const
{
	if (_type != UniformType::TextureCube)
	{
		throw InvalidOperation("Uniform value is not of type 'TextureCube'");
	}

	return _value.as<TextureCube::Handle>();
}

UniformValue::operator bool() const
{
    return _value.hasValue();
}

bool UniformValue::operator==(const UniformValue& uniformValue) const
{
    // Type
    if (_type != uniformValue._type)
    {
        return false;
    }

    // Value
    switch (_type)
    {
    case UniformType::Int:
        return asInt() == uniformValue.asInt();
    case UniformType::Float:
        return asDouble() == uniformValue.asDouble();
    case UniformType::Vector2:
        return asVector2() == uniformValue.asVector2();
    case UniformType::Vector3:
        return asVector3() == uniformValue.asVector3();
    case UniformType::Vector4:
        return asVector4() == uniformValue.asVector4();
    case UniformType::Matrix4:
        return asMatrix4() == uniformValue.asMatrix4();
    case UniformType::Color:
        return asColor() == uniformValue.asColor();
	case UniformType::Texture2:
		return asTexture2() == uniformValue.asTexture2();
	case UniformType::TextureCube:
		return asTextureCube() == uniformValue.asTextureCube();
    }

    return false;
}

bool UniformValue::operator!=(const UniformValue& uniformValue) const
{
    return !(*this == uniformValue);
}

void UniformValue::encode(Encoder& encoder) const
{
    // Type
    encoder << encodeEnum("type", _type);

    // Value
    switch (_type)
    {
    case UniformType::Int:
        encoder << encodeValue("value", asInt());
        break;
    case UniformType::Float:
        encoder << encodeValue("value", asDouble());
        break;
    case UniformType::Vector2:
        encoder << encodeValue("value", asVector2());
        break;
    case UniformType::Vector3:
        encoder << encodeValue("value", asVector3());
        break;
    case UniformType::Vector4:
        encoder << encodeValue("value", asVector4());
        break;
    case UniformType::Matrix4:
        encoder << encodeValue("value", asMatrix4());
        break;
    case UniformType::Color:
        encoder << encodeValue("value", asColor());
        break;
	case UniformType::Texture2:
		encoder << encodeValue("value", asTexture2());
		break;
	case UniformType::TextureCube:
		encoder << encodeValue("value", asTextureCube());
		break;
    }
}

void UniformValue::decode(Decoder& decoder)
{
    // Type
    decoder >> decodeEnum("type", _type);

    // Value
    if (decoder.selectMember("value"))
    {
        switch (_type)
        {
        case UniformType::Int:
        {
            int value;
            decoder >> decodeValue(value);
            setValue(value);
        }
        break;
        case UniformType::Float:
        {
            double value;
            decoder >> decodeValue(value);
            setValue(value);
        }
        break;
        case UniformType::Vector2:
        {
            Vector2 value;
            decoder >> decodeValue(value);
            setValue(value);
        }
        break;
        case UniformType::Vector3:
        {
            Vector3 value;
            decoder >> decodeValue(value);
            setValue(value);
        }
        break;
        case UniformType::Vector4:
        {
            Vector4 value;
            decoder >> decodeValue(value);
            setValue(value);
        }
        break;
        case UniformType::Matrix4:
        {
            Matrix4 value;
            decoder >> decodeValue(value);
            setValue(value);
        }
        break;
        case UniformType::Color:
        {
            Color value;
            decoder >> decodeValue(value);
            setValue(value);
        }
        break;
		case UniformType::Texture2:
		{
			Texture2::Handle texture;
			decoder >> decodeValue(texture);
			setValue(texture);
		}
		break;
		case UniformType::TextureCube:
		{
			TextureCube::Handle texture;
			decoder >> decodeValue(texture);
			setValue(texture);
		}
		break;
        default:
            throw DecodeError(format("Unknown uniform type '%s'", Enum::toString(_type).c_str()));
        }
    }
    else
    {
        setDefaultValue();
    }
}
