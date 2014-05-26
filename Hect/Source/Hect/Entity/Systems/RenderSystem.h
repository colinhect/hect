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

#include "Hect/Entity/Scene.h"
#include "Hect/Entity/Components/Camera.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/Renderer.h"

namespace hect
{

///
/// Provides basic rendering.
class RenderSystem
{
public:

    ///
    /// Constructs the system given the renderer.
    ///
    /// \param scene The scene.
    /// \param renderer The renderer.
    RenderSystem(Scene& scene, Renderer& renderer);
    
    ///
    /// Renders all visible entities.
    ///
    /// \param camera The camera to render from.
    /// \param target The target to render to.
    virtual void renderAll(Camera& camera, RenderTarget& target);

protected:
    void renderMeshPass(const Camera& camera, const RenderTarget& target, const Pass& pass, Mesh& mesh, const Transform& transform);

private:
    Scene* _scene;
    Renderer* _renderer;
};

}