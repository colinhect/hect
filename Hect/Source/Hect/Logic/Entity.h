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

#include "Hect/Logic/Component.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

class EntityPool;

class Entity :
    public Encodable
{
private:
    class IterBase
    {
    public:
        IterBase();
        IterBase(EntityPool* pool, EntityId id);

    protected:
        void _nextValidEntity();
        void _increment();
        bool _isValid() const;
        void _ensureValid() const;
        bool _equals(const IterBase& other) const;

        mutable EntityPool* _pool;
        EntityId _id;
    };

public:
    class Iter :
        public IterBase
    {
    public:
        Iter();
        Iter(EntityPool* pool, EntityId id);

        Entity& operator*() const;
        Entity* operator->() const;
        Iter& operator++();

        bool operator==(const Iter& other) const;
        bool operator!=(const Iter& other) const;

        operator bool() const;
    };

    class ConstIter :
        public IterBase
    {
    public:
        ConstIter();
        ConstIter(const EntityPool* pool, EntityId id);

        const Entity& operator*() const;
        const Entity* operator->() const;
        ConstIter& operator++();

        bool operator==(const ConstIter& other) const;
        bool operator!=(const ConstIter& other) const;

        operator bool() const;
    };

    class Handle { };

    Entity();

    void addComponentBase(const ComponentBase& component);

    template <typename T>
    typename Component<T>::Iter addComponent(T component);

    template <typename T>
    typename Component<T>::Iter replaceComponent(T component);

    template <typename T>
    void removeComponent();

    template <typename T>
    typename Component<T>::Iter component();

    Entity::Iter clone() const;
    void destroy();
    void activate();

    bool isActivated() const;

    void enterPool(EntityPool* pool, EntityId id);
    void exitPool();

    bool inPool() const;

    EntityId id() const;

    void encode(ObjectEncoder& encoder) const;
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

private:
    void _ensureInPool() const;

    EntityPool* _pool;
    EntityId _id;
    bool _activated;
};

}

#include "Entity.inl"