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
#include "Hect/Interface/Widgets/Panel.h"
#include "Hect/Logic/System.h"
#include "Hect/Logic/Components/Transform.h"
#include "Hect/Logic/Systems/InterfaceSystem.h"
#include "Hect/Math/Box.h"

namespace hect
{

class RenderSystem;

///
/// Provides debug functionality.
///
/// \system
class HECT_EXPORT DebugSystem :
    public System<DebugSystem>
{
public:
    DebugSystem(Engine& engine, Scene& scene);

    void tick(double timeStep) override;

    ///
    /// Enqueues a line to be rendered for a duration of time.
    ///
    /// \param color The color of the line.
    /// \param startPosition The world-space start position of the line.
    /// \param endPosition The world-space end position of the line.
    /// \param duration The duration of time in seconds that the line will be
    /// rendered henceforth.  If zero, then it will be rendered only until the
    /// next frame.
    void renderLine(Color color, Vector3 startPosition, Vector3 endPosition, double duration = 0.0);

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
    void renderBox(Color color, Box box, Vector3 position, Quaternion rotation = Quaternion(), double duration = 0.0);

    ///
    /// Adds all enqueued debug geometery to the a scene renderer.
    ///
    /// \param renderSystem The render system to render the debug geometry
    /// with.
    void addRenderCalls(RenderSystem& renderSystem);

    ///
    /// Toggles whether the debug interface is displayed.
    void toggleShowInterface();

    ///
    /// Thee material used to render lines.
    ///
    /// \property
    Material::Handle linesMaterial;

private:
    Mesh& meshForDuration(double duration);

    void createSystemPanel();
    void destroySystemPanel();

    Renderer& _renderer;
    Window& _window;
    InterfaceSystem::Handle _interfaceSystem;

    Interface::Handle _interface;
    Panel::Handle _systemPanel;

    Mesh _linesMesh;
};

}