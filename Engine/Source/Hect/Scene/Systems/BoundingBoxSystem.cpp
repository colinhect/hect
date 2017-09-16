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

#include "Hect/Scene/Components/GeometryComponent.h"
#include "Hect/Scene/Components/TransformComponent.h"

using namespace hect;

BoundingBoxSystem::BoundingBoxSystem(Scene& scene) :
    System(scene)
{
}

BoundingBoxSystem::BoundingBoxSystem(Scene& scene, DebugSystem& debug_system) :
    System(scene),
    _debug_system(&debug_system)
{
}

void BoundingBoxSystem::update_bounding_box(BoundingBoxComponent& bounding_box)
{
    Entity::Handle root = bounding_box.entity().root();
    update_recursively(*root);
}

void BoundingBoxSystem::render_debug_geometry()
{
    if (_debug_system)
    {
        // Render a debug box for each bounding box
        for (const BoundingBoxComponent& bounding_box : scene().components<BoundingBoxComponent>())
        {
            AxisAlignedBox axis_aligned_box = bounding_box.globalExtents;
            Box box(axis_aligned_box.maximum() - axis_aligned_box.minimum());

            _debug_system->render_box(Color::Green, box, axis_aligned_box.center());
        }
    }
}

void BoundingBoxSystem::update_recursively(Entity& entity)
{
    BoundingBoxComponent* entity_bounding_box = nullptr;

    // Compute the bounding box of this entity
    if (entity.has_component<BoundingBoxComponent>())
    {
        auto& bounding_box = entity.component<BoundingBoxComponent>();
        entity_bounding_box = &bounding_box;

        // Update the local extents if the bounding box is adaptive
        if (bounding_box.adaptive)
        {
            // Start with an empty box
            AxisAlignedBox& local_extents = bounding_box.local_extents;
            local_extents = AxisAlignedBox();

            // Expand to fit all meshes that the component has
            if (entity.has_component<GeometryComponent>())
            {
                auto& geometry = entity.component<GeometryComponent>();
                for (const GeometrySurface& surface : geometry.surfaces)
                {
                    Mesh& mesh = *surface.mesh;
                    local_extents.expand_to_include(mesh.axis_aligned_box());
                }
            }
        }

        // Sync the global extents with the local extents
        bounding_box.globalExtents = bounding_box.local_extents;

        // Transform the global extents of the bounding box by the entity's
        // global transform
        if (entity.has_component<TransformComponent>())
        {
            auto& transform = entity.component<TransformComponent>();

            AxisAlignedBox& globalExtents = bounding_box.globalExtents;
            globalExtents.scale(transform.global_scale);
            globalExtents.rotate(transform.global_rotation);
            globalExtents.translate(transform.global_position);
        }
    }

    // Recursively compute the bounding boxes of all children
    for (Entity& child : entity.children())
    {
        update_recursively(child);

        if (entity_bounding_box)
        {
            // If the child has a bounding box
            if (child.has_component<BoundingBoxComponent>())
            {
                auto& child_bounding_box = child.component<BoundingBoxComponent>();

                // Expand the bounding box to include this child
                entity_bounding_box->globalExtents.expand_to_include(child_bounding_box.globalExtents);
            }
        }
    }
}

void BoundingBoxSystem::on_component_added(BoundingBoxComponent& bounding_box)
{
    // Update the extents of the bounding box if it is adaptive
    if (bounding_box.adaptive)
    {
        Entity& entity = bounding_box.entity();

        // Update the extent of the bounding box
        update_recursively(entity);
    }
}
