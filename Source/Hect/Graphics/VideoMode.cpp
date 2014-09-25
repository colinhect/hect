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
#include "VideoMode.h"

using namespace hect;

VideoMode::VideoMode() :
    _width(800),
    _height(600),
    _fullscreen(false)
{
}

VideoMode::VideoMode(unsigned width, unsigned height, bool fullscreen) :
    _width(width),
    _height(height),
    _fullscreen(fullscreen)
{
}

unsigned VideoMode::width() const
{
    return _width;
}

unsigned VideoMode::height() const
{
    return _height;
}

bool VideoMode::isFullscreen() const
{
    return _fullscreen;
}

void VideoMode::encode(Encoder& encoder) const
{
    encoder << encodeValue("width", _width)
        << encodeValue("height", _height)
        << encodeValue("fullscreen", _fullscreen);
}

void VideoMode::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    assetCache;

    if (decoder.hasMember("width"))
    {
        _width = decoder.decodeUInt32("width");
    }

    if (decoder.hasMember("height"))
    {
        _height = decoder.decodeUInt32("height");
    }

    if (decoder.hasMember("fullscreen"))
    {
        _fullscreen = decoder.decodeBool("fullscreen");
    }
}