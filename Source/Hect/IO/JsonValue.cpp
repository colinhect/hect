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
#include "JsonValue.h"

#include "Hect/IO/ReadStream.h"
#include "Hect/IO/WriteStream.h"

#include <json/json.h>

using namespace hect;

const JsonValue JsonValue::_null;
const JsonValue::Array JsonValue::_emptyArray;

JsonValue::JsonValue() :
    _type(JsonValueType::Null)
{
}

JsonValue::JsonValue(JsonValueType::Enum type) :
    _type(type)
{
    switch (type)
    {
    case JsonValueType::Bool:
        _any = false;
        break;
    case JsonValueType::Number:
        _any = 0.0;
        break;
    case JsonValueType::String:
        _any = std::string();
        break;
    case JsonValueType::Array:
        _any = JsonValue::Array();
        break;
    case JsonValueType::Object:
        _any = JsonValue::Object();
        break;
    }
}

JsonValue::JsonValue(bool value) :
    _type(JsonValueType::Bool),
    _any(value)
{
}

JsonValue::JsonValue(int value) :
    _type(JsonValueType::Number),
    _any((double)value)
{
}

JsonValue::JsonValue(unsigned value) :
    _type(JsonValueType::Number),
    _any((double)value)
{
}

JsonValue::JsonValue(double value) :
    _type(JsonValueType::Number),
    _any(value)
{
}

JsonValue::JsonValue(const Vector2& value) :
    _type(JsonValueType::Array),
    _any(JsonValue::Array())
{
    JsonValue::Array& elements = _any.as<JsonValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
}

JsonValue::JsonValue(const Vector3& value) :
    _type(JsonValueType::Array),
    _any(JsonValue::Array())
{
    JsonValue::Array& elements = _any.as<JsonValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
    elements.push_back(value.z);
}

JsonValue::JsonValue(const Vector4& value) :
    _type(JsonValueType::Array),
    _any(JsonValue::Array())
{
    JsonValue::Array& elements = _any.as<JsonValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
    elements.push_back(value.z);
    elements.push_back(value.w);
}

JsonValue::JsonValue(const Matrix4& value) :
    _type(JsonValueType::Array),
    _any(JsonValue::Array())
{
    JsonValue::Array& elements = _any.as<JsonValue::Array>();
    for (unsigned i = 0; i < 16; ++i)
    {
        elements.push_back(value[i]);
    }
}

JsonValue::JsonValue(const Quaternion& value) :
    _type(JsonValueType::Array),
    _any(JsonValue::Array())
{
    JsonValue::Array& elements = _any.as<JsonValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
    elements.push_back(value.z);
    elements.push_back(value.w);
}

JsonValue::JsonValue(const char* value) :
    _type(JsonValueType::String),
    _any(std::string(value))
{
}

JsonValue::JsonValue(const std::string& value) :
    _type(JsonValueType::String),
    _any(value)
{
}

JsonValue::JsonValue(JsonValue&& jsonValue) :
    _type(jsonValue._type),
    _any(std::move(jsonValue._any))
{
    jsonValue._type = JsonValueType::Null;
}

JsonValueType::Enum JsonValue::type() const
{
    return _type;
}

const JsonValue& JsonValue::or(const JsonValue& jsonValue) const
{
    if (isNull())
    {
        return jsonValue;
    }
    else
    {
        return *this;
    }
}

bool JsonValue::isNull() const
{
    return _type == JsonValueType::Null;
}

bool JsonValue::isBool() const
{
    return _type == JsonValueType::Bool;
}

bool JsonValue::isNumber() const
{
    return _type == JsonValueType::Number;
}

bool JsonValue::isString() const
{
    return _type == JsonValueType::String;
}

bool JsonValue::isArray() const
{
    return _type == JsonValueType::Array;
}

bool JsonValue::isObject() const
{
    return _type == JsonValueType::Object;
}

bool JsonValue::asBool() const
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

int JsonValue::asInt() const
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

unsigned JsonValue::asUnsigned() const
{
    if (isNumber())
    {
        return (unsigned)_any.as<double>();
    }
    else
    {
        return 0;
    }
}

double JsonValue::asDouble() const
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

Real JsonValue::asReal() const
{
    return (Real)asDouble();
}

