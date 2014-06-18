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

#include "Hect/Logic/ComponentPool.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

class Entity :
    public Encodable
{
    friend class Scene;
public:
    Entity();
    Entity(Scene& scene, EntityId id);

    void addComponentBase(const ComponentBase& component);

    template <typename T>
    typename ComponentPool<T>::Iterator addComponent(T component);

    template <typename T>
    bool removeComponent();

    template <typename T>
    typename ComponentPool<T>::Iterator component();

    Entity clone() const;
    void destroy();
    void activate();

    bool isActivated() const;

    bool exists() const;

    Scene& scene();
    const Scene& scene() const;

    EntityId id() const;

    void encode(ObjectEncoder& encoder) const;
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

    bool operator==(const Entity& entity) const;
    bool operator!=(const Entity& entity) const;

    operator bool() const;

private:

    void _ensureExists() const;

    Scene* _scene;
    EntityId _id;
};

}

#include "Entity.inl"