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
#include "DirectionalLight.h"

using namespace hect;

DirectionalLight::DirectionalLight() :
    _direction(Vector3::unitX()),
    _color(Vector3::one())
{
}

const Vector3& DirectionalLight::direction() const
{
    return _direction;
}

void DirectionalLight::setDirection(const Vector3& direction)
{
    _direction = direction.normalized();
}

const Vector3& DirectionalLight::color() const
{
    return _color;
}

void DirectionalLight::setColor(const Vector3& color)
{
    _color = color;
}

void DirectionalLight::encode(ObjectEncoder& encoder) const
{
    encoder.encodeVector3("direction", direction());
    encoder.encodeVector3("color", color());
}

void DirectionalLight::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    assetCache;

    if (decoder.hasMember("direction"))
    {
        setDirection(decoder.decodeVector3("direction"));
    }

    if (decoder.hasMember("color"))
    {
        setColor(decoder.decodeVector3("color"));
    }
}