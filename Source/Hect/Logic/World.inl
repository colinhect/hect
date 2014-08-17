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

template <typename T, typename... Args>
T& World::addSystem(Args&... args)
{
    std::type_index typeIndex(typeid(T));
    
    if (_systems.find(typeIndex) != _systems.end())
    {
        std::string typeName = Type::get<T>().name();
        throw Error(format("System of type '%s' has already been added", typeName.c_str()));
    }

    _systems[typeIndex] = System::SharedPointer(new T(*this, args...));
    return (T&)*_systems[typeIndex];
}

template <typename T>
T& World::system()
{
    std::type_index typeIndex(typeid(T));

    auto it = _systems.find(typeIndex);
    if (it == _systems.end())
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
    ComponentPoolBase& componentPool = componentPoolFromTypeIndex(typeIndex);
    return (ComponentPool<T>&)componentPool;
}

template <typename T>
const ComponentPool<T>& World::components() const
{
    std::type_index typeIndex(typeid(T));
    ComponentPoolBase& componentPool = componentPoolFromTypeIndex(typeIndex);
    return (const ComponentPool<T>&)componentPool;
}

}