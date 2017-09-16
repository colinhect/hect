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
#include "CameraSystem.h"

#include "Hect/Runtime/Engine.h"
#include "Hect/Scene/Components/TransformComponent.h"

using namespace hect;

CameraSystem::CameraSystem(Scene& scene) :
    System(scene)
{
}

CameraComponent::Iterator CameraSystem::active_camera()
{
    CameraComponent::Iterator camera;

    if (_active_camera_entity && _active_camera_entity->has_component<CameraComponent>())
    {
        camera = _active_camera_entity->component<CameraComponent>().iterator();
    }

    return camera;
}

void CameraSystem::set_active_camera(CameraComponent& camera)
{
    _active_camera_entity = camera.entity().handle();
}

void CameraSystem::update_camera(CameraComponent& camera)
{
    // If the camera's entity has a transform then use it to compute the
    // camera's position, front, and up vectors
    if (camera.entity().has_component<TransformComponent>())
    {
        auto& transform = camera.entity().component<TransformComponent>();

        camera.position = transform.global_position;

        Quaternion rotation = transform.global_rotation;
        camera.front = (rotation * Vector3::UnitY).normalized();
        camera.up = (rotation * Vector3::UnitZ).normalized();
    }

    camera.right = camera.front.cross(camera.up).normalized();

    camera.view_matrix = Matrix4::create_view(camera.position, camera.front, camera.up);
    camera.projection_matrix = Matrix4::create_perspective(camera.field_of_view, camera.aspect_ratio, camera.near_clip, camera.far_clip);

    camera.frustum = Frustum(camera.position, camera.front, camera.up, camera.field_of_view, camera.aspect_ratio, camera.near_clip, camera.far_clip);
}

void CameraSystem::update_all_cameras()
{
    for (CameraComponent& camera : scene().components<CameraComponent>())
    {
        update_camera(camera);
    }
}

void CameraSystem::on_component_added(CameraComponent& camera)
{
    if (!_active_camera_entity)
    {
        _active_camera_entity = camera.entity().handle();
    }
}
