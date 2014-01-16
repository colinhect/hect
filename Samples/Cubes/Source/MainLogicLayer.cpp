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
#include "MainLogicLayer.h"


MainLogicLayer::MainLogicLayer(AssetCache& assetCache, InputSystem& inputSystem, Window& window, Renderer& renderer) :
    _assetCache(&assetCache),
    _input(&inputSystem),
    _window(&window),
    _renderSystem(renderer),
    _playerCameraSystem(inputSystem)
{
    EntitySerializer& entitySerializer = _scene.entitySerializer();
    entitySerializer.registerComponent<PlayerCamera, PlayerCameraSerializer>("PlayerCamera");

    _scene.addSystem(_cameraSystem);
    _scene.addSystem(_renderSystem);
    _scene.addSystem(_physicsSystem);
    _scene.addSystem(_playerCameraSystem);

    DataValue& sceneDataValue = assetCache.get<DataValue>("Scene.scene");
    _scene.load(sceneDataValue, assetCache);

    Dispatcher<KeyboardEvent>& keyboardDispatcher = _input->keyboard().dispatcher();
    keyboardDispatcher.addListener(*this);

    _window->setCursorLocked(true);
}

MainLogicLayer::~MainLogicLayer()
{
    Dispatcher<KeyboardEvent>& keyboardDispatcher = _input->keyboard().dispatcher();
    keyboardDispatcher.removeListener(*this);
}

void MainLogicLayer::fixedUpdate(double timeStep)
{
    _cameraSystem.update();
    _physicsSystem.update(timeStep, 1);
    _playerCameraSystem.update(timeStep);
    _scene.refresh();

    _input->updateAxes(timeStep);
}

void MainLogicLayer::frameUpdate(double delta)
{
    delta;

    if (!_cameraSystem.hasCamera())
    {
        return;
    }

    Camera& camera = _cameraSystem.camera();
    camera.setAspectRatio(_window->aspectRatio());

    _renderSystem.renderAll(camera, *_window);

    _window->swapBuffers();
}

void MainLogicLayer::receiveEvent(const KeyboardEvent& event)
{
    if (event.type != KeyboardEventType::KeyDown)
    {
        return;
    }

    if (event.key == Key::Esc)
    {
        setActive(false);
    }
}