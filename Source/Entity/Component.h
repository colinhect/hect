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

namespace hect
{

class Entity;
class Scene;

///
/// A number identifying a component type.
typedef uint32_t ComponentTypeId;

///
/// Base entity component.
class BaseComponent
{
public:

    ///
    /// A shared reference to a base component.
    typedef std::shared_ptr<BaseComponent> Ref;

    ///
    /// Returns the next available component type id.
    static ComponentTypeId nextTypeId();

    virtual ~BaseComponent() { }

    ///
    /// Clones the component.
    virtual BaseComponent* clone() const = 0;

    ///
    /// Returns the type id of the component.
    virtual ComponentTypeId componentTypeId() const = 0;
};

///
/// A component of an entity.
template <typename T>
class Component :
    public BaseComponent
{
public:

    ///
    /// Returns the component type id for this type of component.
    static ComponentTypeId typeId();

    BaseComponent* clone() const;
    ComponentTypeId componentTypeId() const;
};

}

#include "Component.inl"
