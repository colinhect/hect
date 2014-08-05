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
#include <Hect/Input/Systems/InputSystem.h>
#include <Hect/Physics/Components/RigidBody.h>
#include <Hect/Physics/Systems/PhysicsSystem.h>
#include <Hect/Spacial/Components/BoundingBox.h>
#include <Hect/Spacial/Components/Transform.h>
#include <Hect/Spacial/Systems/BoundingBoxSystem.h>
#include <Hect/Spacial/Systems/TransformSystem.h>

using namespace hect;

DefaultScene::DefaultScene(InputDevices& inputDevices, Renderer& renderer, RenderTarget& renderTarget, AssetCache& assetCache) :
    _renderTarget(&renderTarget),
    _taskPool(4)
{
    registerComponent<BoundingBox>();
    registerComponent<Camera>();
    registerComponent<DirectionalLight>();
    registerComponent<LightProbe>();
    registerComponent<Model>();
    registerComponent<RigidBody>();
    registerComponent<SkyBox>();
    registerComponent<Transform>();

    addSystem<BoundingBoxSystem>();
    addSystem<DebugRenderSystem>(inputDevices, assetCache, renderer);
    addSystem<InputSystem>(inputDevices);
    addSystem<PhysicallyBasedRenderSystem>(assetCache, renderer);
    addSystem<PhysicsSystem>();
    addSystem<TransformSystem>();
}

void DefaultScene::preFixedUpdate()
{
    system<InputSystem>().update();

    if (_physicsTaskHandle)
    {
        _physicsTaskHandle->wait();
        system<PhysicsSystem>().updateTransforms();
    }

    system<TransformSystem>().update();
    system<BoundingBoxSystem>().update();

    system<PhysicallyBasedRenderSystem>().updateActiveCamera();
}

void DefaultScene::fixedUpdate()
{
}

void DefaultScene::postFixedUpdate()
{
    _physicsTaskHandle = _taskPool.enqueue([this]
    {
        system<PhysicsSystem>().simulate(timeStep());
    });
}

void DefaultScene::frameUpdate(Real delta)
{
    delta;

    system<PhysicallyBasedRenderSystem>().renderAll(*_renderTarget);
    system<DebugRenderSystem>().renderAll(*_renderTarget);
}