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
#include "DefaultScene.h"

#include "Hect/Scene/Components/BoundingBoxComponent.h"
#include "Hect/Scene/Components/CameraComponent.h"
#include "Hect/Scene/Components/DirectionalLightComponent.h"
#include "Hect/Scene/Components/LightProbeComponent.h"
#include "Hect/Scene/Components/MeshComponent.h"
#include "Hect/Scene/Components/RigidBodyComponent.h"
#include "Hect/Scene/Components/SkyBoxComponent.h"
#include "Hect/Scene/Components/TransformComponent.h"

#include "Hect/Scene/Systems/InputSystem.h"
#include "Hect/Scene/Systems/PhysicsSystem.h"
#include "Hect/Scene/Systems/TransformSystem.h"
#include "Hect/Scene/Systems/BoundingBoxSystem.h"
#include "Hect/Scene/Systems/CameraSystem.h"
#include "Hect/Scene/Systems/RenderSystem.h"
#include "Hect/Scene/Systems/DebugSystem.h"
#include "Hect/Scene/Systems/InterfaceSystem.h"

using namespace hect;

DefaultScene::DefaultScene(Engine& engine) :
    Scene(engine),
    _boundingBoxSystem(createSystem<BoundingBoxSystem>()),
    _cameraSystem(createSystem<CameraSystem>()),
    _debugSystem(createSystem<DebugSystem>()),
    _inputSystem(createSystem<InputSystem>()),
    _interfaceSystem(createSystem<InterfaceSystem>()),
    _physicsSystem(createSystem<PhysicsSystem>()),
    _renderSystem(createSystem<RenderSystem>()),
    _transformSystem(createSystem<TransformSystem>())
{
}

void DefaultScene::preTick(double timeStep)
{
    Scene::refresh();

    _inputSystem->updateAxes(timeStep);
    _debugSystem->clearEnqueuedDebugGeometry();
}

void DefaultScene::postTick(double timeStep)
{
    _physicsSystem->tick(timeStep);
    _transformSystem->updateCommittedTransforms();
    _cameraSystem->updateAllCameras();
    _interfaceSystem->tickAllInterfaces(timeStep);

    if (_boundingBoxSystem->isDebugEnabled())
    {
        _boundingBoxSystem->renderDebugGeometry();
    }

    Scene::refresh();
}

void DefaultScene::tick(double timeStep)
{
    preTick(timeStep);
    postTick(timeStep);
}

void DefaultScene::render(RenderTarget& target)
{
    _renderSystem->render(target);
    _interfaceSystem->renderAllInterfaces();
}
