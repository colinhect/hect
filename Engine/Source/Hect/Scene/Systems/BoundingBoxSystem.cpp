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
#include "Hect/Scene/Components/MeshComponent.h"
#include "Hect/Scene/Components/TransformComponent.h"

using namespace hect;

BoundingBoxSystem::BoundingBoxSystem(Engine& engine, Scene& scene) :
    System(engine, scene)
{
}

void BoundingBoxSystem::updateBoundingBox(BoundingBoxComponent& boundingBox)
{
    Entity::Iterator root = boundingBox.entity()->root();
    updateRecursively(*root);
}

void BoundingBoxSystem::renderDebugGeometry()
{
    // Render a debug box for each bounding box
    for (const BoundingBoxComponent& boundingBox : scene().components<BoundingBoxComponent>())
    {
        AxisAlignedBox axisAlignedBox = boundingBox.extents;
        Box box(axisAlignedBox.maximum() - axisAlignedBox.minimum());

        if (scene().hasSystemType<DebugSystem>())
        {
            auto& debugSystem = scene().system<DebugSystem>();
            debugSystem.renderBox(Color::Green, box, axisAlignedBox.center());
        }
    }
}

void BoundingBoxSystem::updateRecursively(Entity& entity)
{
    // Compute the bounding box of this entity
    BoundingBoxComponent::Iterator boundingBox = entity.component<BoundingBoxComponent>();
    if (boundingBox && boundingBox->adaptive)
    {
        // Start with an empty box
        AxisAlignedBox& axisAlignedBox = boundingBox->extents;
        axisAlignedBox = AxisAlignedBox();

        // Expand to fit all meshes that the component has
        MeshComponent::Iterator mesh = entity.component<MeshComponent>();
        if (mesh)
        {
            for (const MeshSurface& surface : mesh->surfaces)
            {
                Mesh& mesh = *surface.mesh;
                axisAlignedBox.expandToInclude(mesh.axisAlignedBox());
            }
        }

        // Transform the bounding box by the entity's global transform
        TransformComponent::Iterator transform = entity.component<TransformComponent>();
        if (transform)
        {
            axisAlignedBox.scale(transform->globalScale);
            axisAlignedBox.rotate(transform->globalRotation);
            axisAlignedBox.translate(transform->globalPosition);
        }
    }

    // Recursively compute the bounding boxes of all children
    for (Entity& child : entity.children())
    {
        updateRecursively(child);

        // If the child has a bounding box
        BoundingBoxComponent::Iterator childBoundingBox = child.component<BoundingBoxComponent>();
        if (childBoundingBox)
        {
            // Expand the bounding box to include this child
            if (boundingBox)
            {
                boundingBox->extents.expandToInclude(childBoundingBox->extents);
            }
        }
    }
}

void BoundingBoxSystem::onComponentAdded(BoundingBoxComponent::Iterator boundingBox)
{
    Entity::Iterator entity = boundingBox->entity();

    // Update the extents of the bounding box if it is adaptive
    if (boundingBox && boundingBox->adaptive)
    {
        // Update the extent of the bounding box
        updateRecursively(*entity);
    }
}
