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

#include "Hect/Core/Export.h"
#include "Hect/Scene/System.h"
#include "Hect/Scene/Components/CameraComponent.h"

namespace hect
{

///
/// Updates and tracks the active CameraComponent in a Scene.
///
/// \system
class HECT_EXPORT CameraSystem :
    public System<CameraSystem, Components<CameraComponent>>
{
public:
    CameraSystem(Scene& scene);

    ///
    /// Returns the active camera in the scene.
    CameraComponent* active_camera();

    ///
    /// Sets the active camera in the scene.
    ///
    /// \param camera The new active camera.
    void set_active_camera(CameraComponent& camera);

    ///
    /// Updates the vectors and matrices of a camera.
    ///
    /// \param camera The camera to update.
    void update_camera(CameraComponent& camera);

    ///
    /// Updates the vectors and matrices of all cameras in the scene.
    void update_all_cameras();

private:
    // System overrides
    void on_component_added(CameraComponent& camera) override;

    EntityHandle _active_camera_entity;
};

}
