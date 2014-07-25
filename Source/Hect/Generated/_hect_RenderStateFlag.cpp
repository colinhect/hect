// Generated by reflect.py
#include <Hect/Core/Enum.h>
#include <Hect/Core/Error.h>
#include <Hect/Core/Format.h>

#include "..\Graphics\RenderState.h"

using namespace hect;

template <>
const std::string& Enum::toString<RenderStateFlag>(RenderStateFlag value)
{
    static std::map<RenderStateFlag, std::string> _valueToString;
    if (_valueToString.empty())
    {
        _valueToString[RenderStateFlag_Blend] = "Blend";
        _valueToString[RenderStateFlag_DepthTest] = "DepthTest";
        _valueToString[RenderStateFlag_DepthWrite] = "DepthWrite";
        _valueToString[RenderStateFlag_CullFace] = "CullFace";
    }
    auto it = _valueToString.find(value);
    if (it == _valueToString.end())
    {
       throw Error("Invalid value for enum 'RenderStateFlag'");
    }
    return it->second;
}

template <>
RenderStateFlag Enum::fromString<RenderStateFlag>(const std::string& string)
{
    static std::map<std::string, RenderStateFlag> _stringToValue;
    if (_stringToValue.empty())
    {
        _stringToValue["Blend"] = RenderStateFlag_Blend;
        _stringToValue["DepthTest"] = RenderStateFlag_DepthTest;
        _stringToValue["DepthWrite"] = RenderStateFlag_DepthWrite;
        _stringToValue["CullFace"] = RenderStateFlag_CullFace;
    }
    auto it = _stringToValue.find(string);
    if (it == _stringToValue.end())
    {
       throw Error(format("Invalid value '%s' for enum 'RenderStateFlag'", string.c_str()));
    }
    return it->second;
}

