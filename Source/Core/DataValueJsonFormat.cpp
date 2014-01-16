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

#include <json/json.h>

using namespace hect;

DataValue toDataValue(Json::Value& jsonValue);
Json::Value fromDataValue(const DataValue& dataValue);

void DataValueJsonFormat::save(const DataValue& dataValue, std::string& json)
{
    Json::Value jsonValue = fromDataValue(dataValue);
    json = jsonValue.toStyledString();
}

void DataValueJsonFormat::save(const DataValue& dataValue, WriteStream& stream)
{
    std::string json;
    DataValueJsonFormat::save(dataValue, json);
    stream.writeString(json, false);
}

void DataValueJsonFormat::load(DataValue& dataValue, const std::string& json)
{
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(json, root))
    {
        throw Error(reader.getFormattedErrorMessages());
    }
    dataValue = toDataValue(root);
}

void DataValueJsonFormat::load(DataValue& dataValue, ReadStream& stream)
{
    std::string json = stream.readAllToString();
    load(dataValue, json);
}

DataValue toDataValue(Json::Value& jsonValue)
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
        DataValue dataValue(DataValueType::Array);
        for (Json::Value& element : jsonValue)
        {
            dataValue.addElement(toDataValue(element));
        }
        return dataValue;
    }
    else if (jsonValue.isObject())
    {
        DataValue dataValue(DataValueType::Object);
        for (std::string& name : jsonValue.getMemberNames())
        {
            dataValue.addMember(name, toDataValue(jsonValue[name]));
        }
        return dataValue;
    }

    return DataValue();
}

Json::Value fromDataValue(const DataValue& dataValue)
{
    if (dataValue.isBool())
    {
        return Json::Value(dataValue.asBool());
    }
    else if (dataValue.isNumber())
    {
        return Json::Value(dataValue.asDouble());
    }
    else if (dataValue.isString())
    {
        return Json::Value(dataValue.asString());
    }
    else if (dataValue.isArray())
    {
        Json::Value jsonValue(Json::arrayValue);
        for (const DataValue& element : dataValue)
        {
            jsonValue.append(fromDataValue(element));
        }
        return jsonValue;
    }
    else if (dataValue.isObject())
    {
        Json::Value jsonValue(Json::objectValue);
        for (const std::string& name : dataValue.memberNames())
        {
            jsonValue[name] = fromDataValue(dataValue[name]);
        }
        return jsonValue;
    }

    return Json::Value();
}