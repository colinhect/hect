// Generated by reflect.py
#include <Hect/Core/Enum.h>
#include <Hect/Core/Error.h>
#include <Hect/Core/Format.h>

#include "..\IO\JsonValue.h"

using namespace hect;

template <>
const std::string& Enum::toString<JsonValueType>(JsonValueType value)
{
    static std::map<JsonValueType, std::string> _valueToString;
    if (_valueToString.empty())
    {
        _valueToString[JsonValueType_Null] = "Null";
        _valueToString[JsonValueType_Bool] = "Bool";
        _valueToString[JsonValueType_Number] = "Number";
        _valueToString[JsonValueType_String] = "String";
        _valueToString[JsonValueType_Array] = "Array";
        _valueToString[JsonValueType_Object] = "Object";
    }
    auto it = _valueToString.find(value);
    if (it == _valueToString.end())
    {
       throw Error("Invalid value for enum 'JsonValueType'");
    }
    return it->second;
}

template <>
JsonValueType Enum::fromString<JsonValueType>(const std::string& string)
{
    static std::map<std::string, JsonValueType> _stringToValue;
    if (_stringToValue.empty())
    {
        _stringToValue["Null"] = JsonValueType_Null;
        _stringToValue["Bool"] = JsonValueType_Bool;
        _stringToValue["Number"] = JsonValueType_Number;
        _stringToValue["String"] = JsonValueType_String;
        _stringToValue["Array"] = JsonValueType_Array;
        _stringToValue["Object"] = JsonValueType_Object;
    }
    auto it = _stringToValue.find(string);
    if (it == _stringToValue.end())
    {
       throw Error(format("Invalid value '%s' for enum 'JsonValueType'", string.c_str()));
    }
    return it->second;
}

