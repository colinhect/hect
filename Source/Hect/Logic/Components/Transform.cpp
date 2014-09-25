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
    localScale(Vector3::one()),
    globalScale(Vector3::one())
{
}

void Transform::encode(Encoder& encoder) const
{
    encoder << encodeValue("localPosition", localPosition)
        << encodeValue("localScale", localScale)
        << encodeValue("localRotation", localRotation);
}

void Transform::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    assetCache;

    if (decoder.hasMember("localPosition"))
    {
        localPosition = decoder.decodeVector3("localPosition");
    }

    if (decoder.hasMember("localScale"))
    {
        localScale = decoder.decodeVector3("localScale");
    }

    if (decoder.hasMember("localRotation"))
    {
        ObjectDecoder rotationDecoder = decoder.decodeObject("localRotation");

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

        localRotation = Quaternion::fromAxisAngle(axis, angle);
    }
}