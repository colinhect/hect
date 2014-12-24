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
#include "Enum.h"

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"
#include "Hect/Reflection/Type.h"

using namespace hect;

EnumValue::EnumValue(EnumValue::Type numericValue, const std::string& stringValue) :
    numericValue(numericValue),
    stringValue(stringValue)
{
}

EnumValue::Type Enum::fromString(const std::string& stringValue) const
{
    auto it = _stringToNumeric.find(stringValue);
    if (it == _stringToNumeric.end())
    {
        throw InvalidOperation(format("Invalid string value '%s' for enum '%s'", stringValue.c_str(), _name.c_str()));
    }
    return it->second;
}

const std::string& Enum::toString(EnumValue::Type numericValue) const
{
    auto it = _numericToString.find(numericValue);
    if (it == _numericToString.end())
    {
        throw InvalidOperation(format("Invalid numeric value '%i' for enum '%s'", numericValue, _name.c_str()));
    }
    return it->second;
}

void Enum::addValue(EnumValue::Type numericValue, const std::string& stringValue)
{
    _values.push_back(EnumValue(numericValue, stringValue));
    _stringToNumeric[stringValue] = numericValue;
    _numericToString[numericValue] = stringValue;
}

const Enum::ValueSequence Enum::values() const
{
    return ValueSequence(_values);
}

Enum::Enum()
{
}

Enum::Enum(const std::string& name) :
    _name(name)
{
}
