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

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "Hect/Core/Exception.h"
#include "Hect/Core/Export.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Sequence.h"
#include "Hect/IO/DecodeError.h"
#include "Hect/IO/EncodeError.h"

namespace hect
{

///
/// A reflected enum value.
class HECT_EXPORT EnumValue
{
public:

    ///
    /// The numeric type that enum values are defined as.
    typedef uint32_t Type;

    ///
    /// Constructs an enum value given its numeric value and string value.
    ///
    /// \param numericValue The numeric value of the enum.
    /// \param stringValue The string value of the enum.
    EnumValue(EnumValue::Type numericValue, const std::string& stringValue);

    ///
    /// The numeric value of the enum.
    EnumValue::Type numericValue;

    ///
    /// The string value of the enum.
    std::string stringValue;
};

///
/// A reflected enum.
class HECT_EXPORT Enum
{
    friend class Type;
    typedef std::vector<EnumValue> ValueContainer;
public:

    ///
    /// A sequence of enum values.
    typedef Sequence<EnumValue, ValueContainer> ValueSequence;

    ///
    /// Creates an enum value for a specific enum type from its string
    /// representation.
    ///
    /// \param stringValue The string representation of the enum value.
    ///
    /// \returns The enum value.
    ///
    /// \throws DecodeError If the string does not represent a valid enum
    /// value or if the type is not an enum.
    template <typename T>
    static T fromString(const std::string& stringValue);

    ///
    /// Converts an enum value to its string representation.
    ///
    /// \param numericValue The numeric representation of the enum value.
    ///
    /// \returns The string representation of the enum value.
    ///
    /// \throws EncodeError If the number does not represent a valid enum
    /// value or if the type is not an enum.
    template <typename T>
    static const std::string& toString(T numericValue);

    ///
    /// Creates an enum value for a specific enum type from its string
    /// representation.
    ///
    /// \param stringValue The string representation of the enum value.
    ///
    /// \returns The enum value.
    ///
    /// \throws DecodeError If the string does not represent a valid enum
    /// value.
    EnumValue::Type fromString(const std::string& stringValue) const;

    ///
    /// Converts an enum value to its string representation.
    ///
    /// \param numericValue The numeric representation of the enum value.
    ///
    /// \returns The string representation of the enum value.
    ///
    /// \throws EncodeError If the number does not represent a valid enum
    /// value or if the type is not an enum.
    const std::string& toString(EnumValue::Type numericValue) const;

    void addValue(EnumValue::Type value, const std::string& string);

    ///
    /// Returns the values in the enum.
    const ValueSequence values() const;

private:
    Enum();
    Enum(const std::string& name);

    std::string _name;
    ValueContainer _values;
    std::map<std::string, EnumValue::Type> _stringToNumeric;
    std::map<EnumValue::Type, std::string> _numericToString;
};

}

#include "Enum.inl"