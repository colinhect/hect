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
#include "UniformValue.h"

#include "Hect/IO/Encoders/UniformValueEncoder.h"

using namespace hect;

UniformValue::UniformValue() :
    _type(UniformType::Float)
{
    setDefaultValue();
}

UniformValue::UniformValue(int value, UniformType::Enum type) :
    _type(type)
{
    if (type != UniformType::Int && type != UniformType::Texture)
    {
        throw Error("Invalid uniform value type");
    }

    setValue(value);
}

UniformValue::UniformValue(Real value) :
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

UniformType::Enum UniformValue::type() const
{
    return _type;
}

void UniformValue::setType(UniformType::Enum type)
{
    _type = type;
    setDefaultValue();
}

const void* UniformValue::data() const
{
    switch (_type)
    {
    case UniformType::Int:
    case UniformType::Texture:
        return &_value.as<int>();
        break;
    case UniformType::Float:
        return &_value.as<float>();
        break;
    case UniformType::Vector2:
        return &_value.as<Vector2T<float>>();
        break;
    case UniformType::Vector3:
        return &_value.as<Vector3T<float>>();
        break;
    case UniformType::Vector4:
        return &_value.as<Vector4T<float>>();
        break;
    case UniformType::Matrix4:
        return &_value.as<Matrix4T<float>>();
        break;
    default:
        throw Error("Unexpected uniform value type");
    }
}

void UniformValue::setDefaultValue()
{
    switch (_type)
    {
    case UniformType::Int:
    case UniformType::Texture:
        _value = (int)0;
        break;
    case UniformType::Float:
        _value = 0.0f;
        break;
    case UniformType::Vector2:
        _value = Vector2T<float>();
        break;
    case UniformType::Vector3:
        _value = Vector3T<float>();
        break;
    case UniformType::Vector4:
        _value = Vector4T<float>();
        break;
    case UniformType::Matrix4:
        _value = Matrix4T<float>();
        break;
    default:
        throw Error("Unexpected uniform value type");
    }
}

void UniformValue::setValue(int value)
{
    if (_type != UniformType::Int && _type != UniformType::Texture)
    {
        throw Error("Uniform value is not of type 'Int' or 'Texture'");
    }

    _value = value;
}

void UniformValue::setValue(Real value)
{
    if (_type != UniformType::Float)
    {
        throw Error("Uniform value is not of type 'Float'");
    }

    _value = (float)value;
}

void UniformValue::setValue(const Vector2& value)
{
    if (_type != UniformType::Vector2)
    {
        throw Error("Uniform value is not of type 'Vector2'");
    }

    _value = Vector2T<float>(value);
}

void UniformValue::setValue(const Vector3& value)
{
    if (_type != UniformType::Vector3)
    {
        throw Error("Uniform value is not of type 'Vector3'");
    }

    _value = Vector3T<float>(value);
}

void UniformValue::setValue(const Vector4& value)
{
    if (_type != UniformType::Vector4)
    {
        throw Error("Uniform value is not of type 'Vector4'");
    }

    _value = Vector4T<float>(value);
}

void UniformValue::setValue(const Matrix4& value)
{
    if (_type != UniformType::Matrix4)
    {
        throw Error("Uniform value is not of type 'Matrix4'");
    }

    _value = Matrix4T<float>(value);
}

int UniformValue::asInt() const
{
    if (_type != UniformType::Int && _type != UniformType::Texture)
    {
        throw Error("Uniform value is not of type 'Int' or 'Texture'");
    }

    return _value.as<int>();
}

Real UniformValue::asReal() const
{
    if (_type != UniformType::Float)
    {
        throw Error("Uniform value is not of type 'Float'");
    }

    return (Real)_value.as<float>();
}

Vector2 UniformValue::asVector2() const
{
    if (_type != UniformType::Vector2)
    {
        throw Error("Uniform value is not of type 'Vector2'");
    }

    return (Vector2)_value.as<Vector2T<float>>();
}

Vector3 UniformValue::asVector3() const
{
    if (_type != UniformType::Vector3)
    {
        throw Error("Uniform value is not of type 'Vector3'");
    }

    return (Vector3)_value.as<Vector3T<float>>();
}

Vector4 UniformValue::asVector4() const
{
    if (_type != UniformType::Vector4)
    {
        throw Error("Uniform value is not of type 'Vector4'");
    }

    return (Vector4)_value.as<Vector4T<float>>();
}

Matrix4 UniformValue::asMatrix4() const
{
    if (_type != UniformType::Matrix4)
    {
        throw Error("Uniform value is not of type 'Matrix4'");
    }

    return (Matrix4)_value.as<Matrix4T<float>>();
}

void UniformValue::encode(ObjectEncoder& encoder) const
{
    UniformValueEncoder::encode(*this, encoder);
}

void UniformValue::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    assetCache;
    UniformValueEncoder::decode(*this, decoder);
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
    case UniformType::Texture:
        return asInt() == uniformValue.asInt();
        break;
    case UniformType::Float:
        return asReal() == uniformValue.asReal();
        break;
    case UniformType::Vector2:
        return asVector2() == uniformValue.asVector2();
        break;
    case UniformType::Vector3:
        return asVector3() == uniformValue.asVector3();
        break;
    case UniformType::Vector4:
        return asVector4() == uniformValue.asVector4();
        break;
    default:
        throw Error("Unsupported uniform value type");
    }
}

bool UniformValue::operator!=(const UniformValue& uniformValue) const
{
    return !(*this == uniformValue);
}