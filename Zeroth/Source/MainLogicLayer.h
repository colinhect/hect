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

#include <Hect/Core/LogicLayer.h>
#include <Hect/Core/Listener.h>
#include <Hect/Core/Uncopyable.h>
#include <Hect/Core/TaskPool.h>
#include <Hect/Asset/AssetCache.h>
#include <Hect/Logic/Scene.h>
#include <Hect/Graphics/Systems/CameraSystem.h>
#include <Hect/Graphics/Systems/RenderSystem.h>
#include <Hect/Graphics/Systems/TransformSystem.h>
#include <Hect/Physics/Systems/PhysicsSystem.h>
#include <Hect/Graphics/Renderer.h>
#include <Hect/Graphics/Window.h>
#include <Hect/Input/InputSystem.h>

using namespace hect;

#include "Components/PlayerCamera.h"
#include "Systems/PlayerCameraSystem.h"

class MainLogicLayer :
    public LogicLayer,
    public Listener<KeyboardEvent>,
    public Uncopyable
{
public:
    MainLogicLayer(AssetCache& assetCache, InputSystem& inputSystem, Window& window, Renderer& renderer);
    ~MainLogicLayer();

    void fixedUpdate(Real timeStep);
    void frameUpdate(Real delta);

    void receiveEvent(const KeyboardEvent& event);

private:
    AssetCache* _assetCache;
    InputSystem* _input;
    Window* _window;

    TaskPool _taskPool;

    Scene _scene;

    Entity::Iter _player;
    Entity::Iter _cube;

    CameraSystem _cameraSystem;
    RenderSystem _renderSystem;
    TransformSystem _transformSystem;
    PhysicsSystem _physicsSystem;

    PlayerCameraSystem _playerCameraSystem;
};