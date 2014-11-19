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
void Scene::addDiscreteSystem()
{
    SystemTypeId typeId = SystemRegistry::typeIdOf<T>();

    bool added;
    addOrGetSystem(typeId, added);

    if (!added)
    {
        throw Error(format("Scene already has system of type '%s'", Type::get<T>().name().c_str()));
    }
}

template <typename T>
T& Scene::system()
{
    SystemTypeId typeId = SystemRegistry::typeIdOf<T>();
    if (typeId >= _systems.size() || !_systems[typeId])
    {
        throw Error(format("Scene does not have system of type '%s'", Type::get<T>().name().c_str()));
    }
    return (T&)*_systems[typeId];
}

template <typename T>
ComponentPool<T>& Scene::components()
{
    ComponentTypeId typeId = ComponentRegistry::typeIdOf<T>();
    return (ComponentPool<T>&)*_componentPoolMap[typeId];
}

template <typename T>
const ComponentPool<T>& Scene::components() const
{
    return const_cast<Scene*>(this)->components<T>();
}

}