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
#include "MaterialValue.h"

#include "Hect/Graphics/Texture.h"
#include "Hect/IO/AssetCache.h"
#include "Hect/IO/Path.h"

using namespace hect;

MaterialValue::MaterialValue()
{
}

MaterialValue::MaterialValue(MaterialValueType type) :
    _type(type)
{
    setDefaultValue();
}

MaterialValue::MaterialValue(int value) :
    _type(MaterialValueType_Int)
{
    setValue(value);
}

MaterialValue::MaterialValue(Real value) :
    _type(MaterialValueType_Float)
{
    setValue(value);
}

MaterialValue::MaterialValue(const Vector2& value) :
    _type(MaterialValueType_Vector2)
{
    setValue(value);
}

MaterialValue::MaterialValue(const Vector3& value) :
    _type(MaterialValueType_Vector3)
{
    setValue(value);
}

MaterialValue::MaterialValue(const Vector4& value) :
    _type(MaterialValueType_Vector4)
{
    setValue(value);
}

MaterialValue::MaterialValue(const Matrix4& value) :
    _type(MaterialValueType_Matrix4)
{
    setValue(value);
}

MaterialValue::MaterialValue(const AssetHandle<Texture>& value) :
    _type(MaterialValueType_Texture)
{
    setValue(value);
}

MaterialValueType MaterialValue::type() const
{
    return _type;
}

void MaterialValue::setDefaultValue()
{
    switch (_type)
    {
    case MaterialValueType_Null:
        _value = Any();
        break;
    case MaterialValueType_Int:
        _value = int(0);
        break;
    case MaterialValueType_Float:
        _value = Real(0);
        break;
    case MaterialValueType_Vector2:
        _value = Vector2();
        break;
    case MaterialValueType_Vector3:
        _value = Vector3();
        break;
    case MaterialValueType_Vector4:
        _value = Vector4();
        break;
    case MaterialValueType_Matrix4:
        _value = Matrix4();
        break;
    case MaterialValueType_Texture:
        _value = AssetHandle<Texture>();
        break;
    default:
        throw Error("Unexpected shader value type");
    }
}

void MaterialValue::setValue(int value)
{
    if (_type != MaterialValueType_Int)
    {
        throw Error("Material value is not of type 'Int'");
    }

    _value = value;
}

void MaterialValue::setValue(Real value)
{
    if (_type != MaterialValueType_Float)
    {
        throw Error("Material value is not of type 'Float'");
    }

    _value = value;
}

void MaterialValue::setValue(const Vector2& value)
{
    if (_type != MaterialValueType_Vector2)
    {
        throw Error("Material value is not of type 'Vector2'");
    }

    _value = value;
}

void MaterialValue::setValue(const Vector3& value)
{
    if (_type != MaterialValueType_Vector3)
    {
        throw Error("Material value is not of type 'Vector3'");
    }

    _value = value;
}

void MaterialValue::setValue(const Vector4& value)
{
    if (_type != MaterialValueType_Vector4)
    {
        throw Error("Material value is not of type 'Vector4'");
    }

    _value = value;
}

void MaterialValue::setValue(const Matrix4& value)
{
    if (_type != MaterialValueType_Matrix4)
    {
        throw Error("Material value is not of type 'Matrix4'");
    }

    _value = value;
}

void MaterialValue::setValue(const AssetHandle<Texture>& value)
{
    if (_type != MaterialValueType_Texture)
    {
        throw Error("Material value is not of type 'Texture'");
    }

    _value = value;
}

int MaterialValue::asInt() const
{
    if (_type != MaterialValueType_Int)
    {
        throw Error("Material value is not of type 'Int'");
    }

    return _value.as<int>();
}

Real MaterialValue::asReal() const
{
    if (_type != MaterialValueType_Float)
    {
        throw Error("Material value is not of type 'Float'");
    }

    return _value.as<Real>();
}

Vector2 MaterialValue::asVector2() const
{
    if (_type != MaterialValueType_Vector2)
    {
        throw Error("Material value is not of type 'Vector2'");
    }

    return _value.as<Vector2>();
}

