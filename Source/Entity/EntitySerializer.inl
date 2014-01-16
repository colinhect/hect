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

template <typename T, typename S>
void EntitySerializer::registerComponent(const std::string& componentTypeName)
{
    ComponentTypeId typeId = T::typeId();

    // Check that the type name is not already registered
    if (_componentTypeIds.find(componentTypeName) != _componentTypeIds.end())
    {
        throw Error(format("Component type '%s' is already registered", componentTypeName.c_str()));
    }

    // Check that the type id is not already registered
    if (_componentSerializers.find(typeId) != _componentSerializers.end())
    {
        throw Error(format("Component type id '%d' is already registered", typeId));
    }

    // Map the type name to the type id
    _componentTypeIds[componentTypeName] = typeId;

    // Map the type id to the type name
    _componentTypeNames[typeId] = componentTypeName;

    // Create the serializer
    _componentSerializers[typeId] = BaseComponentSerializer::Ref(new S());

    // Create the constructor
    _componentConstructors[typeId] = [] { return new T(); };
}

}