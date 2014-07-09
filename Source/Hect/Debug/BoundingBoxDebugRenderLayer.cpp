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
#include "BoundingBoxDebugRenderLayer.h"

#include "Hect/Logic/Scene.h"
#include "Hect/Logic/Components/BoundingBox.h"
#include "Hect/Logic/Components/Transform.h"

using namespace hect;

BoundingBoxDebugRenderLayer::BoundingBoxDebugRenderLayer(AssetCache& assetCache)
{
    _boundingBoxMaterial = assetCache.getHandle<Material>("Debug/BoundingBox.material");
    _boundingBoxMesh = assetCache.getHandle<Mesh>("Debug/BoundingBox.mesh");
}

void BoundingBoxDebugRenderLayer::render(Scene& scene, RenderSystem& renderSystem, RenderTarget& target)
{
    auto camera = renderSystem.activeCamera();
    if (camera)
    {
        Renderer& renderer = renderSystem.renderer();
        renderer.beginFrame();
        renderer.bindTarget(target);

        for (BoundingBox& boundingBox : scene.components<BoundingBox>())
        {
            const AxisAlignedBox& box = boundingBox.axisAlignedBox();
            Vector3 minimum = box.minimum();
            Vector3 maximum = box.maximum();
            Vector3 scale = maximum - minimum;
            Vector3 center = minimum + scale / 2;

            Transform transform;
            transform.setPosition(center);
            transform.setScale(scale);
            transform.updateGlobalTransform();

            renderSystem.renderMesh(*camera, target, *_boundingBoxMaterial, *_boundingBoxMesh, transform);
        }

        renderer.endFrame();
    }
}