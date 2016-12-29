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
#include "Hect/Input/KeyboardEvent.h"
#include "Hect/Scene/Scene.h"
#include "Hect/Scene/Systems/BoundingBoxSystem.h"
#include "Hect/Scene/Systems/CameraSystem.h"
#include "Hect/Scene/Systems/DebugSystem.h"
#include "Hect/Scene/Systems/InputSystem.h"
#include "Hect/Scene/Systems/InterfaceSystem.h"
#include "Hect/Scene/Systems/PhysicsSystem.h"
#include "Hect/Scene/Systems/RenderSystem.h"
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

    void preTick(double timeStep);
    void postTick(double timeStep);

    // Scene overrides
    virtual void tick(double timeStep) override;
    virtual void render(RenderTarget& target) override;

    // EventListener overrides
    virtual void receiveEvent(const KeyboardEvent& event) override;

private:
    BoundingBoxSystem& _boundingBoxSystem;
    CameraSystem& _cameraSystem;
    DebugSystem& _debugSystem;
    InputSystem& _inputSystem;
    InterfaceSystem& _interfaceSystem;
    PhysicsSystem& _physicsSystem;
    RenderSystem& _renderSystem;
    TransformSystem& _transformSystem;
    bool _debugRenderingEnabled { false };
};

}