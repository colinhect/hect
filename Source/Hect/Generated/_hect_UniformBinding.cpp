// Generated by reflect.py
#include <Hect/Core/Enum.h>
#include <Hect/Core/Error.h>
#include <Hect/Core/Format.h>

#include "..\Graphics\Uniform.h"

using namespace hect;

template <>
const std::string& Enum::toString<UniformBinding>(UniformBinding value)
{
    static std::map<UniformBinding, std::string> _valueToString;
    if (_valueToString.empty())
    {
        _valueToString[UniformBinding_None] = "None";
        _valueToString[UniformBinding_RenderTargetSize] = "RenderTargetSize";
        _valueToString[UniformBinding_CameraPosition] = "CameraPosition";
        _valueToString[UniformBinding_CameraFront] = "CameraFront";
        _valueToString[UniformBinding_CameraUp] = "CameraUp";
        _valueToString[UniformBinding_ViewMatrix] = "ViewMatrix";
        _valueToString[UniformBinding_ProjectionMatrix] = "ProjectionMatrix";
        _valueToString[UniformBinding_ViewProjectionMatrix] = "ViewProjectionMatrix";
        _valueToString[UniformBinding_ModelMatrix] = "ModelMatrix";
        _valueToString[UniformBinding_ModelViewMatrix] = "ModelViewMatrix";
        _valueToString[UniformBinding_ModelViewProjectionMatrix] = "ModelViewProjectionMatrix";
    }
    auto it = _valueToString.find(value);
    if (it == _valueToString.end())
    {
       throw Error("Invalid value for enum 'UniformBinding'");
    }
    return it->second;
}

template <>
UniformBinding Enum::fromString<UniformBinding>(const std::string& string)
{
    static std::map<std::string, UniformBinding> _stringToValue;
    if (_stringToValue.empty())
    {
        _stringToValue["None"] = UniformBinding_None;
        _stringToValue["RenderTargetSize"] = UniformBinding_RenderTargetSize;
        _stringToValue["CameraPosition"] = UniformBinding_CameraPosition;
        _stringToValue["CameraFront"] = UniformBinding_CameraFront;
        _stringToValue["CameraUp"] = UniformBinding_CameraUp;
        _stringToValue["ViewMatrix"] = UniformBinding_ViewMatrix;
        _stringToValue["ProjectionMatrix"] = UniformBinding_ProjectionMatrix;
        _stringToValue["ViewProjectionMatrix"] = UniformBinding_ViewProjectionMatrix;
        _stringToValue["ModelMatrix"] = UniformBinding_ModelMatrix;
        _stringToValue["ModelViewMatrix"] = UniformBinding_ModelViewMatrix;
        _stringToValue["ModelViewProjectionMatrix"] = UniformBinding_ModelViewProjectionMatrix;
    }
    auto it = _stringToValue.find(string);
    if (it == _stringToValue.end())
    {
       throw Error(format("Invalid value '%s' for enum 'UniformBinding'", string.c_str()));
    }
    return it->second;
}

