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

#include <Hect/Graphics/Components/Camera.h>
#include <Hect/Graphics/Components/Transform.h>

PlayerCameraSystem::PlayerCameraSystem(Scene& scene, InputSystem& inputSystem) :
    System(scene),
    _mouse(&inputSystem.mouse()),
    _viewX(&inputSystem.axisWithName("viewX")),
    _viewY(&inputSystem.axisWithName("viewY")),
    _moveX(&inputSystem.axisWithName("moveX")),
    _moveY(&inputSystem.axisWithName("moveY")),
    _roll(&inputSystem.axisWithName("roll")),
    _speed(16)
{
}

void PlayerCameraSystem::update(Real timeStep)
{
    for (PlayerCamera& playerCamera : scene().components<PlayerCamera>())
    {
        Entity& entity = playerCamera.entity();

        auto transform = entity.component<Transform>();
        if (transform)
        {
            auto camera = entity.component<Camera>();
            if (camera)
            {
                const Vector3& up = camera->up();
                const Vector3& right = camera->right();
                const Vector3& front = camera->front();

                Real rotateSpeed = timeStep * 50;
                Real rollSpeed = timeStep * 2;
                Real moveSpeed = timeStep * _speed;

                if (_mouse->mode() == MouseMode::Relative)
                {
                    transform->rotate(up, _viewX->value() * rotateSpeed);
                    transform->rotate(right, _viewY->value() * -rotateSpeed);
                    transform->rotate(front, _roll->value() * -rollSpeed);
                }

                transform->translate(right * _moveX->value() * moveSpeed);
                transform->translate(front * _moveY->value() * moveSpeed);
            }
        }
    }
}