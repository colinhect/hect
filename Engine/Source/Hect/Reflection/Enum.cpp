///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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
#include "Enum.h"

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"
#include "Hect/Reflection/Type.h"

using namespace hect;

EnumValue::EnumValue(EnumValue::Type numeric_value, const std::string& string_value) :
    numeric_value(numeric_value),
    string_value(string_value)
{
}

EnumValue::Type Enum::from_string(const std::string& string_value) const
{
    auto it = _string_to_numeric.find(string_value);
    if (it == _string_to_numeric.end())
    {
        throw DecodeError(format("Invalid string value '%s' for enum '%s'", string_value.data(), _name.data()));
    }
    return it->second;
}

const std::string& Enum::to_string(EnumValue::Type numeric_value) const
{
    auto it = _numeric_to_string.find(numeric_value);
    if (it == _numeric_to_string.end())
    {
        throw EncodeError(format("Invalid numeric value '%i' for enum '%s'", numeric_value, _name.data()));
    }
    return it->second;
}

void Enum::add_value(EnumValue::Type numeric_value, const std::string& string_value)
{
    _values.push_back(EnumValue(numeric_value, string_value));
    _string_to_numeric[string_value] = numeric_value;
    _numeric_to_string[numeric_value] = string_value;
}

Enum::ValueSequence Enum::values() const
{
    return ValueSequence(_values);
}

Enum::Enum()
{
}

Enum::Enum(Name name) :
    _name(name)
{
}
