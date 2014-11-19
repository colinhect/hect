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
#include "Hect/Reflection/Type.h"

namespace hect
{

template <typename T>
void ComponentRegistry::registerType()
{
    std::type_index typeIndex(typeid(T));

    if (_typeIndexToId.find(typeIndex) == _typeIndexToId.end())
    {
        std::string typeName = Type::get<T>().name();

        ComponentTypeId typeId = static_cast<ComponentTypeId>(_componentConstructors.size());

        _componentConstructors.push_back([]()
        {
            return std::shared_ptr<ComponentBase>(new T());
        });

        _componentPoolConstructors.push_back([](Scene& scene)
        {
            return std::shared_ptr<ComponentPoolBase>(new ComponentPool<T>(scene));
        });

        _typeIndexToId[typeIndex] = typeId;
        _typeNameToId[typeName] = typeId;
    }
}

template <typename T>
ComponentTypeId ComponentRegistry::typeIdOf()
{
    static ComponentTypeId id = ComponentTypeId(-1);
    if (id == ComponentTypeId(-1))
    {
        std::type_index typeIndex(typeid(T));
        id = typeIdOf(typeIndex);
    }
    return id;
}

}