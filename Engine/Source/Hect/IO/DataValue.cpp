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
#include "DataValue.h"

#include "Hect/IO/ReadStream.h"
#include "Hect/IO/WriteStream.h"

using namespace hect;

const DataValue DataValue::_null;
const DataValue::Array DataValue::_emptyArray;

DataValue::DataValue() :
    _type(DataValueType_Null)
{
}

DataValue::DataValue(DataValueType type) :
    _type(type)
{
    switch (type)
    {
    case DataValueType_Bool:
        _any = false;
        break;
    case DataValueType_Number:
        _any = 0.0;
        break;
    case DataValueType_String:
        _any = std::string();
        break;
    case DataValueType_Array:
        _any = DataValue::Array();
        break;
    case DataValueType_Object:
        _any = DataValue::Object();
        break;
    default:
        break;
    }
}

DataValue::DataValue(bool value) :
    _type(DataValueType_Bool),
    _any(value)
{
}

DataValue::DataValue(int value) :
    _type(DataValueType_Number),
    _any((double)value)
{
}

DataValue::DataValue(unsigned value) :
    _type(DataValueType_Number),
    _any((double)value)
{
}

DataValue::DataValue(double value) :
    _type(DataValueType_Number),
    _any(value)
{
}

DataValue::DataValue(const Vector2& value) :
    _type(DataValueType_Array),
    _any(DataValue::Array())
{
    DataValue::Array& elements = _any.as<DataValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
}

DataValue::DataValue(const Vector3& value) :
    _type(DataValueType_Array),
    _any(DataValue::Array())
{
    DataValue::Array& elements = _any.as<DataValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
    elements.push_back(value.z);
}

DataValue::DataValue(const Vector4& value) :
    _type(DataValueType_Array),
    _any(DataValue::Array())
{
    DataValue::Array& elements = _any.as<DataValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
    elements.push_back(value.z);
    elements.push_back(value.w);
}

DataValue::DataValue(const Matrix4& value) :
    _type(DataValueType_Array),
    _any(DataValue::Array())
{
    DataValue::Array& elements = _any.as<DataValue::Array>();
    for (unsigned i = 0; i < 16; ++i)
    {
        elements.push_back(value[i]);
    }
}

DataValue::DataValue(const Quaternion& value) :
    _type(DataValueType_Array),
    _any(DataValue::Array())
{
    DataValue::Array& elements = _any.as<DataValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
    elements.push_back(value.z);
    elements.push_back(value.w);
}

DataValue::DataValue(const char* value) :
    _type(DataValueType_String),
    _any(std::string(value))
{
}

DataValue::DataValue(const std::string& value) :
    _type(DataValueType_String),
    _any(value)
{
}

DataValue::DataValue(const DataValue& dataValue) :
    _type(dataValue._type),
    _any(dataValue._any)
{
}

DataValue::DataValue(DataValue&& dataValue) :
    _type(dataValue._type),
    _any(std::move(dataValue._any))
{
    dataValue._type = DataValueType_Null;
}

DataValueType DataValue::type() const
{
    return _type;
}

const DataValue& DataValue::orDefault(const DataValue& dataValue) const
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
    return _type == DataValueType_Null;
}

bool DataValue::isBool() const
{
    return _type == DataValueType_Bool;
}

bool DataValue::isNumber() const
{
    return _type == DataValueType_Number;
}

bool DataValue::isString() const
{
    return _type == DataValueType_String;
}

bool DataValue::isArray() const
{
    return _type == DataValueType_Array;
}

bool DataValue::isObject() const
{
    return _type == DataValueType_Object;
}

bool DataValue::asBool() const
{
    if (isBool())
    {
        return _any.as<bool>();
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
        return (int)_any.as<double>();
    }
    else
    {
        return 0;
    }
}

Real DataValue::asReal() const
{
    if (isNumber())
    {
        return _any.as<double>();
    }
    else
    {
        return 0.0;
    }
}

Vector2 DataValue::asVector2() const
{
    Vector2 result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 2)
        {
            result[i++] = component.asReal();
        }
        else
        {
            break;
        }
    }

    return result;
}

Vector3 DataValue::asVector3() const
{
    Vector3 result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 3)
        {
            result[i++] = component.asReal();
        }
        else
        {
            break;
        }
    }

    return result;
}

Vector4 DataValue::asVector4() const
{
    Vector4 result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 4)
        {
            result[i++] = component.asReal();
        }
        else
        {
            break;
        }
    }

    return result;
}

Matrix4 DataValue::asMatrix4() const
{
    Matrix4 result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 16)
        {
            result[i++] = component.asReal();
        }
        else
        {
            break;
        }
    }

    return result;
}

