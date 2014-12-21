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
#include "TransformSystem.h"

#include "Hect/Logic/Components/BoundingBox.h"
#include "Hect/Logic/Systems/BoundingBoxSystem.h"

using namespace hect;

TransformSystem::TransformSystem(Scene& scene) :
    System(scene)
{
}

void TransformSystem::update(Transform& transform)
{
    Entity& entity = transform.entity();
    auto parent = entity.parent();
    if (parent)
    {
        // Update the transform hierarchy starting at this entity's parent
        updateHeierarchy(*parent, entity);
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
            updateHeierarchy(entity, child);
        }
    }

    // Find the root entity
    auto root = entity.findFirstAncestor([](const Entity& entity)
    {
        return !entity.parent();
    });

    // If no root was found then this entity is the root
    if (!root)
    {
        root = entity.iterator();
    }

    // Update the bounding box from the root
    auto boundingBox = root->component<BoundingBox>();
    if (boundingBox)
    {
        BoundingBoxSystem& boundingBoxSystem = scene().system<BoundingBoxSystem>();
        boundingBoxSystem.update(*boundingBox);
    }
}

void TransformSystem::updateHeierarchy(Entity& parent, Entity& child)
{
    auto parentTransform = parent.component<Transform>();
    if (parentTransform)
    {
        auto childTransform = child.component<Transform>();
        if (childTransform)
        {
            childTransform->globalPosition = parentTransform->globalRotation * childTransform->localPosition;
            childTransform->globalPosition += parentTransform->globalPosition;
            childTransform->globalScale = parentTransform->globalScale * childTransform->localScale;
            childTransform->globalRotation = parentTransform->globalRotation * childTransform->localRotation;

            for (Entity& nextChild : child.children())
            {
                updateHeierarchy(child, nextChild);
            }
        }
    }
}
