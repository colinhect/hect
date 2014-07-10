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
#pragma once

#include <string>
#include <map>

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"

namespace hect
{

///
/// Provides functionality for enums.
class Enum
{
public:

    ///
    /// Creates an enum value for a specific enum type from its string
    /// representation.
    ///
    /// \param string The string representation of the enum value.
    ///
    /// \returns The enum value.
    ///
    /// \throws Error If the string does not represent a valid enum value.
    template <typename T>
    static T fromString(const std::string& string);

    ///
    /// Converts an enum value to its string representation.
    ///
    /// \param value The enum value.
    ///
    /// \returns The string representation of the enum value.
    template <typename T>
    static const std::string& toString(T value);
};

#define HECT_ENUM_TO_STRING(type, value) \
    _valueToString[type##_##value] = #value;

#define HECT_ENUM_DEFINE_TO_STRING(type, values) \
    template <> \
    const std::string& Enum::toString<type>(type value) \
    { \
        static std::map<type, std::string> _valueToString; \
        if (_valueToString.empty()) \
        { \
            values \
        } \
        auto it = _valueToString.find(value); \
        if (it == _valueToString.end()) \
        { \
            throw Error("Invalid enum value"); \
        } \
        return it->second; \
    }

#define HECT_ENUM_FROM_STRING(type, value) \
    _stringToValue[#value] = type##_##value;

#define HECT_ENUM_DEFINE_FROM_STRING(type, values) \
    template <> \
    type Enum::fromString<type>(const std::string& string) \
    { \
        static std::map<std::string, type> _stringToValue; \
        if (_stringToValue.empty()) \
        { \
            values \
        } \
        auto it = _stringToValue.find(string); \
        if (it == _stringToValue.end()) \
        { \
            throw Error(format("Invalid enum string '%s'", string.c_str())); \
        } \
        return it->second; \
    }

}