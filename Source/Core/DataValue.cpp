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

DataValue::DataValue() :
    _type(DataValueType::Null)
{
    _value.asInt = 0;
}

DataValue::DataValue(DataValueType type) :
    _type(type)
{
    _value.asInt = 0;

    if (type == DataValueType::Array)
    {
        _any = DataValue::Array();
    }
    else if (type == DataValueType::Object)
    {
        _any = DataValue::Object();
    }
}

DataValue::DataValue(bool value) :
    _type(DataValueType::Bool)
{
    _value.asDouble = value ? 1 : 0;
}

DataValue::DataValue(int value) :
    _type(DataValueType::Number)
{
    _value.asDouble = (double)value;
}

DataValue::DataValue(unsigned value) :
    _type(DataValueType::Number)
{
    _value.asDouble = (double)value;
}

DataValue::DataValue(double value) :
    _type(DataValueType::Number)
{
    _value.asDouble = value;
}

DataValue::DataValue(const Vector2<>& value) :
    _type(DataValueType::Array),
    _any(DataValue::Array())
{
    _value.asInt = 0;

    DataValue::Array& elements = _any.as<DataValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
}

DataValue::DataValue(const Vector3<>& value) :
    _type(DataValueType::Array),
    _any(DataValue::Array())
{
    _value.asInt = 0;
    
    DataValue::Array& elements = _any.as<DataValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
    elements.push_back(value.z);
}

DataValue::DataValue(const Vector4<>& value) :
    _type(DataValueType::Array),
    _any(DataValue::Array())
{
    _value.asInt = 0;
    
    DataValue::Array& elements = _any.as<DataValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
    elements.push_back(value.z);
    elements.push_back(value.w);
}

DataValue::DataValue(const Matrix4<>& value) :
    _type(DataValueType::Array),
    _any(DataValue::Array())
{
    _value.asInt = 0;

    DataValue::Array& elements = _any.as<DataValue::Array>();
    for (unsigned i = 0; i < 16; ++i)
    {
        elements.push_back(value[i]);
    }
}

DataValue::DataValue(const Quaternion<>& value) :
    _type(DataValueType::Array),
    _any(DataValue::Array())
{
    _value.asInt = 0;

    DataValue::Array& elements = _any.as<DataValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
    elements.push_back(value.z);
    elements.push_back(value.w);
}

DataValue::DataValue(const char* value) :
    _type(DataValueType::String)
{
    _value.asString = new char[strlen(value) + 1];
    strcpy(_value.asString, value);
}

DataValue::DataValue(const std::string& value) :
    _type(DataValueType::String)
{
    _value.asString = new char[value.length() + 1];
    strcpy(_value.asString, value.c_str());
}

DataValue::DataValue(const DataValue& dataValue)
{
    *this = dataValue;
}

DataValue::DataValue(DataValue&& dataValue) :
    _type(dataValue._type),
    _value(dataValue._value),
    _any(std::move(dataValue._any))
{
    dataValue._type = DataValueType::Null;
    dataValue._value.asInt = 0;
}

DataValue::~DataValue()
{
    if (_type == DataValueType::String && _value.asString)
    {
        delete [] _value.asString;
    }
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
        return _value.asDouble == 1;
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
        return (int)_value.asDouble;
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
        return (unsigned)_value.asDouble;
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
        return _value.asDouble;
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

const char* DataValue::asString() const
{
    if (isString())
    {
        return _value.asString;
    }
    else
    {
        return "";
    }
}

size_t DataValue::size() const
{
    if (isArray())
    {
        return _any.as<DataValue::Array>().size();
    }
    else if (isObject())
    {
        return _any.as<DataValue::Object>().size();
    }

    return 0;
}

std::vector<std::string> DataValue::memberNames() const
{
    if (isObject())
    {
        std::vector<std::string> result;
        for (auto& pair : _any.as<DataValue::Object>())
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

void DataValue::addMember(const std::string& name, const DataValue& dataValue)
{
    if (isObject())
    {
        _any.as<DataValue::Object>()[name] = dataValue;
    }
    else
    {
        throw Error("The data value is not an object");
    }
}

void DataValue::addElement(const DataValue& dataValue)
{
    if (isArray())
    {
        _any.as<DataValue::Array>().push_back(dataValue);
    }
    else
    {
        throw Error("The data value is not an array");
    }
}

DataValue& DataValue::operator=(const DataValue& dataValue)
{
    _type = dataValue._type;
    _value = dataValue._value;
    _any = dataValue._any;

    if (_type == DataValueType::String)
    {
        _value.asString = new char[strlen(dataValue._value.asString) + 1];
        strcpy(_value.asString, dataValue._value.asString);
    }

    return *this;
}

const DataValue& DataValue::operator[](size_t index) const
{
    if (isArray())
    {
        const Array& array = _any.as<DataValue::Array>();
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
        const DataValue::Object& members = _any.as<DataValue::Object>();
        auto it = members.find(name);
        if (it == members.end())
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
        return _any.as<DataValue::Array>().begin();
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
        return _any.as<DataValue::Array>().end();
    }
    else
    {
        return _emptyArray.end();
    }
}