Vector2 JsonValue::asVector2() const
{
    Vector2 result;

    size_t i = 0;
    for (const JsonValue& component : *this)
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

Vector3 JsonValue::asVector3() const
{
    Vector3 result;

    size_t i = 0;
    for (const JsonValue& component : *this)
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

Vector4 JsonValue::asVector4() const
{
    Vector4 result;

    size_t i = 0;
    for (const JsonValue& component : *this)
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

Matrix4 JsonValue::asMatrix4() const
{
    Matrix4 result;

    size_t i = 0;
    for (const JsonValue& component : *this)
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

Quaternion JsonValue::asQuaternion() const
{
    Quaternion result;

    size_t i = 0;
    for (const JsonValue& component : *this)
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

const std::string& JsonValue::asString() const
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

size_t JsonValue::size() const
{
    if (isArray())
    {
        return _any.as<JsonValue::Array>().size();
    }
    else if (isObject())
    {
        return _any.as<JsonValue::Object>().size();
    }

    return 0;
}

std::vector<std::string> JsonValue::memberNames() const
{
    if (isObject())
    {
        std::vector<std::string> result;
        for (auto& pair : _any.as<JsonValue::Object>())
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

void JsonValue::addMember(const std::string& name, const JsonValue& jsonValue)
{
    if (isObject())
    {
        _any.as<JsonValue::Object>()[name] = jsonValue;
    }
    else
    {
        throw Error("The JSON value is not an object");
    }
}

void JsonValue::addElement(const JsonValue& jsonValue)
{
    if (isArray())
    {
        _any.as<JsonValue::Array>().push_back(jsonValue);
    }
    else
    {
        throw Error("The JSON value is not an array");
    }
}

const JsonValue& JsonValue::operator[](size_t index) const
{
    if (isArray())
    {
        const Array& array = _any.as<JsonValue::Array>();
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

const JsonValue& JsonValue::operator[](const std::string& name) const
{
    if (isObject())
    {
        const JsonValue::Object& members = _any.as<JsonValue::Object>();
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

JsonValue::Array::const_iterator JsonValue::begin() const
{
    if (isArray())
    {
        return _any.as<JsonValue::Array>().begin();
    }
    else
    {
        return _emptyArray.begin();
    }
}

JsonValue::Array::const_iterator JsonValue::end() const
{
    if (isArray())
    {
        return _any.as<JsonValue::Array>().end();
    }
    else
    {
        return _emptyArray.end();
    }
}

JsonValue toJsonValue(Json::Value& jsonValue);
Json::Value fromJsonValue(const JsonValue& jsonValue);

std::string JsonValue::encodeToJson() const
{
    Json::Value jsonValue = fromJsonValue(*this);
    return jsonValue.toStyledString();
}

void JsonValue::encodeToJson(WriteStream& stream) const
{
    stream.writeString(encodeToJson(), false);
}

void JsonValue::decodeFromJson(const std::string& json)
{
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(json, root))
    {
        throw Error(reader.getFormattedErrorMessages());
    }
    *this = toJsonValue(root);
}

void JsonValue::decodeFromJson(ReadStream& stream)
{
    decodeFromJson(stream.readAllToString());
}

JsonValue toJsonValue(Json::Value& jsonValue)
{
    if (jsonValue.isBool())
    {
        return JsonValue(jsonValue.asBool());
    }
    else if (jsonValue.isNumeric())
    {
        return JsonValue(jsonValue.asDouble());
    }
    else if (jsonValue.isString())
    {
        return JsonValue(jsonValue.asString());
    }
    else if (jsonValue.isArray())
    {
        JsonValue value(JsonValueType::Array);
        for (Json::Value& element : jsonValue)
        {
            value.addElement(toJsonValue(element));
        }
        return value;
    }
    else if (jsonValue.isObject())
    {
        JsonValue value(JsonValueType::Object);
        for (std::string& name : jsonValue.getMemberNames())
        {
            value.addMember(name, toJsonValue(jsonValue[name]));
        }
        return value;
    }

    return JsonValue();
}

Json::Value fromJsonValue(const JsonValue& jsonValue)
{
    if (jsonValue.isBool())
    {
        return Json::Value(jsonValue.asBool());
    }
    else if (jsonValue.isNumber())
    {
        return Json::Value(jsonValue.asDouble());
    }
    else if (jsonValue.isString())
    {
        return Json::Value(jsonValue.asString());
    }
    else if (jsonValue.isArray())
    {
        Json::Value value(Json::arrayValue);
        for (const JsonValue& element : jsonValue)
        {
            value.append(fromJsonValue(element));
        }
        return value;
    }
    else if (jsonValue.isObject())
    {
        Json::Value value(Json::objectValue);
        for (const std::string& name : jsonValue.memberNames())
        {
            value[name] = fromJsonValue(jsonValue[name]);
        }
        return value;
    }

    return Json::Value();
}