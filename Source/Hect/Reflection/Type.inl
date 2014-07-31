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
const Type& Type::of(T& value)
{
    std::type_index typeIndex(typeid(value));

    auto it = _registeredTypes.find(typeIndex);
    if (it != _registeredTypes.end())
    {
        return it->second;
    }
}

template <typename T>
const Type& Type::get()
{
    std::type_index typeIndex(typeid(T));

    auto it = _registeredTypes.find(typeIndex);
    if (it != _registeredTypes.end())
    {
        return it->second;
    }
    else
    {
        std::string typeName = typeid(T).name();
        size_t i = typeName.size() - 1;
        while (i > 0 && (typeName[i] != ':' && typeName[i] != ' '))
        {
            --i;
        }
        return create<T>(Kind_None, std::string(&typeName[i + 1]));
    }
}

template <typename T>
Type& Type::create(Kind kind, const std::string& name)
{
    std::type_index typeIndex(typeid(T));

    auto it = _registeredTypes.find(typeIndex);
    if (it != _registeredTypes.end())
    {
        throw Error("Type already exists");
    }

    return _registeredTypes[typeIndex] = Type(kind, name);
}

}
