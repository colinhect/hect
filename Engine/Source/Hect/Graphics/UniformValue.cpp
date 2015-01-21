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
    _type(UniformType_Int)
{
    setValue(value);
}

UniformValue::UniformValue(double value) :
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

UniformValue::UniformValue(const Color& value) :
    _type(UniformType_Color)
{
    setValue(value);
}

UniformValue::UniformValue(const Texture::Handle& value) :
    _type(UniformType_Texture)
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
    case UniformType_Int:
        _value = int(0);
        break;
    case UniformType_Float:
        _value = double(0.0);
        break;
    case UniformType_Vector2:
        _value = Vector2();
        break;
    case UniformType_Vector3:
        _value = Vector3();
        break;
    case UniformType_Vector4:
        _value = Vector4();
        break;
    case UniformType_Matrix4:
        _value = Matrix4();
        break;
    case UniformType_Color:
        _value = Color();
        break;
    case UniformType_Texture:
        _value = Texture::Handle();
        break;
    }
}

void UniformValue::setValue(int value)
{
    if (_type != UniformType_Int)
    {
        throw InvalidOperation("Uniform value is not of type 'Int'");
    }

    _value = value;
}

void UniformValue::setValue(double value)
{
    if (_type != UniformType_Float)
    {
        throw InvalidOperation("Uniform value is not of type 'Float'");
    }

    _value = value;
}

void UniformValue::setValue(const Vector2& value)
{
    if (_type != UniformType_Vector2)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector2'");
    }

    _value = value;
}

void UniformValue::setValue(const Vector3& value)
{
    if (_type != UniformType_Vector3)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector3'");
    }

    _value = value;
}

void UniformValue::setValue(const Vector4& value)
{
    if (_type != UniformType_Vector4)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector4'");
    }

    _value = value;
}

void UniformValue::setValue(const Matrix4& value)
{
    if (_type != UniformType_Matrix4)
    {
        throw InvalidOperation("Uniform value is not of type 'Matrix4'");
    }

    _value = value;
}

void UniformValue::setValue(const Color& value)
{
    if (_type != UniformType_Color)
    {
        throw InvalidOperation("Uniform value is not of type 'Color'");
    }

    _value = value;
}

void UniformValue::setValue(const Texture::Handle& value)
{
    if (_type != UniformType_Texture)
    {
        throw InvalidOperation("Uniform value is not of type 'Texture'");
    }

    _value = value;
}

int UniformValue::asInt() const
{
    if (_type != UniformType_Int)
    {
        throw InvalidOperation("Uniform value is not of type 'Int'");
    }

    return _value.as<int>();
}

double UniformValue::asDouble() const
{
    if (_type != UniformType_Float)
    {
        throw InvalidOperation("Uniform value is not of type 'Float'");
    }

    return _value.as<double>();
}

Vector2 UniformValue::asVector2() const
{
    if (_type != UniformType_Vector2)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector2'");
    }

    return _value.as<Vector2>();
}

Vector3 UniformValue::asVector3() const
{
    if (_type != UniformType_Vector3)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector3'");
    }

    return _value.as<Vector3>();
}

Vector4 UniformValue::asVector4() const
{
    if (_type != UniformType_Vector4)
    {
        throw InvalidOperation("Uniform value is not of type 'Vector4'");
    }

    return _value.as<Vector4>();
}

Matrix4 UniformValue::asMatrix4() const
{
    if (_type != UniformType_Matrix4)
    {
        throw InvalidOperation("Uniform value is not of type 'Matrix4'");
    }

    return _value.as<Matrix4>();
}

Color UniformValue::asColor() const
{
    if (_type != UniformType_Color)
    {
        throw InvalidOperation("Uniform value is not of type 'Color'");
    }

    return _value.as<Color>();
}

Texture::Handle UniformValue::asTexture() const
{
    if (_type != UniformType_Texture)
    {
        throw InvalidOperation("Uniform value is not of type 'Texture'");
    }

    return _value.as<Texture::Handle>();
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
    case UniformType_Int:
        return asInt() == uniformValue.asInt();
    case UniformType_Float:
        return asDouble() == uniformValue.asDouble();
    case UniformType_Vector2:
        return asVector2() == uniformValue.asVector2();
    case UniformType_Vector3:
        return asVector3() == uniformValue.asVector3();
    case UniformType_Vector4:
        return asVector4() == uniformValue.asVector4();
    case UniformType_Matrix4:
        return asMatrix4() == uniformValue.asMatrix4();
    case UniformType_Color:
        return asColor() == uniformValue.asColor();
    case UniformType_Texture:
        return asTexture() == uniformValue.asTexture();
    }

    return false;
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
        encoder << encodeValue("value", uniformValue.asInt());
        break;
    case UniformType_Float:
        encoder << encodeValue("value", uniformValue.asDouble());
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
    case UniformType_Matrix4:
        encoder << encodeValue("value", uniformValue.asMatrix4());
        break;
    case UniformType_Color:
        encoder << encodeValue("value", uniformValue.asColor());
        break;
    case UniformType_Texture:
        encoder << encodeValue("value", uniformValue.asTexture());
        break;
    }

    return encoder;
}

Decoder& operator>>(Decoder& decoder, UniformValue& uniformValue)
{
    // Type
    decoder >> decodeEnum("type", uniformValue._type);

    // Value
    if (decoder.selectMember("value"))
    {
        UniformType type = uniformValue.type();
        switch (type)
        {
        case UniformType_Int:
        {
            int value;
            decoder >> decodeValue(value);
            uniformValue.setValue(value);
        }
        break;
        case UniformType_Float:
        {
            double value;
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
        case UniformType_Matrix4:
        {
            Matrix4 value;
            decoder >> decodeValue(value);
            uniformValue.setValue(value);
        }
        break;
        case UniformType_Color:
        {
            Color value;
            decoder >> decodeValue(value);
            uniformValue.setValue(value);
        }
        break;
        case UniformType_Texture:
        {
            Texture::Handle texture;
            decoder >> decodeValue(texture);
            uniformValue.setValue(texture);
        }
        break;
        default:
            throw DecodeError(format("Unknown uniform type '%s'", Enum::toString(type).c_str()));
        }
    }
    else
    {
        uniformValue.setDefaultValue();
    }

    return decoder;
}

}