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
T& World::system()
{
    std::type_index typeIndex(typeid(T));

    auto it = _systemMap.find(typeIndex);
    if (it == _systemMap.end())
    {
        std::string typeName = Type::get<T>().name();
        throw Error(format("Unknown system type '%s'", typeName.c_str()));
    }

    return (T&)*it->second;
}

template <typename T>
ComponentPool<T>& World::components()
{
    std::type_index typeIndex(typeid(T));

    auto it = _componentPoolMap.find(typeIndex);
    if (it == _componentPoolMap.end())
    {
        std::string typeName = Type::get<T>().name();
        throw Error(format("Unknown component type '%s'", typeName.c_str()));
    }

    return (ComponentPool<T>&)*it->second;
}

template <typename T>
const ComponentPool<T>& World::components() const
{
    return const_cast<World*>(this)->components<T>();
}

}