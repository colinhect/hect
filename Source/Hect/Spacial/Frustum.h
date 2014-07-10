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

#include "Hect/Math/Vector3.h"
#include "Hect/Spacial/AxisAlignedBox.h"
#include "Hect/Spacial/Plane.h"
#include "Hect/Spacial/Sphere.h"

namespace hect
{

///
/// The result of a frustum test.
namespace FrustumTestResult
{
enum Enum
{
    ///
    /// The tested geometry is outside of the frustum.
    Outside,

    ///
    /// The tested geometry is completely inside of the frustum.
    Inside,

    ///
    /// The tested geometry is partially inside of the frustum.
    Intersect
};
}

///
/// A pyramid with a near and far plane cutting the top and bottom.
///
/// \note Useful for check if objects are with the bounds of a camera.
class Frustum
{
public:

    ///
    /// Constructs a default frustum.
    Frustum();

    ///
    /// Constructs a frustum
    ///
    /// \param position The position.
    /// \param front The front direction.
    /// \param up The up direction.
    /// \param fieldOfView The horizontal angle.
    /// \param aspectRatio The ratio of width and height.
    /// \param nearClip The distance from the position to begin the
    /// frustum.
    /// \param farClip The distance from the position to end the frustum.
    Frustum(
        const Vector3& position,
        const Vector3& front,
        const Vector3& up,
        Angle fieldOfView,
        Real aspectRatio,
        Real nearClip,
        Real farClip);

    ///
    /// Tests an axis aligned box against the frustum.
    ///
    /// \param box The box.
    ///
    /// \returns The test result.
    FrustumTestResult::Enum testAxisAlignedBox(const AxisAlignedBox& box) const;

    ///
    /// Returns whether a sphere at a certain positin is within the frustum.
    ///
    /// \param sphere The sphere.
    /// \param position The position of the sphere.
    bool containsSphere(const Sphere& sphere, const Vector3& position) const;

private:
    Plane _planes[6];
    Vector3 _position;
};

}