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

namespace hect
{

///
/// A data value type.
enum class DataValueType
{

    ///
    /// The data value holds no data.
    Null,

    ///
    /// A bool value.
    Bool,

    ///
    /// A floating point number.
    Number,

    ///
    /// A string.
    String,

    ///
    /// An collection of other data values.
    Array,

    ///
    /// A collection of key/value pairs of data values.
    Object
};

///
/// A hierarchical structure of data.
///
/// \remarks Data values are normally constructed from JSON.  The majority of
/// asset types are defined in JSON and parsed through data values.
class DataValue
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
    DataValue(const Vector2<>& value);

    ///
    /// Constructs a data value from a 3-dimensional vector as an array of
    /// numbers.
    ///
    /// \param value The value.
    DataValue(const Vector3<>& value);

    ///
    /// Constructs a data value from a 4-dimensional vector as an array of
    /// numbers.
    ///
    /// \param value The value.
    DataValue(const Vector4<>& value);

    ///
    /// Constructs a data value from a 4 by 4 matrix as an array of numbers.
    ///
    /// \param value The value.
    DataValue(const Matrix4<>& value);

    ///
    /// Constructs a data value from a quaternion as an array of numbers.
    ///
    /// \param value The value.
    DataValue(const Quaternion<>& value);

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
    /// Constructs a data value moved from another.
    ///
    /// \param dataValue The data value to move.
    DataValue(DataValue&& dataValue);

    ///
    /// Returns the type.
    DataValueType type() const;

    ///
    /// Returns this data value or another one if this data value is null.
    ///
    /// \param dataValue The data value to return if this one is null.
    const DataValue& or(const DataValue& dataValue) const;

    ///
    /// Returns whether the value is null.
    bool isNull() const;

    ///
    /// Returns whether the value is a bool.
    bool isBool() const;

    ///
    /// Returns whether the value is a number.
    bool isNumber() const;

    ///
    /// Returns whether the value is a string.
    bool isString() const;

    ///
    /// Returns whether the value is an array.
    bool isArray() const;

    ///
    /// Returns whether the value is an object.
    bool isObject() const;

    ///
    /// Returns the value as a bool (false if the data value is not a bool).
    bool asBool() const;

    ///
    /// Returns the value as an int (zero if the data value is not a number).
    int asInt() const;

    ///
    /// Returns the value as an unsigned int (zero if the data value is not a
    /// number).
    unsigned asUnsigned() const;

    ///
    /// Returns the value as a double (zero if the data value is not a number).
    double asDouble() const;

    ///
    /// Returns the value as a 2-dimensional vector (assumes the value is an
    /// array of numbers).
    Vector2<> asVector2() const;

    ///
    /// Returns the value as a 3-dimensional vector (assumes the value is an
    /// array of numbers).
    Vector3<> asVector3() const;

    ///
    /// Returns the value as a 4-dimensional vector (assumes the value is an
    /// array of numbers).
    Vector4<> asVector4() const;

    ///
    /// Returns the value as a 4 by 4 matrix (assumes the value is an
    /// array of numbers).
    Matrix4<> asMatrix4() const;

    ///
    /// Returns the value as a quaternion (assumes the value is an array of
    /// numbers).
    Quaternion<> asQuaternion() const;

    ///
    /// Returns the value as a string (empty string if the data value is not a
    /// string).
    const std::string& asString() const;

    ///
    /// Returns the number of elements or members.
    size_t size() const;

    ///
    /// Returns the member names (empty if the data value is not an object).
    std::vector<std::string> memberNames() const;

    ///
    /// Adds a new member to the data value.
    ///
    /// \remarks If a member with the given name already exists then its value
    /// is overwritten with the new value.
    ///
    /// \param name The member name.
    /// \param value The member value.
    ///
    /// \throws Error If the data value is not an object.
    void addMember(const std::string& name, const DataValue& value);

    ///
    /// Adds a new element to the data value.
    ///
    /// \param value The element value.
    ///
    /// \throws Error If the data value is not an array.
    void addElement(const DataValue& value);

    ///
    /// Returns the element at the given index.
    ///
    /// \remarks Only applies to data values that are arrays.
    ///
    /// \param index The index to access the element at.
    const DataValue& operator[](size_t index) const;

    ///
    /// Returns the member of the given name.
    ///
    /// \remarks Only applies to data values that are objects.
    ///
    /// \param name The name of the member to access.
    const DataValue& operator[](const std::string& name) const;

    ///
    /// Returns an iterator at the beginning of the elements.
    ///
    /// \remarks Only applies to data values that are arrays.
    Array::const_iterator begin() const;

    ///
    /// Returns an iterator at the end of the elements.
    ///
    /// \remarks Only applies to data values that are arrays.
    Array::const_iterator end() const;

private:
    DataValueType _type;

    std::string _stringValue;
    double _numberValue;
    Array _elements;
    Object _members;

    static const DataValue _null;
    static const Array _emptyArray;
    static const std::string _emptyString;
};

}