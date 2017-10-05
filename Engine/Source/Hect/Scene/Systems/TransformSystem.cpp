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
#include "TransformSystem.h"

#include <algorithm>

#include "Hect/Runtime/Engine.h"

using namespace hect;

TransformSystem::TransformSystem(Scene& scene, BoundingBoxSystem& bounding_box_system) :
    System(scene),
    _bounding_box_system(bounding_box_system)
{
}

void TransformSystem::commit_transform(TransformComponent& transform)
{
    if (transform.mobility != Mobility::Dynamic)
    {
        throw InvalidOperation("TransformComponent is not dynamic");
    }

    const ComponentId id = transform.id();
    _committed_transform_ids.push_back(id);
}

void TransformSystem::update_transform(TransformComponent& transform)
{
    if (transform.mobility != Mobility::Dynamic)
    {
        throw InvalidOperation("TransformComponent is not dynamic");
    }

    Entity& entity = transform.entity();
    EntityHandle parent = entity.parent();
    if (parent)
    {
        // Update the transform hierarchy starting at this entity's parent
        update_recursively(*parent, entity);
    }
    else
    {
        // Local and global are the same if there is no parent
        transform.global_position = transform.local_position;
        transform.global_scale = transform.local_scale;
        transform.global_rotation = transform.local_rotation;

        // Update the transform hierachy for all children
        for (Entity& child : entity.children())
        {
            update_recursively(entity, child);
        }
    }

    // Force the bounding box to update
    _bounding_box_system.update_recursively(entity);
}

void TransformSystem::update_committed_transforms()
{
    // Update all committed transforms
    ComponentPool<TransformComponent>& transform_components = scene().components<TransformComponent>();
    for (ComponentId id : _committed_transform_ids)
    {
        TransformComponent& transform = transform_components.with_id(id);
        update_transform(transform);
    }
    _committed_transform_ids.clear();
}

void TransformSystem::update_recursively(Entity& parent, Entity& child)
{
    if (child.has_component<TransformComponent>())
    {
        auto& child_transform = child.component<TransformComponent>();

        if (parent.has_component<TransformComponent>())
        {
            auto& parent_transform = parent.component<TransformComponent>();

            // Compute global components of the transform
            child_transform.global_position = parent_transform.global_rotation * child_transform.local_position;
            child_transform.global_position += parent_transform.global_position;
            child_transform.global_scale = parent_transform.global_scale * child_transform.local_scale;
            child_transform.global_rotation = parent_transform.global_rotation * child_transform.local_rotation;
        }
        else
        {
            // Local and global are the same if there is no parent
            child_transform.global_position = child_transform.local_position;
            child_transform.global_scale = child_transform.local_scale;
            child_transform.global_rotation = child_transform.local_rotation;
        }

        // Recursively update for all children
        for (Entity& next_child : child.children())
        {
            update_recursively(child, next_child);
        }
    }
}

void TransformSystem::on_component_added(TransformComponent& transform)
{
    // Temporarily make the transform dynamic so it can be initially updated
    const Mobility mobility = transform.mobility;
    transform.mobility = Mobility::Dynamic;

    // Update the transform
    update_transform(transform);

    // Restore the transforms mobility
    transform.mobility = mobility;
}

void TransformSystem::on_component_removed(TransformComponent& transform)
{
    // Remove the transform from the committed transform vector
    _committed_transform_ids.erase(std::remove(_committed_transform_ids.begin(), _committed_transform_ids.end(), transform.id()), _committed_transform_ids.end());
}
