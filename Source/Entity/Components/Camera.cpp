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
#include "Hect.h"

using namespace hect;

Camera::Camera() :
    _fieldOfView(Angle<>::fromDegrees(90)),
    _aspectRatio(1),
    _nearClip(0.1),
    _farClip(1000),
    _front(-Vector3<>::unitZ()),
    _up(Vector3<>::unitY()),
    _right(_front.cross(_up).normalized())
{
}

void Camera::transformTo(const Transform& transform)
{
    const Vector3<>& position = transform.position();
    const Quaternion<>& rotation = transform.rotation();

    _front = (rotation * -Vector3<>::unitZ()).normalized();
    _up = (rotation * Vector3<>::unitY()).normalized();
    _right = _front.cross(_up).normalized();

    _position = position;

    _viewMatrix = Matrix4<>::createView(position, _front, _up);
    _projectionMatrix = Matrix4<>::createPerspective(_fieldOfView, _aspectRatio, _nearClip, _farClip);

    _frustum = Frustum<>(position, _front, _up, _fieldOfView, _aspectRatio, _nearClip, _farClip);
}

const Vector3<>& Camera::front() const
{
    return _front;
}

const Vector3<>& Camera::up() const
{
    return _up;
}

const Vector3<>& Camera::right() const
{
    return _right;
}

const Vector3<>& Camera::position() const
{
    return _position;
}

const Matrix4<>& Camera::viewMatrix() const
{
    return _viewMatrix;
}

const Matrix4<>& Camera::projectionMatrix() const
{
    return _projectionMatrix;
}

Angle<> Camera::fieldOfView() const
{
    return _fieldOfView;
}

void Camera::setFieldOfView(Angle<> fieldOfView)
{
    _fieldOfView = fieldOfView;
}

double Camera::aspectRatio() const
{
    return _aspectRatio;
}

void Camera::setAspectRatio(double aspectRatio)
{
    _aspectRatio = aspectRatio;
}

double Camera::nearClip() const
{
    return _nearClip;
}

void Camera::setNearClip(double nearClip)
{
    _nearClip = nearClip;
}

double Camera::farClip() const
{
    return _farClip;
}

void Camera::setFarClip(double farClip)
{
    _farClip = farClip;
}

const Frustum<>& Camera::frustum() const
{
    return _frustum;
}

void CameraSerializer::save(const Camera& camera, DataWriter& writer) const
{
    writer.writeDouble("fieldOfView", camera.fieldOfView().degrees());
    writer.writeDouble("aspectRatio", camera.aspectRatio());
    writer.writeDouble("nearClip", camera.nearClip());
    writer.writeDouble("farClip", camera.farClip());
}

void CameraSerializer::load(Camera& camera, DataReader& reader, AssetCache& assetCache) const
{
    assetCache;

    if (reader.hasMember("fieldOfView"))
    {
        double degrees = reader.readDouble("fieldOfView");
        camera.setFieldOfView(Angle<>::fromDegrees(degrees));
    }

    if (reader.hasMember("aspectRatio"))
    {
        camera.setAspectRatio(reader.readDouble("aspectRatio"));
    }

    if (reader.hasMember("nearClip"))
    {
        camera.setNearClip(reader.readDouble("nearClip"));
    }

    if (reader.hasMember("farClip"))
    {
        camera.setFarClip(reader.readDouble("farClip"));
    }
}