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
#pragma once

#include "Hect/Logic/Scene.h"
#include "Hect/Spacial/Frustum.h"

namespace hect
{

class Transform;

///
/// A camera component.
///
/// \component
class Camera :
    public Component<Camera>
{
public:

    ///
    /// The horizontal field of view.
    ///
    /// \property
    Angle fieldOfView{ Angle::fromDegrees(90) };

    ///
    /// The aspect ratio.
    ///
    /// \property
    Real aspectRatio{ 1 };

    ///
    /// The near clip distance.
    ///
    /// \property
    Real nearClip{ Real(0.1) };

    ///
    /// The far clip distance.
    ///
    /// \property
    Real farClip{ Real(10000) };

    ///
    /// The front vector.
    Vector3 front { -Vector3::unitZ() };

    ///
    /// The up vector.
    Vector3 up { Vector3::unitY() };

    ///
    /// The right vector.
    Vector3 right { Vector3::unitX() };

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