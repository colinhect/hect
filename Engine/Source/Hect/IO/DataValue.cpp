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
#include "DataValue.h"

#define YAML_DECLARE_STATIC
#include <yaml.h>

#include <regex>
#include <sstream>

#include "Hect/IO/ReadStream.h"

using namespace hect;

const DataValue DataValue::_null;
const DataValue::Array DataValue::_empty_array;

DataValue::DataValue() :
    _type(DataValueType::Null)
{
}

DataValue::DataValue(DataValueType type) :
    _type(type)
{
    switch (type)
    {
    case DataValueType::Bool:
        _any = false;
        break;
    case DataValueType::Number:
        _any = 0.0;
        break;
    case DataValueType::String:
        _any = std::string();
        break;
    case DataValueType::Array:
        _any = DataValue::Array();
        break;
    case DataValueType::Object:
        _any = DataValue::Object();
        break;
    default:
        break;
    }
}

DataValue::DataValue(bool value) :
    _type(DataValueType::Bool),
    _any(value)
{
}

DataValue::DataValue(int value) :
    _type(DataValueType::Number),
    _any(static_cast<double>(value))
{
}

DataValue::DataValue(unsigned value) :
    _type(DataValueType::Number),
    _any(static_cast<double>(value))
{
}

DataValue::DataValue(double value) :
    _type(DataValueType::Number),
    _any(value)
{
}

DataValue::DataValue(Vector2 value) :
    _type(DataValueType::Array),
    _any(DataValue::Array())
{
    DataValue::Array& elements = _any.as<DataValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
}

DataValue::DataValue(Vector3 value) :
    _type(DataValueType::Array),
    _any(DataValue::Array())
{
    DataValue::Array& elements = _any.as<DataValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
    elements.push_back(value.z);
}

DataValue::DataValue(Vector4 value) :
    _type(DataValueType::Array),
    _any(DataValue::Array())
{
    DataValue::Array& elements = _any.as<DataValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
    elements.push_back(value.z);
    elements.push_back(value.w);
}

DataValue::DataValue(const Matrix4& value) :
    _type(DataValueType::Array),
    _any(DataValue::Array())
{
    DataValue::Array& elements = _any.as<DataValue::Array>();
    for (unsigned i = 0; i < 16; ++i)
    {
        elements.push_back(value[i]);
    }
}

DataValue::DataValue(Quaternion value) :
    _type(DataValueType::Array),
    _any(DataValue::Array())
{
    DataValue::Array& elements = _any.as<DataValue::Array>();
    elements.push_back(value.x);
    elements.push_back(value.y);
    elements.push_back(value.z);
    elements.push_back(value.w);
}

DataValue::DataValue(const char* value) :
    _type(DataValueType::String),
    _any(std::string(value))
{
}

DataValue::DataValue(const std::string& value) :
    _type(DataValueType::String),
    _any(value)
{
}

DataValue::DataValue(Name value) :
    _type(DataValueType::String),
    _any(value.as_string())
{
}

DataValue::DataValue(const DataValue& data_value) :
    _type(data_value._type),
    _any(data_value._any)
{
}

DataValue::DataValue(DataValue&& data_value) :
    _type(data_value._type),
    _any(std::move(data_value._any))
{
    data_value._type = DataValueType::Null;
}

DataValueType DataValue::type() const
{
    return _type;
}

const DataValue& DataValue::or_default(const DataValue& data_value) const
{
    if (is_null())
    {
        return data_value;
    }
    else
    {
        return *this;
    }
}

bool DataValue::is_null() const
{
    return _type == DataValueType::Null;
}

bool DataValue::is_bool() const
{
    return _type == DataValueType::Bool;
}

bool DataValue::is_number() const
{
    return _type == DataValueType::Number;
}

bool DataValue::is_string() const
{
    return _type == DataValueType::String;
}

bool DataValue::is_array() const
{
    return _type == DataValueType::Array;
}

bool DataValue::is_object() const
{
    return _type == DataValueType::Object;
}

bool DataValue::as_bool() const
{
    if (is_bool())
    {
        return _any.as<bool>();
    }
    else
    {
        return false;
    }
}

int DataValue::as_int() const
{
    if (is_number())
    {
        return static_cast<int>(_any.as<double>());
    }
    else
    {
        return 0;
    }
}

unsigned DataValue::as_unsigned() const
{
    if (is_number())
    {
        return static_cast<unsigned>(_any.as<double>());
    }
    else
    {
        return 0;
    }
}

double DataValue::as_double() const
{
    if (is_number())
    {
        return _any.as<double>();
    }
    else
    {
        return 0.0;
    }
}

Vector2 DataValue::as_vector2() const
{
    Vector2 result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 2)
        {
            result[i++] = component.as_double();
        }
        else
        {
            break;
        }
    }

    return result;
}

Vector3 DataValue::as_vector3() const
{
    Vector3 result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 3)
        {
            result[i++] = component.as_double();
        }
        else
        {
            break;
        }
    }

    return result;
}

Vector4 DataValue::as_vector4() const
{
    Vector4 result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 4)
        {
            result[i++] = component.as_double();
        }
        else
        {
            break;
        }
    }

    return result;
}

Matrix4 DataValue::as_matrix4() const
{
    Matrix4 result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 16)
        {
            result[i++] = component.as_double();
        }
        else
        {
            break;
        }
    }

    return result;
}

