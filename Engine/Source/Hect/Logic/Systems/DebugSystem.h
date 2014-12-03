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
#pragma once

#include "Hect/Graphics/Pass.h"
#include "Hect/Logic/Scene.h"
#include "Hect/Logic/Components/Transform.h"
#include "Hect/Spacial/Box.h"

namespace hect
{

class SceneRenderer;

///
/// Provides debug functionality.
///
/// \system
class DebugSystem :
    public System
{
public:
    DebugSystem(Scene& scene);

    ///
    /// Enqueues a wire-frame box to be rendered on the next frame.
    ///
    /// \param box The box to draw.
    /// \param color The color of the lines of the box.
    /// \param position The world-space position of the box.
    /// \param rotation The world-space rotation of the box.
    void drawBox(const Box& box, const Vector3& color, const Vector3& position, const Quaternion& rotation = Quaternion());

    ///
    /// Adds all enqueued debug geometery to the a scene renderer.
    ///
    /// \param scenRenderer The scene renderer to render the debug geometry to.
    void addRenderCalls(SceneRenderer& sceneRenderer);

    ///
    /// Clear all enqueued debug geometry.
    void clear();

private:

    class DebugBox
    {
    public:
        DebugBox();
        DebugBox(const Box& box, const Vector3& color, const Vector3& position, const Quaternion& rotation, const Pass& pass);

        Box box;
        Transform transform;
        Pass pass;
    };

    std::vector<DebugBox> _boxes;

    AssetHandle<Shader> _coloredLineShader;
    AssetHandle<Mesh> _boxMesh;
};

}