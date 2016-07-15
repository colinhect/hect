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
#pragma once

#include <bitset>
#include <functional>

#include "Hect/Core/Export.h"
#include "Hect/IO/Encodable.h"
#include "Hect/Scene/Component.h"
#include "Hect/Scene/EntityChildren.h"
#include "Hect/Scene/EntityEvent.h"
#include "Hect/Scene/EntityHandle.h"
#include "Hect/Scene/EntityIterator.h"

namespace hect
{

class Scene;
class EntityPool;

///
/// A dynamic object in a Scene.
///
/// \warning Raw references and pointers to an entity may be invalidated as new
/// entities are created and destroyed in the entity pool.  To maintain a valid
/// reference to an entity, use Entity::Iterator or Entity::Handle.
class HECT_EXPORT Entity :
    public Uncopyable,
    public Encodable
{
    friend class Scene;
    friend class EntityChildren;
    friend class EntityChildIteratorBase;
    friend class EntityPool;
public:

    ///
    /// \copydoc hect::EntityIterator
    typedef EntityIterator Iterator;

    ///
    /// \copydoc hect::EntityConstIterator
    typedef EntityConstIterator ConstIterator;

    ///
    /// \copydoc hect::EntityChildren
    typedef EntityChildren Children;

    ///
    /// \copydoc hect::EntityHandle
    typedef EntityHandle Handle;

    ///
    /// Adds a new Component of a specific type to the Entity.
    ///
    /// \note If the entity is activated then this call will immediately
    /// trigger a ::ComponentEventType::Add event.
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
    /// trigger a ::ComponentEventType::Add event and a
    /// ::ComponentEventType::Remove event.
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
    /// trigger a ::ComponentEventType::Remove event.
    ///
    /// \throws InvalidOperation If the entity does not have a component of the
    /// type or if the entity is invalid.
    template <typename T>
    void removeComponent();

    ///
    /// Returns whether the Entity has a Component of a specific type.
    template <typename T>
    bool hasComponent() const;

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
    /// Returns the name.
    Name name() const;

    ///
    /// Sets the name.
    ///
    /// \param name The new name.
    void setName(Name name);

    ///
    /// Returns the Scene that the Entity is in.
    Scene& scene();

    ///
    /// \copydoc hect::Entity::scene()
    const Scene& scene() const;

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
    /// Marks the entity and all of its children to be destroyed.
    ///
    /// \throws InvalidOperation If the entity is invalid or is already
    /// pending destruction.
    void destroy();

    ///
    /// Marks the entity and all of its children to be activated.
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
    /// Returns whether the entity is transient.
    bool isTransient() const;

    ///
    /// Sets whether the entity is transient.
    ///
    /// \param transient True if the entity is transient; false otherwise.
    void setTransient(bool transient);

    ///
    /// Returns the id of the entity.
    EntityId id() const;

    ///
    /// Returns an iterator to the parent entity (invalid iterator if the
    /// entity does not have a parent).
    Entity::Iterator parent();

    ///
    /// \copydoc hect::Entity::parent()
    Entity::ConstIterator parent() const;

    ///
    /// Returns an iterator to the root entity.
    Entity::Iterator root();

    ///
    /// \copydoc hect::Entity::root()
    Entity::ConstIterator root() const;

    ///
    /// Adds an entity as a child of the entity.
    ///
    /// \param entity The entity to add as a child.
    ///
    /// \throws InvalidOperation If the entity is invalid, in another scene,
    /// in a differing activation state, is pending destruction, or is a child
    /// of another entity.
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
    /// Destroys all child entities from the entity.
    void destroyAllChildren();

    ///
    /// Returns the children of the entity.
    Entity::Children& children();

    ///
    /// Returns the children of the entity.
    const Entity::Children& children() const;

    ///
    /// Returns whether the entity has any children.
    bool hasChildren() const;

    ///
    /// Returns an iterator to the first child entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search; must be callable
    /// as a function accepting a reference to an Entity, returning whether
    /// the entity matches the predicate.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    template <typename T>
    Entity::Iterator findFirstChild(T&& predicate);

    ///
    /// \copydoc Entity::findFirstChild()
    template <typename T>
    Entity::ConstIterator findFirstChild(T&& predicate) const;

    ///
    /// Returns an iterator to the first descendant entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search; must be callable
    /// as a function accepting a reference to an Entity, returning whether
    /// the entity matches the predicate.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    template <typename T>
    Entity::Iterator findFirstDescendant(T&& predicate);

    ///
    /// \copydoc Entity::findFirstDescendant()
    template <typename T>
    Entity::ConstIterator findFirstDescendant(T&& predicate) const;

    ///
    /// Returns an iterator to the first ancestor entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search; must be callable
    /// as a function accepting a reference to an Entity, returning whether
    /// the entity matches the predicate.
    ///
    /// \returns An iterator to the first matching entity; invalid if there
    /// was no matching entity.
    template <typename T>
    Entity::Iterator findFirstAncestor(T&& predicate);

    ///
    /// \copydoc Entity::findFirstAncestor()
    template <typename T>
    Entity::ConstIterator findFirstAncestor(T&& predicate) const;

    ///
    /// Returns iterators to all child entities matching the given predicate.
    ///
    /// \param predicate The predicate to use in the search; must be callable
    /// as a function accepting a reference to an Entity, returning whether
    /// the entity matches the predicate.
    ///
    /// \returns A vector of iterators to the matching entities.
    template <typename T>
    std::vector<Entity::Iterator> findChildren(T&& predicate);

    ///
    /// \copydoc Entity::findChildren()
    template <typename T>
    std::vector<Entity::ConstIterator> findChildren(T&& predicate) const;

    ///
    /// Returns iterators to all descendant entities matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search; must be callable
    /// as a function accepting a reference to an Entity, returning whether
    /// the entity matches the predicate.
    ///
    /// \returns A vector of iterators to the matching entities.
    template <typename T>
    std::vector<Entity::Iterator> findDescendants(T&& predicate);

    ///
    /// \copydoc Entity::findDescendants()
    template <typename T>
    std::vector<Entity::ConstIterator> findDescendants(T&& predicate) const;

    ///
    /// Returns iterators to all ancestor entities matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search; must be callable
    /// as a function accepting a reference to an Entity, returning whether
    /// the entity matches the predicate.
    ///
    /// \returns A vector of iterators to the matching entities.
    template <typename T>
    std::vector<Entity::Iterator> findAncestors(T&& predicate);

    ///
    /// \copydoc Entity::findAncestors()
    template <typename T>
    std::vector<Entity::ConstIterator> findAncestors(T&& predicate) const;

    ///
    /// Iterates over all child entities.
    ///
    /// \param action The action to perform on each entity; must be callable
    /// as a function accepting a reference to an Entity.
    template <typename T>
    void forChildren(T&& action);

    ///
    /// \copydoc Entity::forChildren()
    template <typename T>
    void forChildren(T&& action) const;

    ///
    /// Iterates over all descendant entities.
    ///
    /// \param action The action to perform on each entity; must be callable
    /// as a function accepting a reference to an Entity.
    template <typename T>
    void forDescendants(T&& action);

    ///
    /// \copydoc Entity::forDescendants()
    template <typename T>
    void forDescendants(T&& action) const;

    ///
    /// Iterates over all ancestor entities.
    ///
    /// \param action The action to perform on each entity; must be callable
    /// as a function accepting a reference to an Entity.
    template <typename T>
    void forAncestors(T&& action);

    ///
    /// \copydoc Entity::forAncestors()
    template <typename T>
    void forAncestors(T&& action) const;

    Entity& operator=(const Entity& entity);
    Entity& operator=(Entity&& entity);

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    Entity();
    Entity(const Entity& entity);
    Entity(Entity&& entity);

    enum class Flag
    {
        Transient,
        Activated,
        PendingActivation,
        PendingDestruction
    };

    void enterPool(EntityPool& pool, EntityId id);
    void exitPool();
    bool inPool() const;
    void ensureInPool() const;
    void setFlag(Flag flag, bool value);
    bool flag(Flag flag) const;

    EntityPool* _pool { nullptr };
    EntityId _id { EntityId(-1) };
    EntityId _parentId { EntityId(-1) };
    std::vector<EntityId> _childIds;
    Name _name;
    std::bitset<4> _flags;
};

}
