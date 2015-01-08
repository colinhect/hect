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
#include "BoundingBoxSystem.h"

#include "Hect/Logic/Components/Model.h"
#include "Hect/Logic/Components/Transform.h"
#include "Hect/Logic/Systems/DebugSystem.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

BoundingBoxSystem::BoundingBoxSystem(Engine& engine, Scene& scene) :
    System(scene, SystemTickStage_Subsequent)
{
    (void)engine;

    scene.components<BoundingBox>().addListener(*this);
}

void BoundingBoxSystem::update(BoundingBox& boundingBox)
{
    updateRecursively(*boundingBox.entity()->root());
}

void BoundingBoxSystem::tick(Real timeStep)
{
    (void)timeStep;

    // If the scene has a debug system
    if (scene().hasSystemType<DebugSystem>())
    {
        DebugSystem& debugSystem = scene().system<DebugSystem>();

        // Draw a debug box for each bounding box
        for (const BoundingBox& boundingBox : scene().components<BoundingBox>())
        {
            AxisAlignedBox axisAlignedBox = boundingBox.extents;
            Box box(axisAlignedBox.maximum() - axisAlignedBox.minimum());
            debugSystem.drawBox(DebugColor_Primary, box, axisAlignedBox.center());
        }
    }
}

void BoundingBoxSystem::receiveEvent(const ComponentEvent<BoundingBox>& event)
{
    // If a bounding box component was added to an entity
    if (event.type == ComponentEventType_Add)
    {
        Entity& entity = *event.entity;

        // Update the extents of the bounding box if it is adaptive
        BoundingBox::Iterator boundingBox = entity.component<BoundingBox>();
        if (boundingBox && boundingBox->adaptive)
        {
            // Update the extent of the bounding box
            updateRecursively(entity);
        }
    }
}

void BoundingBoxSystem::updateRecursively(Entity& entity)
{
    // Compute the bounding box of this entity
    BoundingBox::Iterator boundingBox = entity.component<BoundingBox>();
    if (boundingBox && boundingBox->adaptive)
    {
        // Start with an empty box
        AxisAlignedBox& axisAlignedBox = boundingBox->extents;
        axisAlignedBox = AxisAlignedBox();

        // Expand to fit all model that the component has
        Model::Iterator model = entity.component<Model>();
        if (model)
        {
            for (const ModelSurface& surface : model->surfaces)
            {
                Mesh& mesh = *surface.mesh;
                axisAlignedBox.expandToInclude(mesh.axisAlignedBox());
            }
        }

        // Transform the bounding box by the entity's global transform
        Transform::Iterator transform = entity.component<Transform>();
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
        BoundingBox::Iterator childBoundingBox = child.component<BoundingBox>();
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
