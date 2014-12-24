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

#include "Hect/Logic/Components/Transform.h"

using namespace hect;

CameraSystem::CameraSystem(Scene& scene) :
    System(scene, SystemTickStage_Subsequent)
{
    scene.components<Camera>().addListener(*this);
}

Component<Camera>::Iterator CameraSystem::activeCamera()
{
    Component<Camera>::Iterator camera;

    if (_activeCameraEntity)
    {
        camera = _activeCameraEntity->component<Camera>();
    }

    return camera;
}

void CameraSystem::setActiveCamera(Camera& camera)
{
    _activeCameraEntity = camera.entity().createHandle();
}

void CameraSystem::updateCamera(Camera& camera)
{
    auto transform = camera.entity().component<Transform>();
    if (transform)
    {
        const Quaternion& rotation = transform->globalRotation;
        camera.front = (rotation * Vector3::unitZ()).normalized();
        camera.up = (rotation * Vector3::unitY()).normalized();
        camera.right = camera.front.cross(camera.up).normalized();

        camera.position = transform->globalPosition;

        camera.viewMatrix = Matrix4::createView(camera.position, camera.front, camera.up);
        camera.projectionMatrix = Matrix4::createPerspective(camera.fieldOfView, camera.aspectRatio, camera.nearClip, camera.farClip);

        camera.frustum = Frustum(camera.position, camera.front, camera.up, camera.fieldOfView, camera.aspectRatio, camera.nearClip, camera.farClip);
    }
}

void CameraSystem::tick(Real timeStep)
{
    (void)timeStep;

    for (Camera& camera : scene().components<Camera>())
    {
        updateCamera(camera);
    }
}

void CameraSystem::receiveEvent(const ComponentEvent<Camera>& event)
{
    if (event.type == ComponentEventType_Add)
    {
        // If there is no active camera then use the most recently added camera
        if (!_activeCameraEntity)
        {
            _activeCameraEntity = event.entity().createHandle();
        }
    }
}