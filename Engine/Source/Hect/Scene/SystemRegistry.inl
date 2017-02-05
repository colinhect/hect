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
#include "Hect/Reflection/Type.h"

#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"

namespace hect
{

template <typename T>
void SystemRegistry::registerType()
{
    std::type_index typeIndex(typeid(T));

    if (_typeIndexToId.find(typeIndex) == _typeIndexToId.end())
    {
        Name typeName = Type::get<T>().name();
        SystemTypeId typeId = static_cast<SystemTypeId>(_typeIds.size());

        _typeIndexToId[typeIndex] = typeId;
        _typeNameToId[typeName] = typeId;
        _typeIdToName[typeId] = typeName;
        _typeIds.push_back(typeId);

        HECT_DEBUG(format("Registered system type '%s' (type id: %d)", typeName.data(), typeId));
    }
}

template <typename T>
SystemTypeId SystemRegistry::typeIdOf()
{
    static SystemTypeId id = SystemTypeId(-1);
    if (id == SystemTypeId(-1))
    {
        std::type_index typeIndex(typeid(T));
        id = typeIdOf(typeIndex);
    }
    return id;
}

}
