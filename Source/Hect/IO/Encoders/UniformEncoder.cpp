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
        if (encoder.isBinaryStream())
        {
            WriteStream& stream = encoder.binaryStream();
            stream.writeUnsignedByte((uint8_t)uniform.binding());
        }
        else
        {
            encoder.encodeString("binding", uniformBindingToString(uniform.binding()));
        }
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
        if (decoder.isBinaryStream())
        {
            ReadStream& stream = decoder.binaryStream();
            uniform.setBinding((UniformBinding)stream.readUnsignedByte());
        }
        else
        {
            uniform.setBinding(uniformBindingFromString(decoder.decodeString("binding")));
        }
    }
    else
    {
        throw Error("No default value or binding specified");
    }
}

UniformBinding UniformEncoder::uniformBindingFromString(const std::string& value)
{
    static std::map<std::string, UniformBinding> uniformBindings;

    if (uniformBindings.empty())
    {
        uniformBindings["None"] = UniformBinding::None;
        uniformBindings["RenderTargetSize"] = UniformBinding::RenderTargetSize;
        uniformBindings["CameraPosition"] = UniformBinding::CameraPosition;
        uniformBindings["CameraUp"] = UniformBinding::CameraUp;
        uniformBindings["ViewMatrix"] = UniformBinding::ViewMatrix;
        uniformBindings["ProjectionMatrix"] = UniformBinding::ProjectionMatrix;
        uniformBindings["ViewProjectionMatrix"] = UniformBinding::ViewProjectionMatrix;
        uniformBindings["ModelMatrix"] = UniformBinding::ModelMatrix;
        uniformBindings["ModelViewMatrix"] = UniformBinding::ModelViewMatrix;
        uniformBindings["ModelViewProjectionMatrix"] = UniformBinding::ModelViewProjectionMatrix;
    }

    auto it = uniformBindings.find(value);
    if (it == uniformBindings.end())
    {
        throw Error(format("Invalid uniform binding '%s'", value.c_str()));
    }

    return (*it).second;
}

std::string UniformEncoder::uniformBindingToString(UniformBinding uniformBinding)
{
    static std::map<UniformBinding, std::string> uniformBindingNames;

    if (uniformBindingNames.empty())
    {
        uniformBindingNames[UniformBinding::None] = "None";
        uniformBindingNames[UniformBinding::RenderTargetSize] = "RenderTargetSize";
        uniformBindingNames[UniformBinding::CameraPosition] = "CameraPosition";
        uniformBindingNames[UniformBinding::CameraUp] = "CameraUp";
        uniformBindingNames[UniformBinding::ViewMatrix] = "ViewMatrix";
        uniformBindingNames[UniformBinding::ProjectionMatrix] = "ProjectionMatrix";
        uniformBindingNames[UniformBinding::ViewProjectionMatrix] = "ViewProjectionMatrix";
        uniformBindingNames[UniformBinding::ModelMatrix] = "ModelMatrix";
        uniformBindingNames[UniformBinding::ModelViewMatrix] = "ModelViewMatrix";
        uniformBindingNames[UniformBinding::ModelViewProjectionMatrix] = "ModelViewProjectionMatrix";
    }

    auto it = uniformBindingNames.find(uniformBinding);
    if (it == uniformBindingNames.end())
    {
        throw Error(format("Invalid uniform binding '%d'", uniformBinding));
    }

    return (*it).second;
}