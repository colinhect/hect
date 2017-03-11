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
#include "BoundingBoxSystem.h"

#include "Hect/Runtime/Engine.h"
#include "Hect/Scene/Components/GeometryComponent.h"
#include "Hect/Scene/Components/TransformComponent.h"

using namespace hect;

BoundingBoxSystem::BoundingBoxSystem(Scene& scene) :
    System(scene)
{
}

BoundingBoxSystem::BoundingBoxSystem(Scene& scene, DebugSystem& debugSystem) :
    System(scene),
    _debugSystem(&debugSystem)
{
}

void BoundingBoxSystem::updateBoundingBox(BoundingBoxComponent& boundingBox)
{
    Entity::Handle root = boundingBox.entity().root();
    updateRecursively(*root);
}

void BoundingBoxSystem::renderDebugGeometry()
{
    if (_debugSystem)
    {
        // Render a debug box for each bounding box
        for (const BoundingBoxComponent& boundingBox : scene().components<BoundingBoxComponent>())
        {
            AxisAlignedBox axisAlignedBox = boundingBox.globalExtents;
            Box box(axisAlignedBox.maximum() - axisAlignedBox.minimum());

            _debugSystem->renderBox(Color::Green, box, axisAlignedBox.center());
        }
    }
}

void BoundingBoxSystem::updateRecursively(Entity& entity)
{
    // Compute the bounding box of this entity
    auto boundingBox = entity.component<BoundingBoxComponent>();
    if (boundingBox)
    {
        // Update the local extents if the bounding box is adaptive
        if (boundingBox->adaptive)
        {
            // Start with an empty box
            AxisAlignedBox& localExtents = boundingBox->localExtents;
            localExtents = AxisAlignedBox();

            // Expand to fit all meshes that the component has
            auto geometry = entity.component<GeometryComponent>();
            if (geometry)
            {
                for (const GeometrySurface& surface : geometry->surfaces)
                {
                    Mesh& mesh = *surface.mesh;
                    localExtents.expandToInclude(mesh.axisAlignedBox());
                }
            }
        }

        // Sync the global extents with the local extents
        boundingBox->globalExtents = boundingBox->localExtents;

        // Transform the global extents of the bounding box by the entity's
        // global transform
        auto transform = entity.component<TransformComponent>();
        if (transform)
        {
            AxisAlignedBox& globalExtents = boundingBox->globalExtents;
            globalExtents.scale(transform->globalScale);
            globalExtents.rotate(transform->globalRotation);
            globalExtents.translate(transform->globalPosition);
        }
    }

    // Recursively compute the bounding boxes of all children
    for (Entity& child : entity.children())
    {
        updateRecursively(child);

        // If the child has a bounding box
        auto childBoundingBox = child.component<BoundingBoxComponent>();
        if (childBoundingBox)
        {
            // Expand the bounding box to include this child
            if (boundingBox)
            {
                boundingBox->globalExtents.expandToInclude(childBoundingBox->globalExtents);
            }
        }
    }
}

void BoundingBoxSystem::onComponentAdded(BoundingBoxComponent::Iterator boundingBox)
{
    Entity& entity = boundingBox->entity();

    // Update the extents of the bounding box if it is adaptive
    if (boundingBox && boundingBox->adaptive)
    {
        // Update the extent of the bounding box
        updateRecursively(entity);
    }
}
