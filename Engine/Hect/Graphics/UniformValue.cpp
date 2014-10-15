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

using namespace hect;

UniformValue::UniformValue()
{
    setDefaultValue();
}

UniformValue::UniformValue(int value, UniformType type) :
    _type(type)
{
    if (type != UniformType_Int && type != UniformType_Texture)
    {
        throw Error("Invalid uniform value type");
    }

    setValue(value);
}

UniformValue::UniformValue(Real value) :
    _type(UniformType_Float)
{
    setValue(value);
}

UniformValue::UniformValue(const Vector2& value) :
    _type(UniformType_Vector2)
{
    setValue(value);
}

UniformValue::UniformValue(const Vector3& value) :
    _type(UniformType_Vector3)
{
    setValue(value);
}

UniformValue::UniformValue(const Vector4& value) :
    _type(UniformType_Vector4)
{
    setValue(value);
}

UniformValue::UniformValue(const Matrix4& value) :
    _type(UniformType_Matrix4)
{
    setValue(value);
}

UniformType UniformValue::type() const
{
    return _type;
}

void UniformValue::setType(UniformType type)
{
    _type = type;
    setDefaultValue();
}

const void* UniformValue::data() const
{
    switch (_type)
    {
    case UniformType_Int:
    case UniformType_Texture:
        return &_value.as<int>();
        break;
    case UniformType_Float:
        return &_value.as<float>();
        break;
    case UniformType_Vector2:
        return &_value.as<Vector2T<float>>();
        break;
    case UniformType_Vector3:
        return &_value.as<Vector3T<float>>();
        break;
    case UniformType_Vector4:
        return &_value.as<Vector4T<float>>();
        break;
    case UniformType_Matrix4:
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
    case UniformType_Int:
    case UniformType_Texture:
        _value = static_cast<int>(0);
        break;
    case UniformType_Float:
        _value = 0.0f;
        break;
    case UniformType_Vector2:
        _value = Vector2T<float>();
        break;
    case UniformType_Vector3:
        _value = Vector3T<float>();
        break;
    case UniformType_Vector4:
        _value = Vector4T<float>();
        break;
    case UniformType_Matrix4:
        _value = Matrix4T<float>();
        break;
    default:
        throw Error("Unexpected uniform value type");
    }
}

void UniformValue::setValue(int value)
{
    if (_type != UniformType_Int && _type != UniformType_Texture)
    {
        throw Error("Uniform value is not of type 'Int' or 'Texture'");
    }

    _value = value;
}

void UniformValue::setValue(Real value)
{
    if (_type != UniformType_Float)
    {
        throw Error("Uniform value is not of type 'Float'");
    }

    _value = (float)value;
}

void UniformValue::setValue(const Vector2& value)
{
    if (_type != UniformType_Vector2)
    {
        throw Error("Uniform value is not of type 'Vector2'");
    }

    _value = Vector2T<float>(value);
}

void UniformValue::setValue(const Vector3& value)
{
    if (_type != UniformType_Vector3)
    {
        throw Error("Uniform value is not of type 'Vector3'");
    }

    _value = Vector3T<float>(value);
}

void UniformValue::setValue(const Vector4& value)
{
    if (_type != UniformType_Vector4)
    {
        throw Error("Uniform value is not of type 'Vector4'");
    }

    _value = Vector4T<float>(value);
}

void UniformValue::setValue(const Matrix4& value)
{
    if (_type != UniformType_Matrix4)
    {
        throw Error("Uniform value is not of type 'Matrix4'");
    }

    _value = Matrix4T<float>(value);
}

int UniformValue::asInt() const
{
    if (_type != UniformType_Int && _type != UniformType_Texture)
    {
        throw Error("Uniform value is not of type 'Int' or 'Texture'");
    }

    return _value.as<int>();
}

Real UniformValue::asReal() const
{
    if (_type != UniformType_Float)
    {
        throw Error("Uniform value is not of type 'Float'");
    }

    return static_cast<Real>(_value.as<float>());
}

Vector2 UniformValue::asVector2() const
{
    if (_type != UniformType_Vector2)
    {
        throw Error("Uniform value is not of type 'Vector2'");
    }

    return static_cast<Vector2>(_value.as<Vector2T<float>>());
}

Vector3 UniformValue::asVector3() const
{
    if (_type != UniformType_Vector3)
    {
        throw Error("Uniform value is not of type 'Vector3'");
    }

    return static_cast<Vector3>(_value.as<Vector3T<float>>());
}

Vector4 UniformValue::asVector4() const
{
    if (_type != UniformType_Vector4)
    {
        throw Error("Uniform value is not of type 'Vector4'");
    }

    return static_cast<Vector4>(_value.as<Vector4T<float>>());
}

Matrix4 UniformValue::asMatrix4() const
{
    if (_type != UniformType_Matrix4)
    {
        throw Error("Uniform value is not of type 'Matrix4'");
    }

    return static_cast<Matrix4>(_value.as<Matrix4T<float>>());
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
    case UniformType_Int:
    case UniformType_Texture:
        return asInt() == uniformValue.asInt();
        break;
    case UniformType_Float:
        return asReal() == uniformValue.asReal();
        break;
    case UniformType_Vector2:
        return asVector2() == uniformValue.asVector2();
        break;
    case UniformType_Vector3:
        return asVector3() == uniformValue.asVector3();
        break;
    case UniformType_Vector4:
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

namespace hect
{

Encoder& operator<<(Encoder& encoder, const UniformValue& uniformValue)
{
    // Type
    encoder << encodeEnum("type", uniformValue.type());

    // Value
    switch (uniformValue.type())
    {
    case UniformType_Int:
    case UniformType_Texture:
        encoder << encodeValue("value", uniformValue.asInt());
        break;
    case UniformType_Float:
        encoder << encodeValue("value", uniformValue.asReal());
        break;
    case UniformType_Vector2:
        encoder << encodeValue("value", uniformValue.asVector2());
        break;
    case UniformType_Vector3:
        encoder << encodeValue("value", uniformValue.asVector3());
        break;
    case UniformType_Vector4:
        encoder << encodeValue("value", uniformValue.asVector4());
        break;
    default:
        throw Error("Unsupported uniform value type");
    }
    return encoder;
}

Decoder& operator>>(Decoder& decoder, UniformValue& uniformValue)
{
    // Type
    if (decoder.selectMember("type"))
    {
        UniformType type;
        decoder >> decodeEnum(type);
        uniformValue.setType(type);
    }
    else
    {
        throw Error("No uniform type specified");
    }

    // Value
    if (decoder.selectMember("value"))
    {
        switch (uniformValue.type())
        {
        case UniformType_Int:
        case UniformType_Texture:
            uniformValue.setValue(decoder.decodeInt32());
            break;
        case UniformType_Float:
        {
            Real value;
            decoder >> decodeValue(value);
            uniformValue.setValue(value);
        }
        break;
        case UniformType_Vector2:
        {
            Vector2 value;
            decoder >> decodeValue(value);
            uniformValue.setValue(value);
        }
        break;
        case UniformType_Vector3:
        {
            Vector3 value;
            decoder >> decodeValue(value);
            uniformValue.setValue(value);
        }
        break;
        case UniformType_Vector4:
        {
            Vector4 value;
            decoder >> decodeValue(value);
            uniformValue.setValue(value);
        }
        break;
        default:
            throw Error("Unsupported uniform value type");
        }
    }

    return decoder;
}

}