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
#include "Hect.h"

using namespace hect;

const DataValue DataValue::_null;
const DataValue::Array DataValue::_emptyArray;
const std::string DataValue::_emptyString;

DataValue::DataValue() :
    _type(DataValueType::Null)
{
}

DataValue::DataValue(DataValueType type) :
    _type(type)
{
}

DataValue::DataValue(bool value) :
    _type(DataValueType::Bool),
    _numberValue(value ? 1 : 0)
{
}

DataValue::DataValue(int value) :
    _type(DataValueType::Number),
    _numberValue((double)value)
{
}

DataValue::DataValue(unsigned value) :
    _type(DataValueType::Number),
    _numberValue((double)value)
{
}

DataValue::DataValue(double value) :
    _type(DataValueType::Number),
    _numberValue(value)
{
}

DataValue::DataValue(const Vector2<>& value) :
    _type(DataValueType::Array)
{
    _elements.push_back(value.x);
    _elements.push_back(value.y);
}

DataValue::DataValue(const Vector3<>& value) :
    _type(DataValueType::Array)
{
    _elements.push_back(value.x);
    _elements.push_back(value.y);
    _elements.push_back(value.z);
}

DataValue::DataValue(const Vector4<>& value) :
    _type(DataValueType::Array)
{
    _elements.push_back(value.x);
    _elements.push_back(value.y);
    _elements.push_back(value.z);
    _elements.push_back(value.w);
}

DataValue::DataValue(const Matrix4<>& value) :
    _type(DataValueType::Array)
{
    for (unsigned i = 0; i < 16; ++i)
    {
        _elements.push_back(value[i]);
    }
}

DataValue::DataValue(const Quaternion<>& value) :
    _type(DataValueType::Array)
{
    _elements.push_back(value.x);
    _elements.push_back(value.y);
    _elements.push_back(value.z);
    _elements.push_back(value.w);
}

DataValue::DataValue(const char* value) :
    _type(DataValueType::String),
    _stringValue(value)
{
}

DataValue::DataValue(const std::string& value) :
    _type(DataValueType::String),
    _stringValue(value)
{
}

DataValue::DataValue(DataValue&& dataValue) :
    _type(dataValue._type),
    _stringValue(std::move(dataValue._stringValue)),
    _numberValue(dataValue._numberValue),
    _elements(std::move(dataValue._elements)),
    _members(std::move(dataValue._members))
{
    dataValue._type = DataValueType::Null;
}

DataValueType DataValue::type() const
{
    return _type;
}

const DataValue& DataValue::or(const DataValue& dataValue) const
{
    if (isNull())
    {
        return dataValue;
    }
    else
    {
        return *this;
    }
}

bool DataValue::isNull() const
{
    return _type == DataValueType::Null;
}

bool DataValue::isBool() const
{
    return _type == DataValueType::Bool;
}

bool DataValue::isNumber() const
{
    return _type == DataValueType::Number;
}

bool DataValue::isString() const
{
    return _type == DataValueType::String;
}

bool DataValue::isArray() const
{
    return _type == DataValueType::Array;
}

bool DataValue::isObject() const
{
    return _type == DataValueType::Object;
}

bool DataValue::asBool() const
{
    if (isBool())
    {
        return _numberValue == 1;
    }
    else
    {
        return false;
    }
}

int DataValue::asInt() const
{
    if (isNumber())
    {
        return (int)_numberValue;
    }
    else
    {
        return 0;
    }
}

unsigned DataValue::asUnsigned() const
{
    if (isNumber())
    {
        return (unsigned)_numberValue;
    }
    else
    {
        return 0;
    }
}

double DataValue::asDouble() const
{
    if (isNumber())
    {
        return _numberValue;
    }
    else
    {
        return 0.0;
    }
}

Vector2<> DataValue::asVector2() const
{
    Vector2<> result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 2)
        {
            result[i++] = component.asDouble();
        }
        else
        {
            break;
        }
    }

    return result;
}

Vector3<> DataValue::asVector3() const
{
    Vector3<> result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 3)
        {
            result[i++] = component.asDouble();
        }
        else
        {
            break;
        }
    }

    return result;
}

Vector4<> DataValue::asVector4() const
{
    Vector4<> result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 4)
        {
            result[i++] = component.asDouble();
        }
        else
        {
            break;
        }
    }

    return result;
}

Matrix4<> DataValue::asMatrix4() const
{
    Matrix4<> result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 16)
        {
            result[i++] = component.asDouble();
        }
        else
        {
            break;
        }
    }

    return result;
}

Quaternion<> DataValue::asQuaternion() const
{
    Quaternion<> result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 4)
        {
            result[i++] = component.asDouble();
        }
        else
        {
            break;
        }
    }

    return result;
}

const std::string& DataValue::asString() const
{
    if (isString())
    {
        return _stringValue;
    }
    else
    {
        return _emptyString;
    }
}

size_t DataValue::size() const
{
    if (isArray())
    {
        return _elements.size();
    }
    else if (isObject())
    {
        return _members.size();
    }

    return 0;
}

std::vector<std::string> DataValue::memberNames() const
{
    if (isObject())
    {
        std::vector<std::string> result;
        for (auto& pair : _members)
        {
            result.push_back(pair.first);
        }
        return result;
    }
    else
    {
        return std::vector<std::string>();
    }
}

void DataValue::addMember(const std::string& name, const DataValue& value)
{
    if (isObject())
    {
        _members[name] = value;
    }
    else
    {
        throw Error("The data value is not an object");
    }
}

void DataValue::addElement(const DataValue& value)
{
    if (isArray())
    {
        _elements.push_back(value);
    }
    else
    {
        throw Error("The data value is not an array");
    }
}

const DataValue& DataValue::operator[](size_t index) const
{
    if (isArray())
    {
        const Array& array = _elements;
        if (index < array.size())
        {
            return array[index];
        }
        else
        {
            return _null;
        }
    }
    else
    {
        return _null;
    }
}

const DataValue& DataValue::operator[](const std::string& name) const
{
    if (isObject())
    {
        auto it = _members.find(name);
        if (it == _members.end())
        {
            return _null;
        }
        return (*it).second;
    }
    else
    {
        return _null;
    }
}

DataValue::Array::const_iterator DataValue::begin() const
{
    if (isArray())
    {
        return _elements.begin();
    }
    else
    {
        return _emptyArray.begin();
    }
}

DataValue::Array::const_iterator DataValue::end() const
{
    if (isArray())
    {
        return _elements.end();
    }
    else
    {
        return _emptyArray.end();
    }
}