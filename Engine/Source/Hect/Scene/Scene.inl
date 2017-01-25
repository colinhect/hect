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
namespace hect
{

template <typename SystemType>
SystemType& Scene::createSystem()
{
    SystemTypeId typeId = SystemRegistry::typeIdOf<SystemType>();
    addSystemType(typeId);
    return system<SystemType>();
}

template <typename SystemType>
bool Scene::hasSystemType()
{
    SystemTypeId typeId = SystemRegistry::typeIdOf<SystemType>();
    return hasSystemOfTypeId(typeId);
}

template <typename SystemType>
SystemType& Scene::system()
{
    SystemTypeId typeId = SystemRegistry::typeIdOf<SystemType>();
    return *reinterpret_cast<SystemType*>(&systemOfTypeId(typeId));
}

template <typename SystemType>
const SystemType& Scene::system() const
{
    SystemTypeId typeId = SystemRegistry::typeIdOf<SystemType>();
    return *reinterpret_cast<SystemType*>(&systemOfTypeId(typeId));
}

template <typename ComponentType>
ComponentPool<ComponentType>& Scene::components()
{
    ComponentTypeId typeId = ComponentRegistry::typeIdOf<ComponentType>();
    if (typeId >= _componentPools.size() || !_componentPools[typeId])
    {
        const Name componentTypeName = Type::get<ComponentType>().name();
        throw InvalidOperation(format("Scene does not support component type '%s'", componentTypeName.data()));
    }
    return static_cast<ComponentPool<ComponentType>&>(*_componentPools[typeId]);
}

template <typename ComponentType>
const ComponentPool<ComponentType>& Scene::components() const
{
    return const_cast<Scene*>(this)->components<ComponentType>();
}

}
