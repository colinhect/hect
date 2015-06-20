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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Graphics/Color.h"
#include "Hect/Graphics/Material.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Logic/System.h"
#include "Hect/Logic/Components/Transform.h"
#include "Hect/Math/Box.h"

namespace hect
{

class RenderSystem;

///
/// A preset color for debug geometry.
enum class DebugColor
{

    ///
    /// The primary debug color.
    Primary,

    ///
    /// The secondary debug color.
    Secondary,

    ///
    /// The tertiary debug color.
    Tertiary
};

///
/// Provides debug functionality.
///
/// \system
class HECT_EXPORT DebugSystem :
    public System<DebugSystem>
{
public:
    DebugSystem(Engine& engine, Scene& scene);

    ///
    /// Enqueues a wire-frame box to be rendered on the next frame.
    ///
    /// \param color The color of the lines of the box.
    /// \param box The box to render.
    /// \param position The world-space position of the box.
    /// \param rotation The world-space rotation of the box.
    void renderBox(DebugColor color, const Box& box, const Vector3& position, const Quaternion& rotation = Quaternion());

    ///
    /// Adds all enqueued debug geometery to the a scene renderer.
    ///
    /// \param renderSystem The render system to render the debug geometry
    /// with.
    void addRenderCalls(RenderSystem& renderSystem);

    void initialize() override;
    void tick(double timeStep) override;

    ///
    /// Returns whether the debug system is enabled.
    bool isEnabled() const;

    ///
    /// Sets whether the debug system is enabled.
    ///
    /// \param enabled True to enabled; false to disable.
    void setEnabled(bool enabled);

    ///
    /// The shader used to render colored lines.
    ///
    /// \property{required}
    Shader::Handle coloredLineShader;

    ///
    /// The mesh used to render bounding boxes.
    ///
    /// \property{required}
    Mesh::Handle boxMesh;

private:
    void addColoredMaterial(const Color& color);

    class DebugBox
    {
    public:
        DebugBox();
        DebugBox(const Box& box, const Vector3& position, const Quaternion& rotation, DebugColor color);

        Box box;
        Transform transform;
        DebugColor color { DebugColor::Primary };
    };

    Renderer& _renderer;

    bool _enabled;

    std::vector<DebugBox> _boxes;
    std::vector<Material> _coloredMaterials;
};

}