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
#include "UniformValueEncoder.h"

#include "Hect/Reflection/Type.h"

using namespace hect;

void UniformValueEncoder::encode(const UniformValue& uniformValue, ObjectEncoder& encoder)
{
    // Type
    if (encoder.isBinaryStream())
    {
        WriteStream& stream = encoder.binaryStream();
        stream.writeUnsignedByte((uint8_t)uniformValue.type());
    }
    else
    {
        encoder.encodeString("type", Enum::toString(uniformValue.type()));
    }

    // Value
    switch (uniformValue.type())
    {
    case UniformType::Int:
    case UniformType::Texture:
        encoder.encodeInt("value", uniformValue.asInt());
        break;
    case UniformType::Float:
        encoder.encodeReal("value", uniformValue.asReal());
        break;
    case UniformType::Vector2:
        encoder.encodeVector2("value", uniformValue.asVector2());
        break;
    case UniformType::Vector3:
        encoder.encodeVector3("value", uniformValue.asVector3());
        break;
    case UniformType::Vector4:
        encoder.encodeVector4("value", uniformValue.asVector4());
        break;
    default:
        throw Error("Unsupported uniform value type");
    }
}

void UniformValueEncoder::decode(UniformValue& uniformValue, ObjectDecoder& decoder)
{
    // Type
    if (decoder.isBinaryStream())
    {
        ReadStream& stream = decoder.binaryStream();
        uniformValue.setType((UniformType::Enum)stream.readUnsignedByte());
    }
    else if (decoder.hasMember("type"))
    {
        uniformValue.setType(Enum::fromString<UniformType::Enum>(decoder.decodeString("type")));
    }
    else
    {
        throw Error("No uniform type specified");
    }

    // Value
    if (decoder.hasMember("value"))
    {
        switch (uniformValue.type())
        {
        case UniformType::Int:
        case UniformType::Texture:
            uniformValue.setValue(decoder.decodeInt("value"));
            break;
        case UniformType::Float:
            uniformValue.setValue(decoder.decodeReal("value"));
            break;
        case UniformType::Vector2:
            uniformValue.setValue(decoder.decodeVector2("value"));
            break;
        case UniformType::Vector3:
            uniformValue.setValue(decoder.decodeVector3("value"));
            break;
        case UniformType::Vector4:
            uniformValue.setValue(decoder.decodeVector4("value"));
            break;
        default:
            throw Error("Unsupported uniform value type");
        }
    }
    else
    {
        throw Error("No uniform value specified");
    }
}