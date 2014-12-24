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
#include <memory>

#include "Hect/Core/Event.h"
#include "Hect/Core/Export.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"
#include "Hect/Logic/Component.h"
#include "Hect/Logic/EntityEvent.h"

namespace hect
{

class Scene;
class EntityPool;

///
/// A dynamic object in a Scene.
class HECT_EXPORT Entity :
    public Uncopyable
{
    friend class Scene;
    friend class EntityPool;
private:
    class HECT_EXPORT IteratorBase
    {
    public:
        IteratorBase();
        IteratorBase(EntityPool& pool, EntityId id);

    protected:
        void increment();
        bool isValid() const;
        void ensureValid() const;
        bool equals(const IteratorBase& other) const;

        mutable EntityPool* _pool { nullptr };
        EntityId _id { EntityId(-1) };
    };

public:

    ///
    /// A predicate for a Entity search or filter.
    typedef std::function<bool(const Entity&)> Predicate;

    ///
    /// A Entity iterator.
    class HECT_EXPORT Iterator :
        public IteratorBase
    {
        friend class Entity;
    public:

        ///
        /// A vector of entity iterators.
        typedef std::vector<Iterator> Vector;

        ///
        /// Constructs an invalid entity iterator.
        Iterator();

        Iterator(EntityPool& pool, EntityId id);

        ///
        /// Dereferences the iterator to a reference to the entity.
        ///
        /// \returns A reference to the entity.
        ///
        /// \throws InvalidOperation If the iterator is invalid.
        Entity& operator*() const;

        ///
        /// Dereferences the iterator to a pointer to the entity.
        ///
        /// \returns A reference to the entity.
        ///
        /// \throws InvalidOperation If the iterator is invalid.
        Entity* operator->() const;

        ///
        /// Moves to the next activated entity in the entity pool.
        ///
        /// \returns A reference to the iterator.
        Iterator& operator++();

        ///
        /// Returns whether the iterator is equivalent to another.
        ///
        /// \param other The other iterator.
        bool operator==(const Iterator& other) const;

        ///
        /// Returns whether the iterator is different from another.
        ///
        /// \param other The other iterator.
        bool operator!=(const Iterator& other) const;

        ///
        /// Returns whether the iterator is valid.
        operator bool() const;
    };

    ///
    /// A constant Entity iterator.
    class HECT_EXPORT ConstIterator :
        public IteratorBase
    {
        friend class Entity;
    public:

        ///
        /// A vector of entity iterators.
        typedef std::vector<ConstIterator> Vector;

        ///
        /// Constructs an invalid entity iterator.
        ConstIterator();

        ConstIterator(const EntityPool& pool, EntityId id);

        ///
        /// Dereferences the iterator to a reference to the entity.
        ///
        /// \returns A reference to the entity.
        ///
        /// \throws InvalidOperation If the iterator is invalid.
        const Entity& operator*() const;

        ///
        /// Dereferences the iterator to a pointer to the entity.
        ///
        /// \returns A reference to the entity.
        ///
        /// \throws InvalidOperation If the iterator is invalid.
        const Entity* operator->() const;

        ///
        /// Moves to the next activated entity in the entity pool.
        ///
        /// \returns A reference to the iterator.
        ConstIterator& operator++();

        ///
        /// Returns whether the iterator is equivalent to another.
        ///
        /// \param other The other iterator.
        bool operator==(const ConstIterator& other) const;

        ///
        /// Returns whether the iterator is different from another.
        ///
        /// \param other The other iterator.
        bool operator!=(const ConstIterator& other) const;

        ///
        /// Returns whether the iterator is valid.
        operator bool() const;
    };

    ///
    /// An Entity's child entities.
    class HECT_EXPORT Children
    {
        class HECT_EXPORT IteratorBase
        {
        public:
            IteratorBase();
            IteratorBase(EntityPool& pool, EntityId parentId, size_t index);

        protected:
            void increment();
            bool isValid() const;
            void ensureValid() const;
            bool equals(const IteratorBase& other) const;

            EntityPool* _pool { nullptr };
            EntityId _parentId { EntityId(-1) };
            size_t _index { 0 };
        };

    public:

        ///
        /// A Entity child iterator.
        class HECT_EXPORT Iterator :
            public IteratorBase
        {
            friend class Entity;
        public:

            ///
            /// A vector of entity iterators.
            typedef std::vector<Iterator> Vector;

            ///
            /// Constructs an invalid entity iterator.
            Iterator();

            Iterator(EntityPool& pool, EntityId parentId, size_t index);

            ///
            /// Dereferences the iterator to a reference to the entity.
            ///
            /// \returns A reference to the entity.
            ///
            /// \throws InvalidOperation If the iterator is invalid.
            Entity& operator*() const;

            ///
            /// Dereferences the iterator to a pointer to the entity.
            ///
            /// \returns A reference to the entity.
            ///
            /// \throws InvalidOperation If the iterator is invalid.
            Entity* operator->() const;

            ///
            /// Moves to the next activated entity in the entity pool.
            ///
            /// \returns A reference to the iterator.
            Iterator& operator++();

            ///
            /// Returns whether the iterator is equivalent to another.
            ///
            /// \param other The other iterator.
            bool operator==(const Iterator& other) const;

            ///
            /// Returns whether the iterator is different from another.
            ///
            /// \param other The other iterator.
            bool operator!=(const Iterator& other) const;

            ///
            /// Returns whether the iterator is valid.
            operator bool() const;
        };

        ///
        /// A constant Entity child iterator.
        class HECT_EXPORT ConstIterator :
            public IteratorBase
        {
            friend class Entity;
        public:

            ///
            /// Constructs an invalid entity iterator.
            ConstIterator();

            ConstIterator(const EntityPool& pool, EntityId parentId, size_t index);

            ///
            /// Dereferences the iterator to a reference to the entity.
            ///
            /// \returns A reference to the entity.
            ///
            /// \throws InvalidOperation If the iterator is invalid.
            const Entity& operator*() const;

            ///
            /// Dereferences the iterator to a pointer to the entity.
            ///
            /// \returns A reference to the entity.
            ///
            /// \throws InvalidOperation If the iterator is invalid.
            const Entity* operator->() const;

            ///
            /// Moves to the next activated entity in the entity pool.
            ///
            /// \returns A reference to the iterator.
            ConstIterator& operator++();

            ///
            /// Returns whether the iterator is equivalent to another.
            ///
            /// \param other The other iterator.
            bool operator==(const ConstIterator& other) const;

            ///
            /// Returns whether the iterator is different from another.
            ///
            /// \param other The other iterator.
            bool operator!=(const ConstIterator& other) const;

            ///
            /// Returns whether the iterator is valid.
            operator bool() const;
        };

        ///
        /// Returns an iterator to the beginning of the children.
        Iterator begin();

        ///
        /// Returns an iterator to the beginning of the children.
        ConstIterator begin() const;

        ///
        /// Returns an iterator to the end of the children.
        Iterator end();

        ///
        /// Returns an iterator to the end of the children.
        ConstIterator end() const;
    };

    ///
    /// A weak reference to an Entity.
    class HECT_EXPORT Handle
    {
        friend class Entity;
    public:

        ///
        /// Constructs an invalid handle.
        Handle();

        ///
        /// Contructs a handle copied from another.
        ///
        /// \param handle The handle to copy.
        Handle(const Handle& handle);

        ///
        /// Contructs a handle moved from another.
        ///
        /// \param handle The handle to move.
        Handle(Handle&& handle);

        ///
        /// Dereferences the handle to a reference to the entity.
        ///
        /// \returns A reference to the entity.
        ///
        /// \throws InvalidOperation If the handle is invalid.
        Entity& operator*();

        ///
        /// Dereferences the handle to a reference to the entity.
        ///
        /// \returns A reference to the entity.
        ///
        /// \throws InvalidOperation If the handle is invalid.
        const Entity& operator*() const;

        ///
        /// Dereferences the handle to a pointer to the entity.
        ///
        /// \returns A pointer to the entity.
        ///
        /// \throws InvalidOperation If the handle is invalid.
        Entity* operator->();

        ///
        /// Dereferences the handle to a pointer to the entity.
        ///
        /// \returns A pointer to the entity.
        ///
        /// \throws InvalidOperation If the handle is invalid.
        const Entity* operator->() const;

        ///
        /// Sets the handle as a copy of another.
        ///
        /// \param handle The handle to copy.
        Handle& operator=(const Handle& handle);

        ///
        /// Sets the handle as being moved from another.
        ///
        /// \param handle The handle to move.
        Handle& operator=(Handle&& handle);

        ///
        /// Returns whether the handle is equivalent to another.
        ///
        /// \param other The other iterator.
        bool operator==(const Handle& other) const;

        ///
        /// Returns whether the handle is different from another.
        ///
        /// \param other The other iterator.
        bool operator!=(const Handle& other) const;

        ///
        /// Returns whether the handle is valid.
        operator bool() const;

    private:
        Handle(EntityPool& pool, EntityId id);

        bool isValid() const;
        void ensureValid() const;

        class Context :
            public Listener<EntityEvent>
        {
        public:
            Context(EntityPool& pool, EntityId id);
            ~Context();

            void receiveEvent(const EntityEvent& event) override;

            EntityPool* pool;
            EntityId id;
            bool valid { true };
        };

        std::shared_ptr<Context> _context;
    };

    ///
    /// Adds a new Component of a specific type to the Entity.
    ///
    /// \note If the entity is activated then this call will immediately
    /// trigger a ::ComponentEventType_Add event.
    ///
    /// \param args The arguments to pass to the component's constructor.
    ///
    /// \returns An iterator to the added component.
    ///
    /// \throws InvalidOperation If the entity already has a component of the
    /// type or if the entity is invalid.
    template <typename T, typename... Args>
    typename Component<T>::Iterator addComponent(Args&&... args);

    ///
    /// Replaces an existing Component that the Entity has.
    ///
    /// \note If the entity is activated then this call will immediately
    /// trigger a ::ComponentEventType_Add event and a
    /// ::ComponentEventType_Remove event.
    ///
    /// \param args The arguments to pass to the component's constructor.
    ///
    /// \returns An iterator to the added component.
    ///
    /// \throws InvalidOperation If the entity does not have a component of the
    /// type or if the entity is invalid.
    template <typename T, typename... Args>
    typename Component<T>::Iterator replaceComponent(Args&&... args);

    ///
    /// Removes the Component of a specific type from the Entity.
    ///
    /// \note If the entity is activated then this call will immediately
    /// trigger a ::ComponentEventType_Remove event.
    ///
    /// \throws InvalidOperation If the entity does not have a component of the
    /// type or if the entity is invalid.
    template <typename T>
    void removeComponent();

    ///
    /// Returns an iterator to the Component of a specific type for the Entity.
    ///
    /// \note The returned iterator is invalid if the entity does not have a
    /// component of the type.
    ///
    /// \throws InvalidOperation If the entity is invalid.
    template <typename T>
    typename Component<T>::Iterator component();

    ///
    /// Returns an iterator to the Component of a specific type for the Entity.
    ///
    /// \note The returned iterator is invalid if the entity does not have a
    /// component of the type.
    ///
    /// \throws InvalidOperation If the entity is invalid.
    template <typename T>
    typename Component<T>::ConstIterator component() const;

    ///
    /// Creates a handle to the entity.
    Entity::Handle createHandle() const;

    ///
    /// Creates an entity iterator for the entity.
    Entity::Iterator iterator();

    ///
    /// Creates a constant entity iterator for the entity.
    Entity::ConstIterator iterator() const;

    ///
    /// Clones the entity.
    ///
    /// \returns An iterator to the cloned entity.
    ///
    /// \throws InvalidOperation If the entity is invalid.
    Entity::Iterator clone() const;

    ///
    /// Marks the entity and all of its children to be destroyed during the
    /// next Scene::refresh().
    ///
    /// \throws InvalidOperation If the entity is invalid or is already
    /// pending destruction.
    void destroy();

    ///
    /// Marks the entity and all of its children to be activated during the
    /// next Scene::refresh().
    ///
    /// \throws InvalidOperation If the entity is invalid, is already pending
    /// activation, or is already activated.
    void activate();

    ///
    /// Returns whether the entity is activated.
    bool isActivated() const;

    ///
    /// Returns whether the entity is pending activation.
    bool isPendingActivation() const;

    ///
    /// Returns whether the entity is pending destruction.
    bool isPendingDestruction() const;

    ///
    /// Returns the id of the entity.
    EntityId id() const;

    ///
    /// Returns an iterator to the parent entity (invalid iterator if the
    /// entity does not have a parent).
    Entity::Iterator parent();

    ///
    /// Returns an iterator to the parent entity (invalid iterator if the
    /// entity does not have a parent).
    Entity::ConstIterator parent() const;

    ///
    /// Adds an entity as a child of the entity.
    ///
    /// \param entity The entity to add as a child.
    ///
    /// \throws InvalidOperation If the entity is invalid, in another scene,
    /// in a differing activation state, is pending activation/destruction,
    /// or is a child of another entity.
    void addChild(Entity& entity);

    ///
    /// Removes a child entity from the entity.
    ///
    /// \param entity The child entity to remove.
    ///
    /// \throws InvalidOperation If the entity is invalid, in another scene, or
    /// is not a child of the entity.
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
    Entity::Iterator findFirstChild(Predicate predicate);

    ///
    /// Returns an iterator to the first child entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::ConstIterator findFirstChild(Predicate predicate) const;

    ///
    /// Returns an iterator to the first descendant entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::Iterator findFirstDescendant(Predicate predicate);

    ///
    /// Returns an iterator to the first descendant entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::ConstIterator findFirstDescendant(Predicate predicate) const;

    ///
    /// Returns an iterator to the first ancestor entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::Iterator findFirstAncestor(Predicate predicate);

    ///
    /// Returns an iterator to the first ancestor entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    Entity::ConstIterator findFirstAncestor(Predicate predicate) const;

    ///
    /// Returns iterators to all child entities matching the given predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns A vector of iterators to the matching entities.
    Entity::Iterator::Vector findChildren(Predicate predicate);

    ///
    /// Returns iterators to all child entities matching the given predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns A vector of iterators to the matching entities.
    Entity::ConstIterator::Vector findChildren(Predicate predicate) const;

    ///
    /// Returns iterators to all descendant entities matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns A vector of iterators to the matching entities.
    Entity::Iterator::Vector findDescendants(Predicate predicate);

    ///
    /// Returns iterators to all descendant entities matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns A vector of iterators to the matching entities.
    Entity::ConstIterator::Vector findDescendants(Predicate predicate) const;

    ///
    /// Returns iterators to all ancestor entities matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns A vector of iterators to the matching entities.
    Entity::Iterator::Vector findAncestors(Predicate predicate);

    ///
    /// Returns iterators to all ancestor entities matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search.
    ///
    /// \returns A vector of iterators to the matching entities.
    Entity::ConstIterator::Vector findAncestors(Predicate predicate) const;

    Entity& operator=(const Entity& entity);
    Entity& operator=(Entity&& entity);

    friend HECT_EXPORT Encoder& operator<<(Encoder& encoder, const Entity& entity);
    friend HECT_EXPORT Decoder& operator>>(Decoder& decoder, Entity& entity);

private:
    Entity();
    Entity(const Entity& entity);
    Entity(Entity&& entity);

    void enterPool(EntityPool& pool, EntityId id);
    void exitPool();
    bool inPool() const;
    void ensureInPool() const;

    void encode(Encoder& encoder) const;
    void decode(Decoder& decoder);

    Children _children;
    EntityPool* _pool { nullptr };
    EntityId _id { EntityId(-1) };
    EntityId _parentId { EntityId(-1) };
    std::vector<EntityId> _childIds;
    bool _activated { false };
    bool _pendingActivation { false };
    bool _pendingDestruction { false };
};

}
