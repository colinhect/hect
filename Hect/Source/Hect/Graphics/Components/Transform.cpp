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
#include "Transform.h"

using namespace hect;

Transform::Transform() :
    _dirtyBits(0),
    _scale(Vector3::one()),
    _globalScale(Vector3::one())
{
}

void Transform::buildMatrix(Matrix4& matrix) const
{
    matrix = Matrix4();

    if (_dirtyBits & PositionBit)
    {
        matrix.translate(_globalPosition);
    }

    if (_dirtyBits & ScaleBit)
    {
        matrix.scale(_globalScale);
    }

    if (_dirtyBits & RotationBit)
    {
        matrix.rotate(_globalRotation);
    }
}

void Transform::translate(const Vector3& translation)
{
    _position += translation;
    _dirtyBits |= PositionBit;
}

void Transform::scale(const Vector3& scale)
{
    _scale *= scale;
    _dirtyBits |= ScaleBit;
}

void Transform::rotate(const Quaternion& rotation)
{
    _rotation *= rotation;
    _rotation.normalize();
    _dirtyBits |= RotationBit;
}

void Transform::rotate(const Vector3& axis, Angle angle)
{
    rotate(Quaternion::fromAxisAngle(axis, angle));
}

const Vector3& Transform::position() const
{
    return _position;
}

void Transform::setPosition(const Vector3& position)
{
    _position = position;
    _dirtyBits |= PositionBit;
}

const Vector3& Transform::scale() const
{
    return _scale;
}

void Transform::setScale(const Vector3& scale)
{
    _scale = scale;
    _dirtyBits |= ScaleBit;
}

const Quaternion& Transform::rotation() const
{
    return _rotation;
}

void Transform::setRotation(const Quaternion& rotation)
{
    _rotation = rotation;
    _dirtyBits |= RotationBit;
}

void Transform::transformBy(const Transform& transform)
{
    _position = transform._rotation * _position;
    _position += transform._position;
    _scale *= transform._scale;
    _rotation = transform._rotation * _rotation;
    _dirtyBits = PositionBit | ScaleBit | RotationBit;
}

void Transform::updateGlobalTransform()
{
    _globalPosition = _position;
    _globalScale = _scale;
    _globalRotation = _rotation;
}

void Transform::updateGlobalTransform(const Transform& parentTransform)
{
    _globalPosition = parentTransform._globalRotation * _position;
    _globalPosition += parentTransform._globalPosition;
    _globalScale = parentTransform._globalScale * _scale;
    _globalRotation = parentTransform._globalRotation * _rotation;
    _dirtyBits = PositionBit | ScaleBit | RotationBit;
}

void Transform::encode(ObjectEncoder& encoder) const
{
    Vector3 axis;
    Angle angle;
    _rotation.toAxisAngle(axis, angle);

    encoder.encodeVector3("position", _position);
    encoder.encodeVector3("scale", _scale);

    ObjectEncoder rotationEncoder = encoder.encodeObject("rotation");
    rotationEncoder.encodeVector3("axis", axis);
    rotationEncoder.encodeReal("angle", angle.degrees());
}

void Transform::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    assetCache;

    if (decoder.hasMember("position"))
    {
        setPosition(decoder.decodeVector3("position"));
    }

    if (decoder.hasMember("scale"))
    {
        setScale(decoder.decodeVector3("scale"));
    }

    if (decoder.hasMember("rotation"))
    {
        ObjectDecoder rotationDecoder = decoder.decodeObject("rotation");

        Vector3 axis;
        Angle angle;

        if (rotationDecoder.hasMember("axis"))
        {
            axis = rotationDecoder.decodeVector3("axis");
        }

        if (rotationDecoder.hasMember("angle"))
        {
            Real degrees = rotationDecoder.decodeReal("angle");
            angle = Angle::fromDegrees(degrees);
        }

        setRotation(Quaternion::fromAxisAngle(axis, angle));
    }
}