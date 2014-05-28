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
#include <typeinfo>
#include <typeindex>

#include "Hect/Reflection/Type.h"

namespace hect
{

///
/// Provides reflection functionality for enums.
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

    ///
    /// Adds a string representation for an enum value.
    ///
    /// \param string The string representation.
    /// \param value The enum value.
    template <typename T>
    static void add(const std::string& string, T value);

private:
    static HECT_API std::map<std::type_index, std::map<std::string, int>> _stringToValue;
    static HECT_API std::map<std::type_index, std::map<int, std::string>> _valueToString;
};

}

#include "Enum.inl"