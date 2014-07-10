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
#include "UniformEncoder.h"

#include "Hect/Core/Enum.h"
#include "Hect/IO/Encoders/UniformValueEncoder.h"

using namespace hect;

void UniformEncoder::encode(const Uniform& uniform, ObjectEncoder& encoder)
{
    // Name
    encoder.encodeString("name", uniform.name());

    // We need an extra hint in binary
    if (encoder.isBinaryStream())
    {
        WriteStream& stream = encoder.binaryStream();
        stream.writeBool(uniform.hasDefaultValue());
    }

    // Default value
    if (uniform.hasDefaultValue())
    {
        ObjectEncoder defaultValueEncoder = encoder.encodeObject("defaultValue");
        uniform.defaultValue().encode(defaultValueEncoder);
    }

    // Binding
    else if (uniform.hasBinding())
    {
        encoder.encodeEnum("binding", uniform.binding());
    }
    else
    {
        throw Error("The uniform does not have a default value or a binding");
    }
}

void UniformEncoder::decode(Uniform& uniform, ObjectDecoder& decoder)
{
    // Name
    if (decoder.hasMember("name"))
    {
        uniform.setName(decoder.decodeString("name"));
    }
    else
    {
        throw Error("No uniform name specified");
    }

    // Detect if the uniform has a default value or a binding
    bool hasDefaultValue;
    if (decoder.isBinaryStream())
    {
        ReadStream& stream = decoder.binaryStream();
        hasDefaultValue = stream.readBool();
    }
    else
    {
        hasDefaultValue = decoder.hasMember("defaultValue");
    }

    // Default value
    if (hasDefaultValue)
    {
        ObjectDecoder defaultValueDecoder = decoder.decodeObject("defaultValue");

        UniformValue defaultValue;
        UniformValueEncoder::decode(defaultValue, defaultValueDecoder);
        uniform.setDefaultValue(defaultValue);
    }

    // Binding
    else if (decoder.hasMember("binding"))
    {
        uniform.setBinding(decoder.decodeEnum<UniformBinding>("binding"));
    }
    else
    {
        throw Error("No default value or binding specified");
    }
}