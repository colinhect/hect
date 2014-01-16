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

///
/// A game object in a scene.
///
/// \remarks An entity value is a lightweight handle for an entity within a
/// scene.  Copying an entity value will result in another handle pointing
/// to the same entity.
class Entity
{
    friend class Scene;
public:

    ///
    /// The id of an entity within a scene.
    typedef uint32_t Id;

    ///
    /// Constructs a null entity.
    Entity();

    ///
    /// Returns the scene that the entity belongs to.
    ///
    /// \throws Error If the entity is null.
    Scene& scene() const;

    ///
    /// Returns the entity id.
    Id id() const;

    ///
    /// Serializes the entity's components to a data value.
    ///
    /// \param dataValue The data value.
    ///
    /// \throws Error If the entity is null.
    void save(DataValue& dataValue) const;

    ///
    /// Serializes the entity's components to a binary stream.
    ///
    /// \param stream The stream to write to.
    ///
    /// \throws Error If the entity is null.
    void save(WriteStream& stream) const;

    ///
    /// Deserializes and adds components to the entity from a data value.
    ///
    /// \param dataValue The data value.
    /// \param assetCache The asset cache to use to load referenced assets.
    ///
    /// \throws Error If the entity is null or activated.
    void load(const DataValue& dataValue, AssetCache& assetCache) const;

    ///
    /// Deserializes and adds components to the entity from a binary stream.
    ///
    /// \param stream The stream to read from.
    /// \param assetCache The asset cache to use to load referenced assets.
    ///
    /// \throws Error If the entity is null or activated.
    void load(ReadStream& stream, AssetCache& assetCache) const;

    ///
    /// Activates the entity, enqueuing it to be added to systems in the scene
    /// which include the entity on the next call to Scene::refresh().
    ///
    /// \remarks Once the entity is activated, components can no longer be
    /// added or removed.  The entity cannot be deactivated.
    ///
    /// \throws Error If the entity is already activated or is null.
    void activate() const;

    ///
    /// Destroys the entity, enqueuing it to be removed from systems in the
    /// scene which include the entity on the next call to Scene::refresh().
    ///
    /// \remarks The entity cannot be re-created.
    ///
    /// \throws Error If the entity is already destroyed or is null.
    void destroy() const;

    ///
    /// Creates a clone of the entity.
    ///
    /// \remarks Each component of the entity will be copied and added to the
    /// cloned entity.
    Entity clone() const;

    ///
    /// Returns whether the entity is activated.
    bool isActivated() const;

    ///
    /// Returns whether the entity is null.
    bool isNull() const;

    ///
    /// Returns whether the entity should be serialized when the scene it is
    /// in is serialized.
    bool isSerializable() const;

    ///
    /// Sets whether the entity should be serialized when the scene it is in is
    /// serialized.
    ///
    /// \param serializable The new serializable value.
    void setSerializable(bool serializable) const;

    ///
    /// Returns whether an entity has a component of a certain type.
    ///
    /// \warning Always call this before component().
    ///
    /// \throws Error If the entity is null.
    template <typename T>
    bool hasComponent() const;

    ///
    /// Adds a new component of a certain type to the entity.
    ///
    /// \returns The new component.
    ///
    /// \throws Error If the entity is activated or already has a component of
    /// the type.
    template <typename T>
    T& addComponent() const;

    ///
    /// Adds a new component to the entity.
    ///
    /// \param component The new component (the entity will take ownership of
    /// the components lifetime).
    ///
    /// \throws Error If the entity is activated or already has a component of
    /// the type.
    void addComponent(BaseComponent* component) const;

    ///
    /// Returns the component of a certain type from an entity.
    ///
    /// \warning Always call hasComponent() before this.
    ///
    /// \throws Error If the entity does not have a component of the type.
    template <typename T>
    T& component() const;

    ///
    /// Returns the components of the entity.
    std::vector<BaseComponent*> components() const;

    ///
    /// Returns true if the entity is not null; false otherwise.
    operator bool() const;

    ///
    /// Returns whether an entity is equivalent to another.
    ///
    /// \param entity The other entity.
    bool operator==(const Entity& entity) const;

    ///
    /// Returns whether an entity differs from another.
    ///
    /// \param entity The other entity.
    bool operator!=(const Entity& entity) const;

private:
    Entity(Scene& scene, Id id);

    Scene* _scene;
    Id _id;
};

}

#include "Entity.inl"