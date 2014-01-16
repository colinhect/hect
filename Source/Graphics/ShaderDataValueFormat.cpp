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
#include "Hect.h"

using namespace hect;

void ShaderDataValueFormat::load(Shader& shader, const std::string& name, const DataValue& dataValue, AssetCache& assetCache)
{
    AssetHandle<ShaderModule>::Array modules;

    // Add all modules
    for (const DataValue& module : dataValue["modules"])
    {
        AssetHandle<ShaderModule> moduleHandle = assetCache.getHandle<ShaderModule>(module.asString());
        modules.push_back(moduleHandle);
    }

    Uniform::Array uniforms;

    // Add all uniforms
    for (std::string& name : dataValue["uniforms"].memberNames())
    {
        uniforms.push_back(_parseUniform(name, dataValue["uniforms"][name]));
    }

    shader = Shader(name, modules, uniforms);
}

UniformValue ShaderDataValueFormat::parseValue(UniformType type, const DataValue& dataValue)
{
    switch (type)
    {
    case UniformType::Int:
    case UniformType::Texture:
        return UniformValue(dataValue.asInt(), type);
    case UniformType::Float:
        return UniformValue((float)dataValue.asDouble());
    case UniformType::Vector2:
        return UniformValue(dataValue.asVector2());
    case UniformType::Vector3:
        return UniformValue(dataValue.asVector3());
    case UniformType::Vector4:
        return UniformValue(dataValue.asVector4());
    case UniformType::Matrix4:
        return UniformValue(dataValue.asMatrix4());
    default:
        return UniformValue();
    }
}

Uniform ShaderDataValueFormat::_parseUniform(const std::string& name, const DataValue& dataValue)
{
    if (!dataValue["type"].isNull())
    {
        auto type = _parseType(dataValue["type"]);

        const DataValue& defaultValue = dataValue["defaultValue"];
        if (!defaultValue.isNull())
        {
            return Uniform(name, parseValue(type, defaultValue));
        }
        else
        {
            return Uniform(name, type);
        }
    }
    else if (!dataValue["binding"].isNull())
    {
        auto binding = _parseUniformBinding(dataValue["binding"]);
        return Uniform(name, binding);
    }
    else
    {
        throw Error("No type or binding specified");
    }
}

UniformBinding ShaderDataValueFormat::_parseUniformBinding(const DataValue& dataValue)
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

    auto it = uniformBindings.find(dataValue.asString());
    if (it == uniformBindings.end())
    {
        throw Error(format("Invalid uniform binding '%s'", dataValue.asString().c_str()));
    }

    return (*it).second;
}

UniformType ShaderDataValueFormat::_parseType(const DataValue& dataValue)
{
    static std::map<std::string, UniformType> valueTypes;

    if (valueTypes.empty())
    {
        valueTypes["Int"] = UniformType::Int;
        valueTypes["Float"] = UniformType::Float;
        valueTypes["Vector2"] = UniformType::Vector2;
        valueTypes["Vector3"] = UniformType::Vector3;
        valueTypes["Vector4"] = UniformType::Vector4;
        valueTypes["Matrix4"] = UniformType::Matrix4;
        valueTypes["Texture"] = UniformType::Texture;
    }

    auto it = valueTypes.find(dataValue.asString());
    if (it == valueTypes.end())
    {
        throw Error(format("Invalid uniform type '%s'", dataValue.asString().c_str()));
    }

    return (*it).second;
}