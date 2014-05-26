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
#include "CameraSystem.h"

#include "Hect/Entity/Scene.h"
#include "Hect/Entity/Components/Camera.h"
#include "Hect/Entity/Components/Transform.h"

using namespace hect;

CameraSystem::CameraSystem(Scene& scene) :
    _scene(&scene)
{
}

bool CameraSystem::hasCamera() const
{
    bool cameraExists = false;
    for (Camera& camera : _scene->components<Camera>())
    {
        camera;
        cameraExists = true;
        break;
    }
    return cameraExists;
}

Camera& CameraSystem::camera()
{
    Camera* foundCamera = nullptr;
    for (Camera& camera : _scene->components<Camera>())
    {
        foundCamera = &camera;
        break;
    }
    assert(foundCamera);
    return *foundCamera;
}

void CameraSystem::update()
{
    for (Camera& camera : _scene->components<Camera>())
    {
        EntityId entityId = camera.entityId();
        Transform& transform = _scene->entityComponent<Transform>(entityId);

        camera.transformTo(transform);
    }
}
