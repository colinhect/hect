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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Scene/System.h"
#include "Hect/Scene/Systems/DebugSystem.h"
#include "Hect/Scene/Components/BoundingBoxComponent.h"

namespace hect
{

///
/// Manages the BoundingBoxComponent hierarchies of a Scene.
///
/// \system
class HECT_EXPORT BoundingBoxSystem :
    public System<BoundingBoxSystem, Components<BoundingBoxComponent>>
{
    friend class TransformSystem;
public:
    BoundingBoxSystem(Scene& scene);
    BoundingBoxSystem(Scene& scene, DebugSystem& debug_system);

    ///
    /// Updates the extents of a bounding box and all bounding boxes affected.
    ///
    /// \param bounding_box The bounding box to update.
    void update_bounding_box(BoundingBoxComponent& bounding_box);

    ///
    /// Renders geometry providing debug information for bounding boxes.
    void render_debug_geometry();

private:
    void update_recursively(Entity& entity);

    // System overrides
    void on_component_added(BoundingBoxComponent& bounding_box) override;

    DebugSystem* _debug_system { nullptr };
};

}
