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
}

void BoundingBoxSystem::forceUpdate(Entity& entity)
{
    updateHeierarchy(*entity.root());
}

void BoundingBoxSystem::forceUpdate(BoundingBox& boundingBox)
{
    forceUpdate(boundingBox.entity());
}

void BoundingBoxSystem::markForUpdate(Entity& entity)
{
    _markedForUpdate.push_back(entity.root()->id());
}

void BoundingBoxSystem::markForUpdate(BoundingBox& boundingBox)
{
    markForUpdate(boundingBox.entity());
}

void BoundingBoxSystem::tick(Real timeStep)
{
    (void)timeStep;

    // Update all entities marked for update
    for (EntityId id : _markedForUpdate)
    {
        Entity& entity = scene().entities().withId(id);
        forceUpdate(entity);
    }
    _markedForUpdate.clear();

    // If the scene has a debug system
    if (scene().hasSystemType<DebugSystem>())
    {
        DebugSystem& debugSystem = scene().system<DebugSystem>();

        // Draw a debug box for each bounding box
        for (const BoundingBox& boundingBox : scene().components<BoundingBox>())
        {
            AxisAlignedBox axisAlignedBox = boundingBox.axisAlignedBox;
            Box box(axisAlignedBox.maximum() - axisAlignedBox.minimum());
            debugSystem.drawBox(DebugColor_Primary, box, axisAlignedBox.center());
        }
    }
}

void BoundingBoxSystem::updateHeierarchy(Entity& entity)
{
    // Update the bounding box of this entity
    auto boundingBox = entity.component<BoundingBox>();
    if (boundingBox)
    {
        // Start with an empty box
        AxisAlignedBox& axisAlignedBox = boundingBox->axisAlignedBox;
        axisAlignedBox = AxisAlignedBox();

        // Expand to fit all model that the component has
        auto model = entity.component<Model>();
        if (model)
        {
            for (const ModelSurface& surface : model->surfaces)
            {
                Mesh& mesh = *surface.mesh;
                axisAlignedBox.expandToInclude(mesh.axisAlignedBox());
            }
        }

        // Transform the bounding box by the entities global transform
        auto transform = entity.component<Transform>();
        if (transform)
        {
            axisAlignedBox.scale(transform->globalScale);
            axisAlignedBox.rotate(transform->globalRotation);
            axisAlignedBox.translate(transform->globalPosition);
        }
    }

    // Update the bounding boxes of all children
    for (Entity& child : entity.children())
    {
        updateHeierarchy(child);

        // If the child has a bounding box
        auto childBoundingBox = child.component<BoundingBox>();
        if (childBoundingBox)
        {
            // Expand the bounding box to include this child
            if (boundingBox)
            {
                boundingBox->axisAlignedBox.expandToInclude(childBoundingBox->axisAlignedBox);
            }
        }
    }
}
