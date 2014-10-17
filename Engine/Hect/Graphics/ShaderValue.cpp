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
#include "ShaderValue.h"

using namespace hect;

ShaderValue::ShaderValue()
{
    setDefaultValue();
}

ShaderValue::ShaderValue(int value, ShaderValueType type) :
    _type(type)
{
    if (type != ShaderValueType_Int && type != ShaderValueType_Texture)
    {
        throw Error("Invalid shader value type");
    }

    setValue(value);
}

ShaderValue::ShaderValue(Real value) :
    _type(ShaderValueType_Float)
{
    setValue(value);
}

ShaderValue::ShaderValue(const Vector2& value) :
    _type(ShaderValueType_Vector2)
{
    setValue(value);
}

ShaderValue::ShaderValue(const Vector3& value) :
    _type(ShaderValueType_Vector3)
{
    setValue(value);
}

ShaderValue::ShaderValue(const Vector4& value) :
    _type(ShaderValueType_Vector4)
{
    setValue(value);
}

ShaderValue::ShaderValue(const Matrix4& value) :
    _type(ShaderValueType_Matrix4)
{
    setValue(value);
}

ShaderValueType ShaderValue::type() const
{
    return _type;
}

void ShaderValue::setType(ShaderValueType type)
{
    _type = type;
    setDefaultValue();
}

const void* ShaderValue::data() const
{
    switch (_type)
    {
    case ShaderValueType_Int:
    case ShaderValueType_Texture:
        return &_value.as<int>();
        break;
    case ShaderValueType_Float:
        return &_value.as<float>();
        break;
    case ShaderValueType_Vector2:
        return &_value.as<Vector2T<float>>();
        break;
    case ShaderValueType_Vector3:
        return &_value.as<Vector3T<float>>();
        break;
    case ShaderValueType_Vector4:
        return &_value.as<Vector4T<float>>();
        break;
    case ShaderValueType_Matrix4:
        return &_value.as<Matrix4T<float>>();
        break;
    default:
        throw Error("Unexpected shader value type");
    }
}

void ShaderValue::setDefaultValue()
{
    switch (_type)
    {
    case ShaderValueType_Int:
    case ShaderValueType_Texture:
        _value = static_cast<int>(0);
        break;
    case ShaderValueType_Float:
        _value = 0.0f;
        break;
    case ShaderValueType_Vector2:
        _value = Vector2T<float>();
        break;
    case ShaderValueType_Vector3:
        _value = Vector3T<float>();
        break;
    case ShaderValueType_Vector4:
        _value = Vector4T<float>();
        break;
    case ShaderValueType_Matrix4:
        _value = Matrix4T<float>();
        break;
    default:
        throw Error("Unexpected shader value type");
    }
}

void ShaderValue::setValue(int value)
{
    if (_type != ShaderValueType_Int && _type != ShaderValueType_Texture)
    {
        throw Error("Shader value is not of type 'Int' or 'Texture'");
    }

    _value = value;
}

void ShaderValue::setValue(Real value)
{
    if (_type != ShaderValueType_Float)
    {
        throw Error("Shader value is not of type 'Float'");
    }

    _value = (float)value;
}

void ShaderValue::setValue(const Vector2& value)
{
    if (_type != ShaderValueType_Vector2)
    {
        throw Error("Shader value is not of type 'Vector2'");
    }

    _value = Vector2T<float>(value);
}

void ShaderValue::setValue(const Vector3& value)
{
    if (_type != ShaderValueType_Vector3)
    {
        throw Error("Shader value is not of type 'Vector3'");
    }

    _value = Vector3T<float>(value);
}

void ShaderValue::setValue(const Vector4& value)
{
    if (_type != ShaderValueType_Vector4)
    {
        throw Error("Shader value is not of type 'Vector4'");
    }

    _value = Vector4T<float>(value);
}

void ShaderValue::setValue(const Matrix4& value)
{
    if (_type != ShaderValueType_Matrix4)
    {
        throw Error("Shader value is not of type 'Matrix4'");
    }

    _value = Matrix4T<float>(value);
}

int ShaderValue::asInt() const
{
    if (_type != ShaderValueType_Int && _type != ShaderValueType_Texture)
    {
        throw Error("Shader value is not of type 'Int' or 'Texture'");
    }

    return _value.as<int>();
}

