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
    /// Adds a new Component of a specific type to the Entity.
    ///
    /// \note If the entity is activated then this call will immediately
    /// trigger a ::ComponentEventType::Add event.
    ///
    /// \param args The arguments to pass to the component's constructor.
    ///
    /// \returns A reference to the added component.
    ///
    /// \throws InvalidOperation If the entity already has a component of the
    /// type or if the entity is invalid.
    template <typename T, typename... Args>
    T& add_component(Args&&... args);

    ///
    /// Replaces an existing Component that the Entity has.
    ///
    /// \note If the entity is activated then this call will immediately
    /// trigger a ::ComponentEventType::Add event and a
    /// ::ComponentEventType::Remove event.
    ///
    /// \param args The arguments to pass to the component's constructor.
    ///
    /// \returns A reference to the added component.
    ///
    /// \throws InvalidOperation If the entity does not have a component of the
    /// type or if the entity is invalid.
    template <typename T, typename... Args>
    T& replace_component(Args&&... args);

    ///
    /// Removes the Component of a specific type from the Entity.
    ///
    /// \note If the entity is activated then this call will immediately
    /// trigger a ::ComponentEventType::Remove event.
    ///
    /// \throws InvalidOperation If the entity does not have a component of the
    /// type or if the entity is invalid.
    template <typename T>
    void remove_component();

    ///
    /// Returns whether the Entity has a Component of a specific type.
    template <typename T>
    bool has_component() const;

    ///
    /// Returns a reference to the Component of a specific type for the Entity.
    ///
    /// \throws InvalidOperation If the entity does not have a component of the
    /// type or the entity is invalid.
    template <typename T>
    T& component();

    ///
    /// Returns a reference to the Component of a specific type for the Entity.
    ///
    /// \throws InvalidOperation If the entity does not have a component of the
    /// type or the entity is invalid.
    template <typename T>
    const T& component() const;

    ///
    /// Returns the name.
    Name name() const;

    ///
    /// Sets the name.
    ///
    /// \param name The new name.
    void set_name(Name name);

    ///
    /// Returns the Scene that the Entity is in.
    Scene& scene();

    ///
    /// \copydoc hect::Entity::scene()
    const Scene& scene() const;

    ///
    /// Returns a handle to the entity.
    EntityHandle handle() const;

    ///
    /// Returns an entity iterator for the entity.
    EntityIterator iterator();

    ///
    /// Returns a constant entity iterator for the entity.
    EntityConstIterator iterator() const;

    ///
    /// Clones the entity.
    ///
    /// \returns A reference to the cloned entity.
    ///
    /// \throws InvalidOperation If the entity is invalid.
    Entity& clone() const;

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
    bool is_activated() const;

    ///
    /// Returns whether the entity is pending activation.
    bool is_pending_activation() const;

    ///
    /// Returns whether the entity is pending destruction.
    bool is_pending_destruction() const;

    ///
    /// Returns whether the entity is transient.
    bool is_transient() const;

    ///
    /// Sets whether the entity is transient.
    ///
    /// \param transient True if the entity is transient; false otherwise.
    void set_transient(bool transient);

    ///
    /// Returns the id of the entity.
    EntityId id() const;

    ///
    /// Returns a handle to the parent entity (invalid iterator if the
    /// entity does not have a parent).
    EntityHandle parent() const;

    ///
    /// Returns a handle to the root entity.
    EntityHandle root() const;

    ///
    /// Adds an entity as a child of the entity.
    ///
    /// \param entity The entity to add as a child.
    ///
    /// \throws InvalidOperation If the entity is invalid, in another scene,
    /// in a differing activation state, is pending destruction, or is a child
    /// of another entity.
    void add_child(Entity& entity);

    ///
    /// Removes a child entity from the entity.
    ///
    /// \param entity The child entity to remove.
    ///
    /// \throws InvalidOperation If the entity is invalid, in another scene, or
    /// is not a child of the entity.
    void remove_child(Entity& entity);

    ///
    /// Destroys all child entities from the entity.
    void destroy_all_children();

    ///
    /// Returns the children of the entity.
    EntityChildren& children();

    ///
    /// Returns the children of the entity.
    const EntityChildren& children() const;

    ///
    /// Returns whether the entity has any children.
    bool has_children() const;

    ///
    /// Returns a handle to the first child entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search; must be callable
    /// as a function accepting a reference to an Entity, returning whether
    /// the entity matches the predicate.
    ///
    /// \returns A handle to the first matching entity; invalid if there
    /// was no matching entity.
    template <typename T>
    EntityHandle find_first_child(T&& predicate) const;

    ///
    /// Returns a handle to the first descendant entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search; must be callable
    /// as a function accepting a reference to an Entity, returning whether
    /// the entity matches the predicate.
    ///
    /// \returns A handle to the first matching entity; invalid if there
    /// was no matching entity.
    template <typename T>
    EntityHandle find_first_descendant(T&& predicate) const;

    ///
    /// Returns a handle to the first ancestor entity matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search; must be callable
    /// as a function accepting a reference to an Entity, returning whether
    /// the entity matches the predicate.
    ///
    /// \returns A handle to the first matching entity; invalid if there
    /// was no matching entity.
    template <typename T>
    EntityHandle find_first_ancestor(T&& predicate) const;

    ///
    /// Returns iterators to all child entities matching the given predicate.
    ///
    /// \param predicate The predicate to use in the search; must be callable
    /// as a function accepting a reference to an Entity, returning whether
    /// the entity matches the predicate.
    ///
    /// \returns A vector of handles to the matching entities.
    template <typename T>
    std::vector<EntityHandle> find_children(T&& predicate) const;

    ///
    /// Returns iterators to all descendant entities matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search; must be callable
    /// as a function accepting a reference to an Entity, returning whether
    /// the entity matches the predicate.
    ///
    /// \returns A vector of handles to the matching entities.
    template <typename T>
    std::vector<EntityHandle> find_descendants(T&& predicate) const;

    ///
    /// Returns iterators to all ancestor entities matching the given
    /// predicate.
    ///
    /// \param predicate The predicate to use in the search; must be callable
    /// as a function accepting a reference to an Entity, returning whether
    /// the entity matches the predicate.
    ///
    /// \returns A vector of handles to the matching entities.
    template <typename T>
    std::vector<EntityHandle> find_ancestors(T&& predicate) const;

    ///
    /// Iterates over all child entities.
    ///
    /// \param action The action to perform on each entity; must be callable
    /// as a function accepting a reference to an Entity.
    template <typename T>
    void for_children(T&& action);

    ///
    /// \copydoc Entity::for_children()
    template <typename T>
    void for_children(T&& action) const;

    ///
    /// Iterates over all descendant entities.
    ///
    /// \param action The action to perform on each entity; must be callable
    /// as a function accepting a reference to an Entity.
    template <typename T>
    void for_descendants(T&& action);

    ///
    /// \copydoc Entity::for_descendants()
    template <typename T>
    void for_descendants(T&& action) const;

    ///
    /// Iterates over all ancestor entities.
    ///
    /// \param action The action to perform on each entity; must be callable
    /// as a function accepting a reference to an Entity.
    template <typename T>
    void for_ancestors(T&& action);

    ///
    /// \copydoc Entity::for_ancestors()
    template <typename T>
    void for_ancestors(T&& action) const;

    ///
    /// Returns whether the entity is valid.
    operator bool() const;

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    Entity();
    Entity(const Entity& entity);
    Entity(Entity&& entity);

    Entity& operator=(const Entity& entity);
    Entity& operator=(Entity&& entity);

    enum class Flag
    {
        Transient,
        Activated,
        PendingActivation,
        PendingDestruction
    };

    void enter_pool(EntityPool& pool, EntityId id);
    void exit_pool();
    bool in_pool() const;
    void ensure_in_pool() const;
    void set_flag(Flag flag, bool value);
    bool flag(Flag flag) const;

    EntityPool* _pool { nullptr };
    EntityId _id { EntityId(-1) };
    EntityId _parent_id { EntityId(-1) };
    std::vector<EntityId> _child_ids;
    Name _name;
    std::bitset<4> _flags;
    mutable EntityHandle _handle;
};

}
