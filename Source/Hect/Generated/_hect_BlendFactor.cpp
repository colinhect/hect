// Generated by reflect.py
#include <Hect/Core/Enum.h>
#include <Hect/Core/Error.h>
#include <Hect/Core/Format.h>

#include "..\Graphics\RenderState.h"

using namespace hect;

template <>
const std::string& Enum::toString<BlendFactor>(BlendFactor value)
{
    static std::map<BlendFactor, std::string> _valueToString;
    if (_valueToString.empty())
    {
        _valueToString[BlendFactor_Zero] = "Zero";
        _valueToString[BlendFactor_One] = "One";
        _valueToString[BlendFactor_SourceColor] = "SourceColor";
        _valueToString[BlendFactor_OneMinusSourceColor] = "OneMinusSourceColor";
        _valueToString[BlendFactor_DestColor] = "DestColor";
        _valueToString[BlendFactor_OneMinusDestColor] = "OneMinusDestColor";
        _valueToString[BlendFactor_SourceAlpha] = "SourceAlpha";
        _valueToString[BlendFactor_OneMinusSourceAlpha] = "OneMinusSourceAlpha";
        _valueToString[BlendFactor_DestAlpha] = "DestAlpha";
        _valueToString[BlendFactor_OneMinusDestAlpha] = "OneMinusDestAlpha";
    }
    auto it = _valueToString.find(value);
    if (it == _valueToString.end())
    {
       throw Error("Invalid value for enum 'BlendFactor'");
    }
    return it->second;
}

template <>
BlendFactor Enum::fromString<BlendFactor>(const std::string& string)
{
    static std::map<std::string, BlendFactor> _stringToValue;
    if (_stringToValue.empty())
    {
        _stringToValue["Zero"] = BlendFactor_Zero;
        _stringToValue["One"] = BlendFactor_One;
        _stringToValue["SourceColor"] = BlendFactor_SourceColor;
        _stringToValue["OneMinusSourceColor"] = BlendFactor_OneMinusSourceColor;
        _stringToValue["DestColor"] = BlendFactor_DestColor;
        _stringToValue["OneMinusDestColor"] = BlendFactor_OneMinusDestColor;
        _stringToValue["SourceAlpha"] = BlendFactor_SourceAlpha;
        _stringToValue["OneMinusSourceAlpha"] = BlendFactor_OneMinusSourceAlpha;
        _stringToValue["DestAlpha"] = BlendFactor_DestAlpha;
        _stringToValue["OneMinusDestAlpha"] = BlendFactor_OneMinusDestAlpha;
    }
    auto it = _stringToValue.find(string);
    if (it == _stringToValue.end())
    {
       throw Error(format("Invalid value '%s' for enum 'BlendFactor'", string.c_str()));
    }
    return it->second;
}

