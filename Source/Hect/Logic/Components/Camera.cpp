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
#include "Camera.h"

#include "Hect/Logic/Components/Transform.h"

using namespace hect;

Camera::Camera() :
    front(-Vector3::unitZ()),
    up(Vector3::unitY()),
    right(front.cross(up).normalized()),
    fieldOfView(Angle::fromDegrees(90)),
    aspectRatio(1),
    nearClip((Real)0.1),
    farClip(1000)
{
}

void Camera::encode(Encoder& encoder) const
{
    encoder << encodeValue("fieldOfView", fieldOfView)
        << encodeValue("aspectRatio", aspectRatio)
        << encodeValue("nearClip", nearClip)
        << encodeValue("farClip", farClip);
}

void Camera::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    assetCache;

    if (decoder.hasMember("fieldOfView"))
    {
        Real degrees = decoder.decodeReal("fieldOfView");
        fieldOfView = Angle::fromDegrees(degrees);
    }

    if (decoder.hasMember("aspectRatio"))
    {
        aspectRatio = decoder.decodeReal("aspectRatio");
    }

    if (decoder.hasMember("nearClip"))
    {
        nearClip = decoder.decodeReal("nearClip");
    }

    if (decoder.hasMember("farClip"))
    {
        farClip = decoder.decodeReal("farClip");
    }
}
