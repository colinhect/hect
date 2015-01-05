///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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

#include "Hect/Logic/Systems/BoundingBoxSystem.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

TransformSystem::TransformSystem(Engine& engine, Scene& scene) :
    System(scene, SystemTickStage_Subsequent)
{
    (void)engine;
}

void TransformSystem::commit(Transform& transform)
{
    ComponentId id = transform.id();
    _committed.push_back(id);
}

void TransformSystem::update(Transform& transform)
{
    Entity& entity = transform.entity();
    auto parent = entity.parent();
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
    BoundingBoxSystem& boundingBoxSystem = scene().system<BoundingBoxSystem>();
    boundingBoxSystem.updateRecursively(entity);
}

void TransformSystem::tick(Real timeStep)
{
    (void)timeStep;

    // Update all committed transforms
    for (ComponentId id : _committed)
    {
        Transform& transform = scene().components<Transform>().withId(id);
        update(transform);
    }
    _committed.clear();
}

void TransformSystem::updateRecursively(Entity& parent, Entity& child)
{
    // Get the parent transform
    auto parentTransform = parent.component<Transform>();
    if (parentTransform)
    {
        // Get the child transform
        auto childTransform = child.component<Transform>();
        if (childTransform)
        {
            // Compute global components of the transform
            childTransform->globalPosition = parentTransform->globalRotation * childTransform->localPosition;
            childTransform->globalPosition += parentTransform->globalPosition;
            childTransform->globalScale = parentTransform->globalScale * childTransform->localScale;
            childTransform->globalRotation = parentTransform->globalRotation * childTransform->localRotation;

            // Recursively update for all children
            for (Entity& nextChild : child.children())
            {
                updateRecursively(child, nextChild);
            }
        }
    }
}
