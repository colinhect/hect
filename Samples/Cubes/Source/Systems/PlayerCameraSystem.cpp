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
#include "PlayerCameraSystem.h"

PlayerCameraSystem::PlayerCameraSystem(InputSystem& inputSystem) :
    _mouse(&inputSystem.mouse()),
    _viewX(&inputSystem.axisWithName("viewX")),
    _viewY(&inputSystem.axisWithName("viewY")),
    _moveX(&inputSystem.axisWithName("moveX")),
    _moveY(&inputSystem.axisWithName("moveY")),
    _roll(&inputSystem.axisWithName("roll")),
    _speed(16)
{
}

bool PlayerCameraSystem::includesEntity(const Entity& entity) const
{
    return entity.hasComponent<Transform>() && entity.hasComponent<Camera>() && entity.hasComponent<PlayerCamera>();
}

void PlayerCameraSystem::update(double timeStep)
{
    if (entities().empty())
    {
        return;
    }

    Entity& entity = entities().front();

    Transform& transform = entity.component<Transform>();
    Camera& camera = entity.component<Camera>();
    
    transform.rotateGlobal(camera.up(), Angle<>::fromRadians(_viewX->value() * 50.0 * timeStep));
    transform.rotateGlobal(camera.right(), Angle<>::fromRadians(_viewY->value() * -50.0 * timeStep));
    transform.rotateGlobal(camera.front(),  Angle<>::fromRadians(_roll->value() * -2.0 * timeStep));
    transform.translate(camera.right() * _moveX->value() * _speed * timeStep);
    transform.translate(camera.front() * _moveY->value() * _speed * timeStep);
}