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

TransformSystem::TransformSystem(Scene& scene, BoundingBoxSystem& boundingBoxSystem) :
    System(scene),
    _boundingBoxSystem(boundingBoxSystem)
{
}

void TransformSystem::commitTransform(TransformComponent& transform)
{
    if (transform.mobility != Mobility::Dynamic)
    {
        throw InvalidOperation("TransformComponent is not dynamic");
    }

    const ComponentId id = transform.id();
    _committedTransformIds.push_back(id);
}

void TransformSystem::updateTransform(TransformComponent& transform)
{
    if (transform.mobility != Mobility::Dynamic)
    {
        throw InvalidOperation("TransformComponent is not dynamic");
    }

    Entity& entity = transform.entity();
    Entity::Handle parent = entity.parent();
    if (parent)
    {
        // Update the transform hierarchy starting at this entity's parent
        updateRecursively(*parent, entity);
    }
    else
    {
        // Local and global are the same if there is no parent
        transform.globalPosition = transform.localPosition;
        transform.globalScale = transform.localScale;
        transform.globalRotation = transform.localRotation;

        // Update the transform hierachy for all children
        for (Entity& child : entity.children())
        {
            updateRecursively(entity, child);
        }
    }

    // Force the bounding box to update
    _boundingBoxSystem.updateRecursively(entity);
}

void TransformSystem::updateCommittedTransforms()
{
    // Update all committed transforms
    ComponentPool<TransformComponent>& transformComponents = scene().components<TransformComponent>();
    for (ComponentId id : _committedTransformIds)
    {
        TransformComponent& transform = transformComponents.withId(id);
        updateTransform(transform);
    }
    _committedTransformIds.clear();
}

void TransformSystem::updateRecursively(Entity& parent, Entity& child)
{
    if (child.hasComponent<TransformComponent>())
    {
        auto& childTransform = child.component<TransformComponent>();

        if (parent.hasComponent<TransformComponent>())
        {
            auto& parentTransform = parent.component<TransformComponent>();

            // Compute global components of the transform
            childTransform.globalPosition = parentTransform.globalRotation * childTransform.localPosition;
            childTransform.globalPosition += parentTransform.globalPosition;
            childTransform.globalScale = parentTransform.globalScale * childTransform.localScale;
            childTransform.globalRotation = parentTransform.globalRotation * childTransform.localRotation;
        }
        else
        {
            // Local and global are the same if there is no parent
            childTransform.globalPosition = childTransform.localPosition;
            childTransform.globalScale = childTransform.localScale;
            childTransform.globalRotation = childTransform.localRotation;
        }

        // Recursively update for all children
        for (Entity& nextChild : child.children())
        {
            updateRecursively(child, nextChild);
        }
    }
}

void TransformSystem::onComponentAdded(TransformComponent& transform)
{
    // Temporarily make the transform dynamic so it can be initially updated
    const Mobility mobility = transform.mobility;
    transform.mobility = Mobility::Dynamic;

    // Update the transform
    updateTransform(transform);

    // Restore the transforms mobility
    transform.mobility = mobility;
}

void TransformSystem::onComponentRemoved(TransformComponent& transform)
{
    // Remove the transform from the committed transform vector
    _committedTransformIds.erase(std::remove(_committedTransformIds.begin(), _committedTransformIds.end(), transform.id()), _committedTransformIds.end());
}
