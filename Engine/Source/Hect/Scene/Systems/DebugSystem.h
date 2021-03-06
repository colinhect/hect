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
#include "Hect/Graphics/Color.h"
#include "Hect/Graphics/Material.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Math/Box.h"
#include "Hect/Scene/System.h"
#include "Hect/Scene/Components/TransformComponent.h"
#include "Hect/Scene/Systems/InterfaceSystem.h"

namespace hect
{

class PhysicallyBasedSceneRenderer;

///
/// Provides debug functionality.
///
/// \system
class HECT_EXPORT DebugSystem :
    public System<DebugSystem>
{
public:
    DebugSystem(Scene& scene, AssetCache& asset_cache, InterfaceSystem& interface_system);

    ///
    /// Clears all enqueued debug geometry.
    void clear_enqueued_debug_geometry();

    ///
    /// Enqueues a line to be rendered for a duration of time.
    ///
    /// \param color The color of the line.
    /// \param start_position The world-space start position of the line.
    /// \param end_position The world-space end position of the line.
    /// \param duration The duration of time in seconds that the line will be
    /// rendered henceforth.  If zero, then it will be rendered only until the
    /// next frame.
    void render_line(Color color, Vector3 start_position, Vector3 end_position, double duration = 0.0);

    ///
    /// Enqueues a wire-frame box to be rendered on the next frame.
    ///
    /// \param color The color of the lines of the box.
    /// \param box The box to render.
    /// \param position The world-space position of the box.
    /// \param rotation The world-space rotation of the box.
    /// \param duration The duration of time in seconds that the line will be
    /// rendered henceforth.  If zero, then it will be rendered only until the
    /// next frame.
    void render_box(Color color, Box box, Vector3 position, Quaternion rotation = Quaternion(), double duration = 0.0);

    ///
    /// Adds all enqueued debug geometery to the a scene renderer.
    ///
    /// \param scene_renderer The scene renderer to render the debug geometry
    /// with.
    void add_render_calls(PhysicallyBasedSceneRenderer& scene_renderer);

    ///
    /// Thee material used to render lines.
    ///
    /// \property
    AssetHandle<Material> lines_material;

private:
    Mesh& mesh_for_duration(double duration);

    InterfaceSystem& _interface_system;

    Interface::Handle _interface;

    Mesh _lines_mesh;
};

}
