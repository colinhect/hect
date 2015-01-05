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

#define YAML_DECLARE_STATIC
#include <yaml.h>

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

unsigned DataValue::asUnsigned() const
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
        throw InvalidOperation("The data value is not an object");
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
        throw InvalidOperation("The data value is not an array");
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

namespace hect
{

// Converts a YAML node to a DataValue
DataValue fromYaml(yaml_document_t* document, yaml_node_t* node)
{
    assert(document);
    assert(node);

    switch (node->type)
    {
    case YAML_SCALAR_NODE:
    {
        const char* value = reinterpret_cast<const char*>(node->data.scalar.value);
        std::string string(value);

        // Check if it is a bool
        if (string == "true" || string == "false")
        {
            bool boolValue = string == "true";
            return DataValue(boolValue);
        }
        else
        {
            // Attempt to parse a double from the value
            char* end = nullptr;
            double numberValue = std::strtod(value, &end);
            if (end && *end == '\0')
            {
                // It is a valid number
                return DataValue(numberValue);
            }
            else
            {
                // Otherwise it is just a string
                return DataValue(string);
            }
        }
    }
    case YAML_SEQUENCE_NODE:
    {
        DataValue dataValue(DataValueType_Array);

        // For each item in the sequence
        for (yaml_node_item_t* item = node->data.sequence.items.start;
                item < node->data.sequence.items.top; item++)
        {
            // Convert the item to a DataValue
            yaml_node_t* itemNode = yaml_document_get_node(document, *item);
            DataValue element = fromYaml(document, itemNode);

            // Add the element
            dataValue.addElement(element);
        }

        return dataValue;
    }
    case YAML_MAPPING_NODE:
    {
        DataValue dataValue(DataValueType_Object);

        // For each pair in the mapping
        for (yaml_node_pair_t* pair = node->data.mapping.pairs.start;
                pair < node->data.mapping.pairs.top; pair++)
        {
            // Get the name from the pair
            yaml_node_t* keyNode = yaml_document_get_node(document, pair->key);
            const char* name = reinterpret_cast<const char*>(keyNode->data.scalar.value);

            // Convert the value node to a DataValue
            yaml_node_t* valueNode = yaml_document_get_node(document, pair->value);
            DataValue value = fromYaml(document, valueNode);

            // Add the member
            dataValue.addMember(name, value);
        }

        return dataValue;
    }
    default:
        return DataValue();
    }
}

}

void DataValue::decodeFromYaml(const std::string& yaml)
{
    // Initialize the parser
    yaml_parser_t parser;
    yaml_parser_initialize(&parser);
    yaml_parser_set_input_string(&parser, reinterpret_cast<const unsigned char*>(yaml.c_str()), yaml.size());

    // Attempt to parse the document
    yaml_document_t document;
    std::string errorMessage;
    if (!yaml_parser_load(&parser, &document))
    {
        // Remember the error message
        errorMessage = format("Invalid YAML: %s on line %i column %i", parser.problem, parser.problem_mark.line, parser.problem_mark.column);
    }

    if (errorMessage.empty())
    {
        yaml_node_t* node = yaml_document_get_root_node(&document);
        if (node)
        {
            // Convert all nodes recursively
            *this = fromYaml(&document, node);
        }
    }

    // Clean up document and parser
    yaml_document_delete(&document);
    yaml_parser_delete(&parser);

    // Throw the error if there was one
    if (!errorMessage.empty())
    {
        throw DecodeError(errorMessage);
    }
}

void DataValue::decodeFromYaml(ReadStream& stream)
{
    decodeFromYaml(stream.readAllToString());
}