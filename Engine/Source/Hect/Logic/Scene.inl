///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
void Scene::addSystemType()
{
    SystemTypeId typeId = SystemRegistry::typeIdOf<T>();
    addSystemType(typeId);
}

template <typename T>
void Scene::removeSystemType()
{
    SystemTypeId typeId = SystemRegistry::typeIdOf<T>();

    // Ensure that the system is supported
    if (!hasSystemType<T>())
    {
        throw InvalidOperation(format("Scene does not support system type '%s'", Type::get<T>().name().c_str()));
    }

    // Remove the system from the tick stages
    for (std::vector<SystemTypeId>& tickStage : _tickStages)
    {
        tickStage.erase(std::remove(tickStage.begin(), tickStage.end(), typeId), tickStage.end());
    }

    // Remove the system
    _systems[typeId].reset();
}

template <typename T>
bool Scene::hasSystemType()
{
    SystemTypeId typeId = SystemRegistry::typeIdOf<T>();
    return typeId < _systems.size() && _systems[typeId];
}

template <typename T>
typename T::Handle Scene::system()
{
    SystemHandle<T> handle;

    if (hasSystemType<T>())
    {
        SystemTypeId typeId = SystemRegistry::typeIdOf<T>();
        handle = SystemHandle<T>((T&)*_systems[typeId]);
    }

    return handle;
}

template <typename T>
typename T::ConstHandle Scene::system() const
{
    SystemConstHandle<T> handle;

    if (hasSystemType<T>())
    {
        SystemTypeId typeId = SystemRegistry::typeIdOf<T>();
        handle = SystemConstHandle<T>((const T&)*_systems[typeId]);
    }

    return handle;
}

template <typename T>
void Scene::addComponentType()
{
    ComponentTypeId typeId = ComponentRegistry::typeIdOf<T>();
    addComponentType(typeId);
}

template <typename T>
bool Scene::hasComponentType()
{
    ComponentTypeId typeId = ComponentRegistry::typeIdOf<T>();
    return typeId < _componentPools.size() && _componentPools[typeId];
}

template <typename T>
ComponentPool<T>& Scene::components()
{
    ComponentTypeId typeId = ComponentRegistry::typeIdOf<T>();
    if (typeId >= _componentPools.size() || !_componentPools[typeId])
    {
        throw InvalidOperation(format("Scene does not support component type '%s'", Type::get<T>().name().c_str()));
    }
    return static_cast<ComponentPool<T>&>(*_componentPools[typeId]);
}

template <typename T>
const ComponentPool<T>& Scene::components() const
{
    return const_cast<Scene*>(this)->components<T>();
}

}