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
#include "DebugSystem.h"

#include "Hect/Graphics/SceneRenderer.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

DebugSystem::DebugSystem(Engine& engine, Scene& scene) :
    System(scene, SystemTickStage_Precedent)
{
    AssetCache& assetCache = engine.assetCache();

    _coloredLineMaterial = assetCache.getHandle<Material>("Hect/ColoredLine.material");
    _boxMesh = assetCache.getHandle<Mesh>("Hect/Box.mesh");
}

void DebugSystem::drawBox(const Box& box, const Vector3& color, const Vector3& position, const Quaternion& rotation)
{
    (void)color;
    _boxes.emplace_back(box, position, rotation, *_coloredLineMaterial);
}

void DebugSystem::addRenderCalls(SceneRenderer& sceneRenderer)
{
    for (auto& box : _boxes)
    {
        sceneRenderer.addRenderCall(box.transform, *_boxMesh, box.material);
    }
}

void DebugSystem::tick(Real timeStep)
{
    (void)timeStep;
    _boxes.clear();
}

DebugSystem::DebugBox::DebugBox()
{
}

DebugSystem::DebugBox::DebugBox(const Box& box, const Vector3& position, const Quaternion& rotation, const Material& material) :
    box(box),
    material(material)
{
    transform.globalPosition = position;
    transform.globalScale = box.scale();
    transform.globalRotation = rotation;
}
