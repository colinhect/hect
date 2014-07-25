// Generated by reflect.py
#include <Hect/Core/Enum.h>
#include <Hect/Core/Error.h>
#include <Hect/Core/Format.h>

#include "..\Input\InputAxis.h"

using namespace hect;

template <>
const std::string& Enum::toString<InputAxisSource>(InputAxisSource value)
{
    static std::map<InputAxisSource, std::string> _valueToString;
    if (_valueToString.empty())
    {
        _valueToString[InputAxisSource_MouseMoveX] = "MouseMoveX";
        _valueToString[InputAxisSource_MouseMoveY] = "MouseMoveY";
        _valueToString[InputAxisSource_MouseButton] = "MouseButton";
        _valueToString[InputAxisSource_MouseScroll] = "MouseScroll";
        _valueToString[InputAxisSource_Key] = "Key";
    }
    auto it = _valueToString.find(value);
    if (it == _valueToString.end())
    {
       throw Error("Invalid value for enum 'InputAxisSource'");
    }
    return it->second;
}

template <>
InputAxisSource Enum::fromString<InputAxisSource>(const std::string& string)
{
    static std::map<std::string, InputAxisSource> _stringToValue;
    if (_stringToValue.empty())
    {
        _stringToValue["MouseMoveX"] = InputAxisSource_MouseMoveX;
        _stringToValue["MouseMoveY"] = InputAxisSource_MouseMoveY;
        _stringToValue["MouseButton"] = InputAxisSource_MouseButton;
        _stringToValue["MouseScroll"] = InputAxisSource_MouseScroll;
        _stringToValue["Key"] = InputAxisSource_Key;
    }
    auto it = _stringToValue.find(string);
    if (it == _stringToValue.end())
    {
       throw Error(format("Invalid value '%s' for enum 'InputAxisSource'", string.c_str()));
    }
    return it->second;
}