Quaternion DataValue::asQuaternion() const
{
    Quaternion result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 4)
        {
            result[i++] = component.asReal();
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
    static std::string empty;

    if (isString())
    {
        return _any.as<std::string>();
    }
    else
    {
        return empty;
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

DataValue& DataValue::operator=(const DataValue& dataValue)
{
    _type = dataValue._type;
    _any = dataValue._any;
    return *this;
}

DataValue& DataValue::operator=(DataValue&& dataValue)
{
    _type = dataValue._type;
    _any = std::move(dataValue._any);
    return *this;
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

#include <json/json.h>

DataValue fromJson(Json::Value& jsonValue);
Json::Value toJson(const DataValue& dataValue);

std::string DataValue::encodeToJson() const
{
    Json::Value jsonValue = toJson(*this);
    return jsonValue.toStyledString();
}

void DataValue::encodeToJson(WriteStream& stream) const
{
    std::string json = encodeToJson();
    stream.write(reinterpret_cast<const uint8_t*>(json.c_str()), json.size());
}

void DataValue::decodeFromJson(const std::string& json)
{
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(json, root))
    {
        throw Error(reader.getFormattedErrorMessages());
    }
    *this = fromJson(root);
}

void DataValue::decodeFromJson(ReadStream& stream)
{
    decodeFromJson(stream.readAllToString());
}

#include <yaml-cpp/yaml.h>

DataValue fromYaml(YAML::Node& node);
YAML::Node toYaml(const DataValue& dataValue);

std::string DataValue::encodeToYaml() const
{
    YAML::Node node = toYaml(*this);
    return YAML::Dump(node);
}

void DataValue::encodeToYaml(WriteStream& stream) const
{
    std::string yaml = encodeToYaml();
    stream.write(reinterpret_cast<const uint8_t*>(yaml.c_str()), yaml.size());
}

void DataValue::decodeFromYaml(const std::string& yaml)
{
    try
    {
        YAML::Node node = YAML::Load(yaml);
        *this = fromYaml(node);
    }
    catch (YAML::ParserException& ex)
    {
        throw Error(ex.what());
    }
}

void DataValue::decodeFromYaml(ReadStream& stream)
{
    decodeFromYaml(stream.readAllToString());
}

DataValue fromJson(Json::Value& jsonValue)
{
    if (jsonValue.isBool())
    {
        return DataValue(jsonValue.asBool());
    }
    else if (jsonValue.isNumeric())
    {
        return DataValue(jsonValue.asDouble());
    }
    else if (jsonValue.isString())
    {
        return DataValue(jsonValue.asString());
    }
    else if (jsonValue.isArray())
    {
        DataValue value(DataValueType_Array);
        for (Json::Value& element : jsonValue)
        {
            value.addElement(fromJson(element));
        }
        return value;
    }
    else if (jsonValue.isObject())
    {
        DataValue value(DataValueType_Object);
        for (std::string& name : jsonValue.getMemberNames())
        {
            value.addMember(name, fromJson(jsonValue[name]));
        }
        return value;
    }

    return DataValue();
}

Json::Value toJson(const DataValue& dataValue)
{
    if (dataValue.isBool())
    {
        return Json::Value(dataValue.asBool());
    }
    else if (dataValue.isNumber())
    {
        return Json::Value(dataValue.asReal());
    }
    else if (dataValue.isString())
    {
        return Json::Value(dataValue.asString());
    }
    else if (dataValue.isArray())
    {
        Json::Value value(Json::arrayValue);
        for (const DataValue& element : dataValue)
        {
            value.append(toJson(element));
        }
        return value;
    }
    else if (dataValue.isObject())
    {
        Json::Value value(Json::objectValue);
        for (const std::string& name : dataValue.memberNames())
        {
            value[name] = toJson(dataValue[name]);
        }
        return value;
    }

    return Json::Value();
}

bool isNumeric(const std::string& string)
{
    char* end = nullptr;
    strtod(string.c_str(), &end);
    return !*end;
}

DataValue fromYaml(YAML::Node& node)
{
    if (node.IsScalar() && (node.Scalar() == "true" || node.Scalar() == "false"))
    {
        return DataValue(node.Scalar() == "true");
    }
    else if (node.IsScalar() && isNumeric(node.Scalar()))
    {
        return DataValue(node.as<double>());
    }
    else if (node.IsScalar())
    {
        return DataValue(node.Scalar());
    }
    else if (node.IsSequence())
    {
        DataValue value(DataValueType_Array);
        for (size_t i = 0; i < node.size(); ++i)
        {
            YAML::Node itemNode = node[i];
            value.addElement(fromYaml(itemNode));
        }
        return value;
    }
    else if (node.IsMap())
    {
        DataValue value(DataValueType_Object);
        for (auto member : node)
        {
            value.addMember(member.first.Scalar(), fromYaml(member.second));
        }
        return value;
    }

    return DataValue();
}

YAML::Node toYaml(const DataValue& dataValue)
{
    if (dataValue.isBool())
    {
        return YAML::Node(dataValue.asBool());
    }
    else if (dataValue.isNumber())
    {
        return YAML::Node(dataValue.asReal());
    }
    else if (dataValue.isString())
    {
        return YAML::Node(dataValue.asString());
    }
    else if (dataValue.isArray())
    {
        YAML::Node node;
        for (const DataValue& element : dataValue)
        {
            node.push_back(toYaml(element));
        }
        return node;
    }
    else if (dataValue.isObject())
    {
        YAML::Node node;
        for (const std::string& name : dataValue.memberNames())
        {
            node[name] = toYaml(dataValue[name]);
        }
        return node;
    }

    return YAML::Node();
}