Real ShaderValue::asReal() const
{
    if (_type != ShaderValueType_Float)
    {
        throw Error("Shader value is not of type 'Float'");
    }

    return static_cast<Real>(_value.as<float>());
}

Vector2 ShaderValue::asVector2() const
{
    if (_type != ShaderValueType_Vector2)
    {
        throw Error("Shader value is not of type 'Vector2'");
    }

    return static_cast<Vector2>(_value.as<Vector2T<float>>());
}

Vector3 ShaderValue::asVector3() const
{
    if (_type != ShaderValueType_Vector3)
    {
        throw Error("Shader value is not of type 'Vector3'");
    }

    return static_cast<Vector3>(_value.as<Vector3T<float>>());
}

Vector4 ShaderValue::asVector4() const
{
    if (_type != ShaderValueType_Vector4)
    {
        throw Error("Shader value is not of type 'Vector4'");
    }

    return static_cast<Vector4>(_value.as<Vector4T<float>>());
}

Matrix4 ShaderValue::asMatrix4() const
{
    if (_type != ShaderValueType_Matrix4)
    {
        throw Error("Shader value is not of type 'Matrix4'");
    }

    return static_cast<Matrix4>(_value.as<Matrix4T<float>>());
}

bool ShaderValue::operator==(const ShaderValue& shaderValue) const
{
    // Type
    if (_type != shaderValue._type)
    {
        return false;
    }

    // Value
    switch (_type)
    {
    case ShaderValueType_Int:
    case ShaderValueType_Texture:
        return asInt() == shaderValue.asInt();
        break;
    case ShaderValueType_Float:
        return asReal() == shaderValue.asReal();
        break;
    case ShaderValueType_Vector2:
        return asVector2() == shaderValue.asVector2();
        break;
    case ShaderValueType_Vector3:
        return asVector3() == shaderValue.asVector3();
        break;
    case ShaderValueType_Vector4:
        return asVector4() == shaderValue.asVector4();
        break;
    default:
        throw Error("Unsupported shader value type");
    }
}

bool ShaderValue::operator!=(const ShaderValue& shaderValue) const
{
    return !(*this == shaderValue);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const ShaderValue& shaderValue)
{
    // Type
    encoder << encodeEnum("type", shaderValue.type());

    // Value
    switch (shaderValue.type())
    {
    case ShaderValueType_Int:
    case ShaderValueType_Texture:
        encoder << encodeValue("value", shaderValue.asInt());
        break;
    case ShaderValueType_Float:
        encoder << encodeValue("value", shaderValue.asReal());
        break;
    case ShaderValueType_Vector2:
        encoder << encodeValue("value", shaderValue.asVector2());
        break;
    case ShaderValueType_Vector3:
        encoder << encodeValue("value", shaderValue.asVector3());
        break;
    case ShaderValueType_Vector4:
        encoder << encodeValue("value", shaderValue.asVector4());
        break;
    default:
        throw Error("Unsupported shader value type");
    }
    return encoder;
}

Decoder& operator>>(Decoder& decoder, ShaderValue& shaderValue)
{
    // Type
    if (decoder.selectMember("type"))
    {
        ShaderValueType type;
        decoder >> decodeEnum(type);
        shaderValue.setType(type);
    }
    else
    {
        throw Error("No shader value type specified");
    }

    // Value
    if (decoder.selectMember("value"))
    {
        switch (shaderValue.type())
        {
        case ShaderValueType_Int:
        case ShaderValueType_Texture:
            shaderValue.setValue(decoder.decodeInt32());
            break;
        case ShaderValueType_Float:
        {
            Real value;
            decoder >> decodeValue(value);
            shaderValue.setValue(value);
        }
        break;
        case ShaderValueType_Vector2:
        {
            Vector2 value;
            decoder >> decodeValue(value);
            shaderValue.setValue(value);
        }
        break;
        case ShaderValueType_Vector3:
        {
            Vector3 value;
            decoder >> decodeValue(value);
            shaderValue.setValue(value);
        }
        break;
        case ShaderValueType_Vector4:
        {
            Vector4 value;
            decoder >> decodeValue(value);
            shaderValue.setValue(value);
        }
        break;
        default:
            throw Error("Unsupported shader value type");
        }
    }

    return decoder;
}

}