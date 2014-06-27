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

#include "Hect/Logic/Component.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

///
/// A dynamic object in a Scene.
class Entity :
    public Encodable,
    public Uncopyable
{
    friend class Scene;
    friend class EntityPool;
    friend class std::allocator<Entity>;
private:
    class IterBase
    {
    public:
        IterBase();
        IterBase(EntityPool& pool, EntityId id);

    protected:
        void _increment();
        bool _isValid() const;
        void _ensureValid() const;
        bool _equals(const IterBase& other) const;

        mutable EntityPool* _pool;
        EntityId _id;
    };

public:

    ///
    /// A predicate for a entity search or filter.
    typedef std::function<bool(const Entity&)> Predicate;

    ///
    /// A entity iterator.
    class Iter :
        public IterBase
    {
        friend class Entity;
    public:

        ///
        /// An array of entity iterators.
        typedef std::vector<Iter> Array;

        ///
        /// Constructs an invalid entity iterator.
        Iter();

        Iter(EntityPool& pool, EntityId id);

        ///
        /// Dereferences the iterator to a reference to the entity.
        ///
        /// \returns A reference to the entity.
        ///
        /// \throws Error If the iterator is invalid.
        Entity& operator*() const;

        ///
        /// Dereferences the iterator to a pointer to the entity.
        ///
        /// \returns A reference to the entity.
        ///
        /// \throws Error If the iterator is invalid.
        Entity* operator->() const;

        ///
        /// Moves to the next activated entity in the entity pool.
        ///
        /// \returns A reference to the iterator.
        Iter& operator++();

        ///
        /// Returns whether the iterator is equivalent to another.
        ///
        /// \param other The other iterator.
        bool operator==(const Iter& other) const;

        ///
        /// Returns whether the iterator is different from another.
        ///
        /// \param other The other iterator.
        bool operator!=(const Iter& other) const;

        ///
        /// Returns whether the iterator is valid.
        operator bool() const;
    };

    ///
    /// A constant entity iterator.
    class ConstIter :
        public IterBase
    {
        friend class Entity;
    public:

        ///
        /// An array of entity iterators.
        typedef std::vector<ConstIter> Array;

        ///
        /// Constructs an invalid entity iterator.
        ConstIter();

        ConstIter(const EntityPool& pool, EntityId id);

        ///
        /// Dereferences the iterator to a reference to the entity.
        ///
        /// \returns A reference to the entity.
        ///
        /// \throws Error If the iterator is invalid.
        const Entity& operator*() const;

        ///
        /// Dereferences the iterator to a pointer to the entity.
        ///
        /// \returns A reference to the entity.
        ///
        /// \throws Error If the iterator is invalid.
        const Entity* operator->() const;

        ///
        /// Moves to the next activated entity in the entity pool.
        ///
        /// \returns A reference to the iterator.
        ConstIter& operator++();

        ///
        /// Returns whether the iterator is equivalent to another.
        ///
        /// \param other The other iterator.
        bool operator==(const ConstIter& other) const;

        ///
        /// Returns whether the iterator is different from another.
        ///
        /// \param other The other iterator.
        bool operator!=(const ConstIter& other) const;

        ///
        /// Returns whether the iterator is valid.
        operator bool() const;
    };

    class Handle { };

    ///
    /// An entity's child entities.
    class Children
    {
    private:
        class IterBase
        {
        public:
            IterBase();
            IterBase(EntityPool& pool, EntityId parentId, size_t index);
            
        protected:
            void _increment();
            bool _isValid() const;
            void _ensureValid() const;
            bool _equals(const IterBase& other) const;

            EntityPool* _pool;
            EntityId _parentId;
            size_t _index;
        };
    public:

        ///
        /// A entity child iterator.
        class Iter :
            public IterBase
        {
            friend class Entity;
        public:

            ///
            /// An array of entity iterators.
            typedef std::vector<Iter> Array;

            ///
            /// Constructs an invalid entity iterator.
            Iter();

            Iter(EntityPool& pool, EntityId parentId, size_t index);

            ///
            /// Dereferences the iterator to a reference to the entity.
            ///
            /// \returns A reference to the entity.
            ///
            /// \throws Error If the iterator is invalid.
            Entity& operator*() const;

            ///
            /// Dereferences the iterator to a pointer to the entity.
            ///
            /// \returns A reference to the entity.
            ///
            /// \throws Error If the iterator is invalid.
            Entity* operator->() const;

            ///
            /// Moves to the next activated entity in the entity pool.
            ///
            /// \returns A reference to the iterator.
            Iter& operator++();

            ///
            /// Returns whether the iterator is equivalent to another.
            ///
            /// \param other The other iterator.
            bool operator==(const Iter& other) const;

            ///
            /// Returns whether the iterator is different from another.
            ///
            /// \param other The other iterator.
            bool operator!=(const Iter& other) const;

            ///
            /// Returns whether the iterator is valid.
            operator bool() const;
        };

        ///
        /// A constant entity child iterator.
        class ConstIter :
            public IterBase
        {
            friend class Entity;
        public:

            ///
            /// An array of entity iterators.
            typedef std::vector<ConstIter> Array;

            ///
            /// Constructs an invalid entity iterator.
            ConstIter();

            ConstIter(const EntityPool& pool, EntityId parentId, size_t index);

            ///
            /// Dereferences the iterator to a reference to the entity.
            ///
            /// \returns A reference to the entity.
            ///
            /// \throws Error If the iterator is invalid.
            const Entity& operator*() const;

            ///
            /// Dereferences the iterator to a pointer to the entity.
            ///
            /// \returns A reference to the entity.
            ///
            /// \throws Error If the iterator is invalid.
            const Entity* operator->() const;

            ///
            /// Moves to the next activated entity in the entity pool.
            ///
            /// \returns A reference to the iterator.
            ConstIter& operator++();

            ///
            /// Returns whether the iterator is equivalent to another.
            ///
            /// \param other The other iterator.
            bool operator==(const ConstIter& other) const;

            ///
            /// Returns whether the iterator is different from another.
            ///
            /// \param other The other iterator.
            bool operator!=(const ConstIter& other) const;

            ///
            /// Returns whether the iterator is valid.
            operator bool() const;
        };

        ///
        /// Returns an iterator to the beginning of the children.
        Iter begin();

        ///
        /// Returns an iterator to the beginning of the children.
        ConstIter begin() const;

        ///
        /// Returns an iterator to the end of the children.
        Iter end();

        ///
        /// Returns an iterator to the end of the children.
        ConstIter end() const;
    };

    ///
    /// Adds a component of a specific type to the entity.
    ///
    /// \param component The component to add to the entity.  A copy of this
    /// will be added to the entity so the original reference will not refer
    /// to the same component added to the entity.
    ///
    /// \returns An iterator to the added component.
    ///
    /// \throws Error If the entity already has a component of the type or if
    /// the entity is invalid.
    template <typename T>
    typename Component<T>::Iter addComponent(const T& component);

    ///
    /// Replaces an existing component that the entity has.
    ///
    /// \param component The component to replace the existing component with.
    /// A copy of this will be replaced for the existing component so the
    /// original reference will not refer to the same component replace for the
    /// entity.
    ///
    /// \returns An iterator to the added component.
    ///
    /// \throws Error If the entity does not have a component of the type or if
    /// the entity is invalid.
    template <typename T>
    typename Component<T>::Iter replaceComponent(const T& component);

    ///
    /// Removes the component of a specific type from the entity.
    ///
    /// \throws Error If the entity does not have a component of the type or if
    /// the entity is invalid.
    template <typename T>
    void removeComponent();

    ///
    /// Returns an iterator to the component of a specific type for the entity.
    ///
    /// \note The returned iterator is invalid if the entity does not have a
    /// component of the type.
    ///
    /// \throws Error If the entity is invalid.
    template <typename T>
    typename Component<T>::Iter component();

    ///
    /// Returns an iterator to the component of a specific type for the entity.
    ///
    /// \note The returned iterator is invalid if the entity does not have a
    /// component of the type.
    ///
    /// \throws Error If the entity is invalid.
    template <typename T>
    typename Component<T>::ConstIter component() const;

    ///
    /// Clones the entity.
    ///
    /// \param name The name of the cloned entity.
    ///
    /// \returns An iterator to the cloned entity.
    ///
    /// \throws Error If the entity is invalid.
    Entity::Iter clone(const std::string& name = std::string()) const;

    ///
    /// Destroys the entity and all of its children.
    ///
    /// \throws Error If the entity is invalid.
    void destroy();

    ///
    /// Activates the entity and all of its children.
    ///
    /// \throws Error If the entity is already activated or the entity is
    /// invalid.
    void activate();

    ///
    /// Returns whether the entity is activated.
    bool isActivated() const;

    ///
    /// Returns the id of the entity.
    EntityId id() const;

    ///
    /// Returns the name of the entity.
    const std::string& name() const;

    ///
    /// Sets the name of the entity.
    ///
    /// \param name The new name of the entity.
    void setName(const std::string& name);

    ///
    /// Returns an iterator to the parent entity (invalid iterator if the
    /// entity does not have a parent).
    Entity::Iter parent();

    ///
    /// Returns an iterator to the parent entity (invalid iterator if the
    /// entity does not have a parent).
    Entity::ConstIter parent() const;

    ///
    /// Adds an entity as a child of the entity.
    ///
    /// \param entity The entity to add as a child.
    ///
    /// \throws Error If the entity is invalid, in another scene, in a
    /// differing activation state, or is a child of another entity.
    void addChild(Entity& entity);

    ///
    /// Removes a child entity from the entity.
    ///
    /// \param entity The child entity to remove.
    ///
    /// \throws Error If the entity is invalid, in another scene, or is not a
    /// child of the entity.
    void removeChild(Entity& entity);

    ///
    /// Returns the children of the entity.
    Entity::Children& children();

    ///
    /// Returns the children of the entity.
    const Entity::Children& children() const;

    ///
    /// Returns an iterator to the first child entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::Iter findFirstChild(Predicate predicate);

    ///
    /// Returns an iterator to the first child entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::ConstIter findFirstChild(Predicate predicate) const;

    ///
    /// Returns an iterator to the first descendant entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::Iter findFirstDescendant(Predicate predicate);

    ///
    /// Returns an iterator to the first descendant entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::ConstIter findFirstDescendant(Predicate predicate) const;

    ///
    /// Returns an iterator to the first ancestor entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::Iter findFirstAncestor(Predicate predicate);

    ///
    /// Returns an iterator to the first ancestor entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::ConstIter findFirstAncestor(Predicate predicate) const;

    ///
    /// Returns iterators to all child entities matching the given predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An array of iterators to the matching entities.
    Entity::Iter::Array findChildren(Predicate predicate);

    ///
    /// Returns iterators to all child entities matching the given predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An array of iterators to the matching entities.
    Entity::ConstIter::Array findChildren(Predicate predicate) const;

    ///
    /// Returns iterators to all descendant entities matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An array of iterators to the matching entities.
    Entity::Iter::Array findDescendants(Predicate predicate);

    ///
    /// Returns iterators to all descendant entities matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An array of iterators to the matching entities.
    Entity::ConstIter::Array findDescendants(Predicate predicate) const;

    ///
    /// Returns iterators to all ancestor entities matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An array of iterators to the matching entities.
    Entity::Iter::Array findAncestors(Predicate predicate);

    ///
    /// Returns iterators to all ancestor entities matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An array of iterators to the matching entities.
    Entity::ConstIter::Array findAncestors(Predicate predicate) const;

    void encode(ObjectEncoder& encoder) const;
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

private:
    Entity();
    Entity(const Entity& entity);
    Entity(Entity&& entity);

    void _addComponentBase(const ComponentBase& component);

    void _enterPool(EntityPool& pool, EntityId id);
    void _exitPool();
    bool _inPool() const;
    void _ensureInPool() const;

    Children _children;
    EntityPool* _pool;
    EntityId _id;
    EntityId _parentId;
    std::vector<EntityId> _childIds;
    std::string _name;
    bool _activated;
};

}

#include "Entity.inl"