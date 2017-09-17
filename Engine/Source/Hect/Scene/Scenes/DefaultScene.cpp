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

#include "Hect/Runtime/Platform.h"
#include "Hect/Scene/Systems/InputSystem.h"
#include "Hect/Scene/Systems/PhysicsSystem.h"
#include "Hect/Scene/Systems/TransformSystem.h"
#include "Hect/Scene/Systems/BoundingBoxSystem.h"
#include "Hect/Scene/Systems/CameraSystem.h"
#include "Hect/Scene/Systems/DebugSystem.h"
#include "Hect/Scene/Systems/InterfaceSystem.h"

using namespace hect;

DefaultScene::DefaultScene(Engine& engine) :
    Scene(engine),
    _interface_system(*this, engine.asset_cache(), engine.platform(), engine.renderer(), engine.vector_renderer()),
    _debug_system(*this, engine.asset_cache(), _interface_system),
    _input_system(*this, engine.platform(), engine.settings()),
    _camera_system(*this),
    _bounding_box_system(*this, _debug_system),
    _transform_system(*this, _bounding_box_system),
    _physics_system(*this, _transform_system),
    _scene_renderer(engine.asset_cache(), engine.task_pool())
{
    Platform& platform = engine.platform();
    if (platform.has_keyboard())
    {
        Keyboard& keyboard = platform.keyboard();
        keyboard.register_listener(*this);
    }
}

void DefaultScene::pre_tick(Seconds time_step)
{
    Scene::refresh();

    _input_system.update_axes(time_step);
    _debug_system.clear_enqueued_debug_geometry();
}

void DefaultScene::post_tick(Seconds time_step)
{
    _physics_system.wait_for_simulation_task();
    _physics_system.sync_with_simulation();
    _physics_system.begin_simulation_task(engine().task_pool(), time_step);

    _transform_system.update_committed_transforms();
    _camera_system.update_all_cameras();

    _interface_system.tick_all_interfaces(time_step);

    if (_debug_rendering_enabled)
    {
        _bounding_box_system.render_debug_geometry();
    }

    Scene::refresh();
}

void DefaultScene::tick(Seconds time_step)
{
    pre_tick(time_step);
    post_tick(time_step);
}

void DefaultScene::render(RenderTarget& target)
{
    Renderer& renderer = engine().renderer();
    _scene_renderer.render(*this, _camera_system, renderer, target);
    _interface_system.render_all_interfaces();
}

void DefaultScene::receive_event(const KeyboardEvent& event)
{
    if (event.is_key_down(Key::F5))
    {
        _debug_rendering_enabled = !_debug_rendering_enabled;
    }
}

InterfaceSystem& DefaultScene::interface_system()
{
    return _interface_system;
}

DebugSystem& DefaultScene::debug_system()
{
    return _debug_system;
}

InputSystem& DefaultScene::input_system()
{
    return _input_system;
}

CameraSystem& DefaultScene::camera_system()
{
    return _camera_system;
}

BoundingBoxSystem& DefaultScene::bounding_box_system()
{
    return _bounding_box_system;
}

TransformSystem& DefaultScene::transform_system()
{
    return _transform_system;
}

PhysicsSystem& DefaultScene::physics_system()
{
    return _physics_system;
}
