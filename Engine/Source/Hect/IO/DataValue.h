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
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "Hect/Core/Any.h"
#include "Hect/Core/Export.h"
#include "Hect/Core/Name.h"
#include "Hect/IO/DataValueType.h"
#include "Hect/Math/Vector2.h"
#include "Hect/Math/Vector3.h"
#include "Hect/Math/Vector4.h"
#include "Hect/Math/Matrix4.h"
#include "Hect/Math/Quaternion.h"

namespace hect
{

class ReadStream;
class WriteStream;

///
/// A node in a hierarchical structure of data.
class HECT_EXPORT DataValue
{
public:

    ///
    /// The underlying type used for an array.
    typedef std::vector<DataValue> Array;

    ///
    /// The underlying type used for an object.
    typedef std::map<std::string, DataValue> Object;

    ///
    /// Constructs a null data value.
    DataValue();

    ///
    /// Constructs a data value of a certain type.
    DataValue(DataValueType type);

    ///
    /// Constructs a bool data value.
    ///
    /// \param value The bool value.
    DataValue(bool value);

    ///
    /// Constructs a number data value.
    ///
    /// \param value The number value.
    DataValue(int value);

    ///
    /// Constructs a number data value.
    ///
    /// \param value The number value.
    DataValue(unsigned value);

    ///
    /// Constructs a number data value.
    ///
    /// \param value The number value.
    DataValue(double value);

    ///
    /// Constructs a data value from a 2-dimensional vector as an array of
    /// numbers.
    ///
    /// \param value The value.
    DataValue(Vector2 value);

    ///
    /// Constructs a data value from a 3-dimensional vector as an array of
    /// numbers.
    ///
    /// \param value The value.
    DataValue(Vector3 value);

    ///
    /// Constructs a data value from a 4-dimensional vector as an array of
    /// numbers.
    ///
    /// \param value The value.
    DataValue(Vector4 value);

    ///
    /// Constructs a data value from a 4 by 4 matrix as an array of numbers.
    ///
    /// \param value The value.
    DataValue(const Matrix4& value);

    ///
    /// Constructs a data value from a quaternion as an array of numbers.
    ///
    /// \param value The value.
    DataValue(Quaternion value);

    ///
    /// Constructs a string data value.
    ///
    /// \param value The string value.
    DataValue(const char* value);

    ///
    /// Constructs a string data value.
    ///
    /// \param value The string value.
    DataValue(const std::string& value);

    ///
    /// Constructs a string data value.
    ///
    /// \param value The string value.
    DataValue(Name value);

    ///
    /// Constructs a data value copied from another.
    ///
    /// \param data_value The data value to copy.
    DataValue(const DataValue& data_value);

    ///
    /// Constructs a data value moved from another.
    ///
    /// \param data_value The data value to move.
    DataValue(DataValue&& data_value);

    ///
    /// Returns the type.
    DataValueType type() const;

    ///
    /// Returns this data value or another one if this data value is null.
    ///
    /// \param data_value The data value to return if this one is null.
    const DataValue& or_default(const DataValue& data_value) const;

    ///
    /// Returns whether the value is null.
    bool is_null() const;

    ///
    /// Returns whether the value is a bool.
    bool is_bool() const;

    ///
    /// Returns whether the value is a number.
    bool is_number() const;

    ///
    /// Returns whether the value is a string.
    bool is_string() const;

    ///
    /// Returns whether the value is an array.
    bool is_array() const;

    ///
    /// Returns whether the value is an object.
    bool is_object() const;

    ///
    /// Returns the value as a bool (false if the data value is not a bool).
    bool as_bool() const;

    ///
    /// Returns the value as an int (zero if the data value is not a number).
    int as_int() const;

    ///
    /// Returns the value as an unsigned int (zero if the data value is not a number).
    unsigned as_unsigned() const;

    ///
    /// Returns the value as a double (zero if the data value is not a number).
    double as_double() const;

    ///
    /// Returns the value as a 2-dimensional vector (assumes the value is an
    /// array of numbers).
    Vector2 as_vector2() const;

    ///
    /// Returns the value as a 3-dimensional vector (assumes the value is an
    /// array of numbers).
    Vector3 as_vector3() const;

    ///
    /// Returns the value as a 4-dimensional vector (assumes the value is an
    /// array of numbers).
    Vector4 as_vector4() const;

    ///
    /// Returns the value as a 4 by 4 matrix (assumes the value is an
    /// array of numbers).
    Matrix4 as_matrix4() const;

    ///
    /// Returns the value as a quaternion (assumes the value is an array of
    /// numbers).
    Quaternion as_quaternion() const;

    ///
    /// Returns the value as a string (empty string if the data value is not a
    /// string).
    const std::string& as_string() const;

    ///
    /// Returns the value as a name (empty string if the data value is not a
    /// string).
    Name as_name() const;

    ///
    /// Returns the number of elements or members.
    size_t size() const;

    ///
    /// Returns the member names (empty if the data value is not an object).
    std::vector<std::string> member_names() const;

    ///
    /// Adds a new member to the data value.
    ///
    /// \note If a member with the given name already exists then its value
    /// is overwritten with the new value.
    ///
    /// \param name The member name.
    /// \param data_value The member value.
    ///
    /// \throws InvalidOperation If the data value is not an object.
    void add_member(const std::string& name, const DataValue& data_value);

    ///
    /// Adds a new element to the data value.
    ///
    /// \param data_value The element value.
    ///
    /// \throws InvalidOperation If the data value is not an array.
    void add_element(const DataValue& data_value);

    ///
    /// Returns the element at the given index.
    ///
    /// \note Only applies to data values that are arrays.
    ///
    /// \param index The index to access the element at.
    const DataValue& operator[](size_t index) const;

    ///
    /// Returns the member of the given name.
    ///
    /// \note Only applies to data values that are objects.
    ///
    /// \param name The name of the member to access.
    const DataValue& operator[](const std::string& name) const;

    ///
    /// Sets the data value as a copy of another.
    ///
    /// \param data_value The handle to copy.
    DataValue& operator=(const DataValue& data_value);

    ///
    /// Sets the data value as being moved from another.
    ///
    /// \param data_value The handle to move.
    DataValue& operator=(DataValue&& data_value);

    ///
    /// Returns an iterator at the beginning of the elements.
    ///
    /// \note Only applies to data values that are arrays.
    Array::const_iterator begin() const;

    ///
    /// Returns an iterator at the end of the elements.
    ///
    /// \note Only applies to data values that are arrays.
    Array::const_iterator end() const;

    ///
    /// Decodes the data value from YAML.
    ///
    /// \param yaml The YAML string.
    ///
    /// \throw DecodeError If invalid YAML is encountered.
    void decode_from_yaml(const std::string& yaml);

    ///
    /// Decodes the data value from YAML.
    ///
    /// \param stream The stream to read the YAML from.
    ///
    /// \throw DecodeError If invalid YAML is encountered.
    void decode_from_yaml(ReadStream& stream);

private:
    DataValueType _type;
    Any _any;

    static const DataValue _null;
    static const Array _empty_array;
};

}
