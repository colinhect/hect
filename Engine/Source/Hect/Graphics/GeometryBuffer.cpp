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
    // Depth buffer
    _depthBuffer = Texture2("DepthBuffer", width, height);
    _depthBuffer.setPixelFormat(PixelFormat::R32);
    _depthBuffer.setMinFilter(TextureFilter::Nearest);
    _depthBuffer.setMagFilter(TextureFilter::Nearest);
    _depthBuffer.setMipmapped(false);
    _depthBuffer.setWrapped(false);

    // Diffuse buffer: Red Green Blue Lighting
    _diffuseBuffer = Texture2("DiffuseBuffer", width, height);
    _diffuseBuffer.setPixelFormat(PixelFormat::Rgba32);
    _diffuseBuffer.setMinFilter(TextureFilter::Nearest);
    _diffuseBuffer.setMagFilter(TextureFilter::Nearest);
    _diffuseBuffer.setMipmapped(false);
    _diffuseBuffer.setWrapped(false);

    // Material buffer: Roughness Metallic ?
    _materialBuffer = Texture2("MaterialBuffer", width, height);
    _materialBuffer.setPixelFormat(PixelFormat::Rgb32);
    _materialBuffer.setMinFilter(TextureFilter::Nearest);
    _materialBuffer.setMagFilter(TextureFilter::Nearest);
    _materialBuffer.setMipmapped(false);
    _materialBuffer.setWrapped(false);

    // Position buffer: X Y Z
    _positionBuffer = Texture2("PositionBuffer", width, height);
    _positionBuffer.setPixelFormat(PixelFormat::Rgb32);
    _positionBuffer.setMinFilter(TextureFilter::Nearest);
    _positionBuffer.setMagFilter(TextureFilter::Nearest);
    _positionBuffer.setMipmapped(false);
    _positionBuffer.setWrapped(false);

    // Normal buffer: X Y Z Depth
    _normalBuffer = Texture2("NormalBuffer", width, height);
    _normalBuffer.setPixelFormat(PixelFormat::Rgba16);
    _normalBuffer.setMinFilter(TextureFilter::Nearest);
    _normalBuffer.setMagFilter(TextureFilter::Nearest);
    _normalBuffer.setMipmapped(false);
    _normalBuffer.setWrapped(false);

    // Back buffers
    _backBuffers[0] = Texture2("BackBuffer0", width, height);
    _backBuffers[0].setPixelFormat(PixelFormat::Rgb32);
    _backBuffers[0].setMinFilter(TextureFilter::Nearest);
    _backBuffers[0].setMagFilter(TextureFilter::Nearest);
    _backBuffers[0].setMipmapped(false);
    _backBuffers[0].setWrapped(false);
    _backBuffers[1] = Texture2("BackBuffer1", width, height);
    _backBuffers[1].setPixelFormat(PixelFormat::Rgb32);
    _backBuffers[1].setMinFilter(TextureFilter::Nearest);
    _backBuffers[1].setMagFilter(TextureFilter::Nearest);
    _backBuffers[1].setMipmapped(false);
    _backBuffers[1].setWrapped(false);

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
