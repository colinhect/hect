// Generated by reflect.py
#include <Hect/Core/Enum.h>
#include <Hect/Core/Error.h>
#include <Hect/Core/Format.h>

#include "..\Input\Mouse.h"

using namespace hect;

template <>
const std::string& Enum::toString<MouseMode>(MouseMode value)
{
    static std::map<MouseMode, std::string> _valueToString;
    if (_valueToString.empty())
    {
        _valueToString[MouseMode_Cursor] = "Cursor";
        _valueToString[MouseMode_Relative] = "Relative";
    }
    auto it = _valueToString.find(value);
    if (it == _valueToString.end())
    {
       throw Error("Invalid value for enum 'MouseMode'");
    }
    return it->second;
}

template <>
MouseMode Enum::fromString<MouseMode>(const std::string& string)
{
    static std::map<std::string, MouseMode> _stringToValue;
    if (_stringToValue.empty())
    {
        _stringToValue["Cursor"] = MouseMode_Cursor;
        _stringToValue["Relative"] = MouseMode_Relative;
    }
    auto it = _stringToValue.find(string);
    if (it == _stringToValue.end())
    {
       throw Error(format("Invalid value '%s' for enum 'MouseMode'", string.c_str()));
    }
    return it->second;
}