Vector3 MaterialValue::asVector3() const
{
    if (_type != MaterialValueType_Vector3)
    {
        throw Error("Material value is not of type 'Vector3'");
    }

    return _value.as<Vector3>();
}

Vector4 MaterialValue::asVector4() const
{
    if (_type != MaterialValueType_Vector4)
    {
        throw Error("Material value is not of type 'Vector4'");
    }

    return _value.as<Vector4>();
}

Matrix4 MaterialValue::asMatrix4() const
{
    if (_type != MaterialValueType_Matrix4)
    {
        throw Error("Material value is not of type 'Matrix4'");
    }

    return _value.as<Matrix4>();
}

AssetHandle<Texture> MaterialValue::asTexture() const
{
    if (_type != MaterialValueType_Texture)
    {
        throw Error("Material value is not of type 'Texture'");
    }

    return _value.as<AssetHandle<Texture>>();
}

bool MaterialValue::operator==(const MaterialValue& materialValue) const
{
    // Type
    if (_type != materialValue._type)
    {
        return false;
    }

    // Value
    switch (_type)
    {
    case MaterialValueType_Null:
        return true;
        break;
    case MaterialValueType_Int:
        return asInt() == materialValue.asInt();
        break;
    case MaterialValueType_Float:
        return asReal() == materialValue.asReal();
        break;
    case MaterialValueType_Vector2:
        return asVector2() == materialValue.asVector2();
        break;
    case MaterialValueType_Vector3:
        return asVector3() == materialValue.asVector3();
        break;
    case MaterialValueType_Vector4:
        return asVector4() == materialValue.asVector4();
        break;
    case MaterialValueType_Texture:
        return asTexture() == materialValue.asTexture();
    default:
        throw Error("Unsupported material value type");
    }
}

bool MaterialValue::operator!=(const MaterialValue& materialValue) const
{
    return !(*this == materialValue);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const MaterialValue& materialValue)
{
    // Type
    encoder << encodeEnum("type", materialValue.type());

    // Value
    switch (materialValue.type())
    {
    case MaterialValueType_Null:
        break;
    case MaterialValueType_Int:
        encoder << encodeValue("value", materialValue.asInt());
        break;
    case MaterialValueType_Float:
        encoder << encodeValue("value", materialValue.asReal());
        break;
    case MaterialValueType_Vector2:
        encoder << encodeValue("value", materialValue.asVector2());
        break;
    case MaterialValueType_Vector3:
        encoder << encodeValue("value", materialValue.asVector3());
        break;
    case MaterialValueType_Vector4:
        encoder << encodeValue("value", materialValue.asVector4());
        break;
    case MaterialValueType_Texture:
        encoder << encodeValue("value", materialValue.asTexture());
        break;
    default:
        throw Error("Unsupported shader value type");
    }
    return encoder;
}

Decoder& operator>>(Decoder& decoder, MaterialValue& materialValue)
{
    // Type
    decoder >> decodeEnum("type", materialValue._type);

    // Value
    if (decoder.selectMember("value"))
    {
        switch (materialValue.type())
        {
        case MaterialValueType_Null:
        {
            throw Error("Cannot specify value for null material value");
        }
        break;
        case MaterialValueType_Int:
        {
            int value;
            decoder >> decodeValue(value);
            materialValue.setValue(value);
        }
        break;
        case MaterialValueType_Float:
        {
            Real value;
            decoder >> decodeValue(value);
            materialValue.setValue(value);
        }
        break;
        case MaterialValueType_Vector2:
        {
            Vector2 value;
            decoder >> decodeValue(value);
            materialValue.setValue(value);
        }
        break;
        case MaterialValueType_Vector3:
        {
            Vector3 value;
            decoder >> decodeValue(value);
            materialValue.setValue(value);
        }
        break;
        case MaterialValueType_Vector4:
        {
            Vector4 value;
            decoder >> decodeValue(value);
            materialValue.setValue(value);
        }
        break;
        case MaterialValueType_Texture:
        {
            AssetHandle<Texture> texture;
            decoder >> decodeValue(texture);
            materialValue.setValue(texture);
        }
        break;
        default:
            throw Error("Unexpected material value type");
        }
    }

    return decoder;
}

}