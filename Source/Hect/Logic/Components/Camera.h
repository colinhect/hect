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

#include "Hect/Logic/Component.h"
#include "Hect/Spacial/Frustum.h"

namespace hect
{

class Transform;

///
/// A camera component.
class Camera :
    public Component<Camera>
{
public:
    Camera();

    ///
    /// The front vector.
    Vector3 front;

    ///
    /// The up vector.
    Vector3 up;

    ///
    /// The right vector.
    Vector3 right;

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
    /// The horizontal field of view.
    Angle fieldOfView;

    ///
    /// The aspect ratio.
    Real aspectRatio;

    ///
    /// The near clip distance.
    Real nearClip;

    ///
    /// The far clip distance.
    Real farClip;

    ///
    /// The frustum.
    Frustum frustum;

    void encode(Encoder& encoder) const;
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);
};

}