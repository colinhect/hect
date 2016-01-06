///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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
#include "GeometryBuffer.h"

using namespace hect;

GeometryBuffer::GeometryBuffer(unsigned width, unsigned height)
{
    TextureFilter nearest = TextureFilter::Nearest;

    // Depth buffer
    _depthBuffer = Texture2("DepthBuffer", width, height, PixelFormat::R32, nearest, nearest, false, false);

    // Diffuse buffer: Red Green Blue Lighting
    _diffuseBuffer = Texture2("DiffuseBuffer", width, height, PixelFormat::Rgba32, nearest, nearest, false, false);

    // Material buffer: Roughness Metallic ?
    _materialBuffer = Texture2("MaterialBuffer", width, height, PixelFormat::Rgb32, nearest, nearest, false, false);

    // Position buffer: X Y Z
    _positionBuffer = Texture2("PositionBuffer", width, height, PixelFormat::Rgb32, nearest, nearest, false, false);

    // Normal buffer: X Y Z Depth
    _normalBuffer = Texture2("NormalBuffer", width, height, PixelFormat::Rgba16, nearest, nearest, false, false);

    // Back buffers
    _backBuffers[0] = Texture2("BackBuffer0", width, height, PixelFormat::Rgb32, nearest, nearest, false, false);
    _backBuffers[1] = Texture2("BackBuffer1", width, height, PixelFormat::Rgb32, nearest, nearest, false, false);

    // Geometry frame buffer
    _frameBuffer = FrameBuffer(width, height);
    _frameBuffer.attach(FrameBufferSlot::Depth, _depthBuffer);
    _frameBuffer.attach(FrameBufferSlot::Color0, _diffuseBuffer);
    _frameBuffer.attach(FrameBufferSlot::Color1, _materialBuffer);
    _frameBuffer.attach(FrameBufferSlot::Color2, _positionBuffer);
    _frameBuffer.attach(FrameBufferSlot::Color3, _normalBuffer);

    // Back frame buffers
    _backFrameBuffers[0] = FrameBuffer(width, height);
    _backFrameBuffers[0].attach(FrameBufferSlot::Depth, _depthBuffer);
    _backFrameBuffers[0].attach(FrameBufferSlot::Color0, _backBuffers[0]);

    _backFrameBuffers[1] = FrameBuffer(width, height);
    _backFrameBuffers[1].attach(FrameBufferSlot::Depth, _depthBuffer);
    _backFrameBuffers[1].attach(FrameBufferSlot::Color0, _backBuffers[1]);
}

FrameBuffer& GeometryBuffer::frameBuffer()
{
    return _frameBuffer;
}

FrameBuffer& GeometryBuffer::backFrameBuffer()
{
    return _backFrameBuffers[_backBufferIndex];
}

Texture2& GeometryBuffer::depthBuffer()
{
    return _depthBuffer;
}

Texture2& GeometryBuffer::diffuseBuffer()
{
    return _diffuseBuffer;
}

Texture2& GeometryBuffer::materialBuffer()
{
    return _materialBuffer;
}

Texture2& GeometryBuffer::positionBuffer()
{
    return _positionBuffer;
}

Texture2& GeometryBuffer::normalBuffer()
{
    return _normalBuffer;
}

void GeometryBuffer::swapBackBuffers()
{
    _backBufferIndex = (_backBufferIndex + 1) % 2;
}

Texture2& GeometryBuffer::backBuffer()
{
    return _backBuffers[_backBufferIndex];
}

Texture2& GeometryBuffer::lastBackBuffer()
{
    return _backBuffers[(_backBufferIndex + 1) % 2];
}
