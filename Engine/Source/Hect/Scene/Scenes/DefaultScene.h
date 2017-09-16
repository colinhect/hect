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
#pragma once

#include "Hect/Core/EventListener.h"
#include "Hect/Graphics/PhysicallyBasedSceneRenderer.h"
#include "Hect/Input/KeyboardEvent.h"
#include "Hect/Scene/Scene.h"
#include "Hect/Scene/Systems/BoundingBoxSystem.h"
#include "Hect/Scene/Systems/CameraSystem.h"
#include "Hect/Scene/Systems/DebugSystem.h"
#include "Hect/Scene/Systems/InputSystem.h"
#include "Hect/Scene/Systems/InterfaceSystem.h"
#include "Hect/Scene/Systems/PhysicsSystem.h"
#include "Hect/Scene/Systems/TransformSystem.h"

namespace hect
{

///
/// The default Scene type.
///
/// \scene
class HECT_EXPORT DefaultScene :
    public Scene,
    public EventListener<KeyboardEvent>
{
public:

    ///
    /// Constructs an empty default scene.
    ///
    /// \param engine The engine.
    DefaultScene(Engine& engine);

    void pre_tick(Seconds time_step);
    void post_tick(Seconds time_step);

    // Scene overrides
    virtual void tick(Seconds time_step) override;
    virtual void render(RenderTarget& target) override;

    // EventListener overrides
    virtual void receive_event(const KeyboardEvent& event) override;

    ///
    /// Returns the InterfaceSystem.
    InterfaceSystem& interface_system();

    ///
    /// Returns the DebugSystem.
    DebugSystem& debug_system();

    ///
    /// Returns the InputSystem.
    InputSystem& input_system();

    ///
    /// Returns the CameraSystem.
    CameraSystem& camera_system();

    ///
    /// Returns the BoundingBoxSystem.
    BoundingBoxSystem& bounding_box_system();

    ///
    /// Returns the TransformSystem.
    TransformSystem& transform_system();

    ///
    /// Returns the PhysicsSystem.
    PhysicsSystem& physics_system();

private:
    InterfaceSystem _interface_system;
    DebugSystem _debug_system;
    InputSystem _input_system;
    CameraSystem _camera_system;
    BoundingBoxSystem _bounding_box_system;
    TransformSystem _transform_system;
    PhysicsSystem _physics_system;
    PhysicallyBasedSceneRenderer _scene_renderer;
    bool _debug_rendering_enabled { false };
};

}
