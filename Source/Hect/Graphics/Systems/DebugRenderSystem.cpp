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
#include "DebugRenderSystem.h"

#include "Hect/Logic/Scene.h"
#include "Hect/Graphics/MeshWriter.h"
#include "Hect/Graphics/Components/Transform.h"

using namespace hect;

DebugRenderSystem::DebugRenderSystem(Scene& scene, Renderer& renderer, AssetCache& assetCache) :
    RenderSystem(scene, renderer),
    _renderer(&renderer)
{
    _coloredLineMaterial = assetCache.getHandle<Material>("Hect/Debug/ColoredLine.material");
    _transformMesh = assetCache.getHandle<Mesh>("Hect/Debug/Transform.mesh");
}

void DebugRenderSystem::renderAllLayers(Camera& camera, RenderTarget& target)
{
    camera;

    _renderer->beginFrame();
    _renderer->bindTarget(target);

    for (Transform& transform : scene().components<Transform>())
    {
        for (const Pass& pass : _coloredLineMaterial->techniques()[0].passes())
        {
            renderMeshPass(camera, target, pass, *_transformMesh, transform);
        }
    }

    _renderer->endFrame();
}