Quaternion DataValue::as_quaternion() const
{
    Quaternion result;

    size_t i = 0;
    for (const DataValue& component : *this)
    {
        if (i < 4)
        {
            result[i++] = component.as_double();
        }
        else
        {
            break;
        }
    }

    return result;
}

const std::string& DataValue::as_string() const
{
    static std::string empty;

    if (is_string())
    {
        return _any.as<std::string>();
    }
    else
    {
        return empty;
    }
}

Name DataValue::as_name() const
{
    Name name;

    if (is_string())
    {
        name = _any.as<std::string>();
    }

    return name;
}

size_t DataValue::size() const
{
    if (is_array())
    {
        return _any.as<DataValue::Array>().size();
    }
    else if (is_object())
    {
        return _any.as<DataValue::Object>().size();
    }

    return 0;
}

std::vector<std::string> DataValue::member_names() const
{
    if (is_object())
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

void DataValue::add_member(const std::string& name, const DataValue& data_value)
{
    if (is_object())
    {
        _any.as<DataValue::Object>()[name] = data_value;
    }
    else
    {
        throw InvalidOperation("The data value is not an object");
    }
}

void DataValue::add_element(const DataValue& data_value)
{
    if (is_array())
    {
        _any.as<DataValue::Array>().push_back(data_value);
    }
    else
    {
        throw InvalidOperation("The data value is not an array");
    }
}

const DataValue& DataValue::operator[](size_t index) const
{
    if (is_array())
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
    if (is_object())
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

DataValue& DataValue::operator=(const DataValue& data_value)
{
    _type = data_value._type;
    _any = data_value._any;
    return *this;
}

DataValue& DataValue::operator=(DataValue&& data_value)
{
    _type = data_value._type;
    _any = std::move(data_value._any);
    return *this;
}

DataValue::Array::const_iterator DataValue::begin() const
{
    if (is_array())
    {
        return _any.as<DataValue::Array>().begin();
    }
    else
    {
        return _empty_array.begin();
    }
}

DataValue::Array::const_iterator DataValue::end() const
{
    if (is_array())
    {
        return _any.as<DataValue::Array>().end();
    }
    else
    {
        return _empty_array.end();
    }
}

namespace hect
{

static std::regex number_regex("[-+]?[0-9]*\\.?[0-9]+");

// Converts a YAML node to a DataValue
DataValue from_yaml(yaml_document_t* document, yaml_node_t* node)
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
            bool bool_value = string == "true";
            return DataValue(bool_value);
        }

        // Check if it is a number
        else if (std::regex_match(string, number_regex))
        {
            // Parse the number
            double number_value = 0.0;
            std::istringstream ss(string);
            ss >> number_value;

            return DataValue(number_value);
        }

        // Otherwise it is just a string
        else
        {
            return DataValue(string);
        }
    }
    case YAML_SEQUENCE_NODE:
    {
        DataValue data_value(DataValueType::Array);

        // For each item in the sequence
        for (yaml_node_item_t* item = node->data.sequence.items.start;
                item < node->data.sequence.items.top; item++)
        {
            // Convert the item to a DataValue
            yaml_node_t* item_node = yaml_document_get_node(document, *item);
            DataValue element = from_yaml(document, item_node);

            // Add the element
            data_value.add_element(element);
        }

        return data_value;
    }
    case YAML_MAPPING_NODE:
    {
        DataValue data_value(DataValueType::Object);

        // For each pair in the mapping
        for (yaml_node_pair_t* pair = node->data.mapping.pairs.start;
                pair < node->data.mapping.pairs.top; pair++)
        {
            // Get the name from the pair
            yaml_node_t* key_node = yaml_document_get_node(document, pair->key);
            const char* name = reinterpret_cast<const char*>(key_node->data.scalar.value);

            // Convert the value node to a DataValue
            yaml_node_t* value_node = yaml_document_get_node(document, pair->value);
            DataValue value = from_yaml(document, value_node);

            // Add the member
            data_value.add_member(name, value);
        }

        return data_value;
    }
    default:
        return DataValue();
    }
}

}

void DataValue::decode_from_yaml(const std::string& yaml)
{
    // Initialize the parser
    yaml_parser_t parser;
    yaml_parser_initialize(&parser);
    yaml_parser_set_input_string(&parser, reinterpret_cast<const unsigned char*>(yaml.data()), yaml.size());

    // Attempt to parse the document
    yaml_document_t document;
    std::string error_message;
    if (!yaml_parser_load(&parser, &document))
    {
        // Remember the error message
        error_message = format("Invalid YAML: %s on line %i column %i", parser.problem, parser.problem_mark.line, parser.problem_mark.column);
    }

    if (error_message.empty())
    {
        yaml_node_t* node = yaml_document_get_root_node(&document);
        if (node)
        {
            // Convert all nodes recursively
            *this = from_yaml(&document, node);
        }
    }

    // Clean up document and parser
    yaml_document_delete(&document);
    yaml_parser_delete(&parser);

    // Throw the error if there was one
    if (!error_message.empty())
    {
        throw DecodeError(error_message);
    }
}

void DataValue::decode_from_yaml(ReadStream& stream)
{
    decode_from_yaml(stream.read_all_to_string());
}
