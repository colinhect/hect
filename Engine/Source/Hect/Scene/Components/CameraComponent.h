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
#include "Hect/Graphics/Color.h"
#include "Hect/Math/Degrees.h"
#include "Hect/Math/Frustum.h"
#include "Hect/Scene/Scene.h"

namespace hect
{

class TransformComponent;

///
/// A camera.
///
/// \component
class HECT_EXPORT CameraComponent :
    public Component<CameraComponent>
{
public:

    ///
    /// The horizontal field of view.
    ///
    /// \property
    Angle fieldOfView { Degrees(90) };

    ///
    /// The aspect ratio.
    ///
    /// \property
    double aspectRatio { 1.0 };

    ///
    /// The near clip distance.
    ///
    /// \property
    double nearClip { 0.1 };

    ///
    /// The far clip distance.
    ///
    /// \property
    double farClip { 10000 };

    ///
    /// The exposure.
    ///
    /// \property
    double exposure { 1.0 };

    ///
    /// The gamma.
    ///
    /// \property
    double gamma { 2.2 };

    ///
    /// The color used to clear the render target.
    ///
    /// \property
    Color clearColor { Color::Zero };

    ///
    /// The front vector.
    Vector3 front { Vector3::UnitY };

    ///
    /// The up vector.
    Vector3 up { Vector3::UnitZ };

    ///
    /// The right vector.
    Vector3 right { Vector3::UnitX };

    ///
    /// The position.
    Vector3 position;

    ///
    /// The view matrix.
    Matrix4 viewMatrix;

    ///
    /// The projection matrix.
    Matrix4 projectionMatrix;

    ///
    /// The frustum.
    Frustum frustum;
};

}
