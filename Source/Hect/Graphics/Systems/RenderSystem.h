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

#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/Components/Camera.h"
#include "Hect/Logic/System.h"

namespace hect
{

///
/// Provides basic rendering.
class RenderSystem :
    public System
{
public:
    RenderSystem(Scene& scene, Renderer& renderer);
    virtual ~RenderSystem();

    void updateActiveCamera();

    ///
    /// Returns the active camera in the scene.
    Component<Camera>::Iter activeCamera();

    ///
    /// Renders all visible entities.
    ///
    /// \param target The target to render to.
    virtual void renderAll( RenderTarget& target);

    void render(Camera& camera, RenderTarget& target, Entity& entity, bool frustumTest = true);
    void renderMesh(const Camera& camera, const RenderTarget& target, Material& material, Mesh& mesh, const Transform& transform);
    void renderMeshPass(const Camera& camera, const RenderTarget& target, Pass& pass, Mesh& mesh, const Transform& transform);

    Renderer& renderer();

private:
    Renderer* _renderer;
};

}