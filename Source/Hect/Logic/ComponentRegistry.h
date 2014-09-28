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
#pragma once

#include <functional>
#include <map>
#include <string>

#include "Hect/Core/Uncopyable.h"
#include "Hect/Logic/Component.h"
#include "Hect/Logic/ComponentPool.h"

namespace hect
{

typedef std::map<std::type_index, ComponentPoolBase::Pointer> ComponentPoolMap;

class ComponentRegistry :
    public Uncopyable
{
public:
    static ComponentBase::Pointer createComponent(std::type_index typeIndex);
    static ComponentBase::Pointer createComponent(const std::string& typeName);

    static ComponentPoolBase::Pointer createComponentPool(std::type_index typeIndex, World& world);
    static ComponentPoolBase::Pointer createComponentPool(const std::string& typeName, World& world);

    static ComponentPoolMap createComponentPoolMap(World& world);

    template <typename T>
    static void registerType();

private:
    static ComponentTypeId _nextTypeId;

    static std::map<std::string, ComponentTypeId> _typeNameToId;
    static std::map<std::type_index, ComponentTypeId> _typeIndexToId;

    typedef std::function<ComponentBase::Pointer(void)> ComponentConstructor;
    typedef std::function<ComponentPoolBase::Pointer(World&)> ComponentPoolConstructor;

    static std::map<ComponentTypeId, ComponentConstructor> _componentConstructors;
    static std::map<ComponentTypeId, ComponentPoolConstructor> _componentPoolConstructors;
};

}

#include "ComponentRegistry.inl"