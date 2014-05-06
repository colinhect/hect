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
namespace hect
{

template <typename T>
T Enum::fromString(const std::string& string)
{
    const Type& type = Type::get<T>();
    std::map<std::string, int>& stringToValue = _stringToValue[std::type_index(typeid(T))];
    auto it = stringToValue.find(string);
    if (it != stringToValue.end())
    {
        return (T)it->second;
    }
    else
    {
        throw Error(format("Invalid string '%s' for type '%s'", string.c_str(), type.name().c_str()));
    }
}

template <typename T>
const std::string& Enum::toString(T value)
{
    const Type& type = Type::get<T>();
    std::map<int, std::string>& valueToString = _valueToString[std::type_index(typeid(T))];
    auto it = valueToString.find(value);
    if (it != valueToString.end())
    {
        return it->second;
    }
    else
    {
        throw Error(format("Invalid value for type '%s'", type.name().c_str()));
    }
}

template <typename T>
void Enum::add(const std::string& string, T value)
{
    Type::get<T>();
    _stringToValue[std::type_index(typeid(T))][string] = (int)value;
    _valueToString[std::type_index(typeid(T))][(int)value] = string;
}

template <typename T>
Type& Type::create(const std::string& name)
{
    std::shared_ptr<Type> type(new Type(name));
    _types[std::type_index(typeid(T))] = type;
    return *type;
}

template <typename T>
const Type& Type::get()
{
    Type* type = _types[std::type_index(typeid(T))].get();
    if (type)
    {
        return *type;
    }
    else
    {
        throw Error("Unknown type");
    }
}

template <typename T>
const Type& Type::of(const T& object)
{
    Type* type = _types[std::type_index(typeid(object))].get();
    if (type)
    {
        return *type;
    }
    else
    {
        throw Error("Unknown type");
    }
}

}