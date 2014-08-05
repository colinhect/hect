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
#include "DefaultScene.h"

#include <Hect/Debug/Systems/DebugRenderSystem.h>
#include <Hect/Graphics/Components/Camera.h>
#include <Hect/Graphics/Components/DirectionalLight.h>
#include <Hect/Graphics/Components/LightProbe.h>
#include <Hect/Graphics/Components/Model.h>
#include <Hect/Graphics/Components/SkyBox.h>
#include <Hect/Graphics/Systems/PhysicallyBasedRenderSystem.h>
#include <Hect/Physics/Components/RigidBody.h>
#include <Hect/Physics/Systems/PhysicsSystem.h>
#include <Hect/Spacial/Components/BoundingBox.h>
#include <Hect/Spacial/Components/Transform.h>
#include <Hect/Spacial/Systems/BoundingBoxSystem.h>
#include <Hect/Spacial/Systems/TransformSystem.h>

using namespace hect;

DefaultScene::DefaultScene(Input& input, Renderer& renderer, hect::AssetCache& assetCache) :
    _taskPool(4)
{
    registerComponent<Camera>();
    registerComponent<DirectionalLight>();
    registerComponent<LightProbe>();
    registerComponent<Model>();
    registerComponent<SkyBox>();
    registerComponent<RigidBody>();
    registerComponent<Transform>();
    registerComponent<BoundingBox>();

    addSystem<PhysicallyBasedRenderSystem>(assetCache, renderer);
    addSystem<DebugRenderSystem>(input, assetCache, renderer);
    addSystem<TransformSystem>();
    addSystem<BoundingBoxSystem>();
    addSystem<PhysicsSystem>();
}

void DefaultScene::update(Real timeStep)
{
    if (_physicsTaskHandle)
    {
        _physicsTaskHandle->wait();
        system<PhysicsSystem>().updateTransforms();
    }
    system<TransformSystem>().update();
    system<BoundingBoxSystem>().update();

    system<PhysicallyBasedRenderSystem>().updateActiveCamera();

    systemUpdate(timeStep);

    _physicsTaskHandle = _taskPool.enqueue([this, timeStep]
    {
        system<PhysicsSystem>().simulate(timeStep, 4);
    });    
}

void DefaultScene::render(Real delta, RenderTarget& target)
{
    delta;

    system<PhysicallyBasedRenderSystem>().renderAll(target);
    system<DebugRenderSystem>().renderAll(target);
}