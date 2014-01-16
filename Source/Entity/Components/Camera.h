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
    /// Transforms the camera to a transformation.
    ///
    /// \param transform The transformation to transform to.
    void transformTo(const Transform& transform);

    ///
    /// Returns the front vector.
    const Vector3<>& front() const;

    ///
    /// Returns the up vector.
    const Vector3<>& up() const;

    ///
    /// Returns the right vector.
    const Vector3<>& right() const;

    ///
    /// Returns the position.
    const Vector3<>& position() const;

    ///
    /// Returns the view matrix.
    const Matrix4<>& viewMatrix() const;

    ///
    /// Returns the projection matrix.
    const Matrix4<>& projectionMatrix() const;

    ///
    /// Returns the horizontal field of view.
    Angle<> fieldOfView() const;

    ///
    /// Sets the horizontal field of view.
    ///
    /// \param fieldOfView The new horizontal field of view.
    void setFieldOfView(Angle<> fieldOfView);

    ///
    /// Returns the aspect ratio.
    double aspectRatio() const;

    ///
    /// Sets the aspect ratio.
    ///
    /// \param aspectRatio The new aspect ratio.
    void setAspectRatio(double aspectRatio);

    ///
    /// Returns the near clip distance.
    double nearClip() const;

    ///
    /// Sets the near clip distance.
    ///
    /// \param nearClip The new near clip distance.
    void setNearClip(double nearClip);

    ///
    /// Returns the far clip distance.
    double farClip() const;

    ///
    /// Sets the far clip distance.
    ///
    /// \param farClip The new far clip distance.
    void setFarClip(double farClip);

    ///
    /// Returns the frustum.
    const Frustum<>& frustum() const;

private:
    Angle<> _fieldOfView;

    double _aspectRatio;
    double _nearClip;
    double _farClip;

    Vector3<> _front;
    Vector3<> _up;
    Vector3<> _right;

    Vector3<> _position;

    Matrix4<> _viewMatrix;
    Matrix4<> _projectionMatrix;

    Frustum<> _frustum;
};

///
/// Serializer for Camera.
class CameraSerializer :
    public ComponentSerializer<Camera>
{
public:

    ///
    /// See BaseComponentSerializer::save()
    void save(const Camera& camera, DataWriter& writer) const;

    ///
    /// See BaseComponentSerializer::load()
    void load(Camera& camera, DataReader& reader, AssetCache& assetCache) const;
};

}