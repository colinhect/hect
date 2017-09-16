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
    _depth_buffer = Texture2("DepthBuffer", width, height);
    _depth_buffer.set_pixel_format(PixelFormat::R32);
    _depth_buffer.set_min_filter(TextureFilter::Nearest);
    _depth_buffer.set_mag_filter(TextureFilter::Nearest);
    _depth_buffer.set_mipmapped(false);
    _depth_buffer.set_wrapped(false);

    // Diffuse buffer: Red Green Blue Lighting
    _diffuse_buffer = Texture2("DiffuseBuffer", width, height);
    _diffuse_buffer.set_pixel_format(PixelFormat::Rgba32);
    _diffuse_buffer.set_min_filter(TextureFilter::Nearest);
    _diffuse_buffer.set_mag_filter(TextureFilter::Nearest);
    _diffuse_buffer.set_mipmapped(false);
    _diffuse_buffer.set_wrapped(false);

    // Material buffer: Roughness Metallic ?
    _material_buffer = Texture2("MaterialBuffer", width, height);
    _material_buffer.set_pixel_format(PixelFormat::Rgb32);
    _material_buffer.set_min_filter(TextureFilter::Nearest);
    _material_buffer.set_mag_filter(TextureFilter::Nearest);
    _material_buffer.set_mipmapped(false);
    _material_buffer.set_wrapped(false);

    // Position buffer: X Y Z
    _position_buffer = Texture2("PositionBuffer", width, height);
    _position_buffer.set_pixel_format(PixelFormat::Rgb32);
    _position_buffer.set_min_filter(TextureFilter::Nearest);
    _position_buffer.set_mag_filter(TextureFilter::Nearest);
    _position_buffer.set_mipmapped(false);
    _position_buffer.set_wrapped(false);

    // Normal buffer: X Y Z Depth
    _normal_buffer = Texture2("NormalBuffer", width, height);
    _normal_buffer.set_pixel_format(PixelFormat::Rgba16);
    _normal_buffer.set_min_filter(TextureFilter::Nearest);
    _normal_buffer.set_mag_filter(TextureFilter::Nearest);
    _normal_buffer.set_mipmapped(false);
    _normal_buffer.set_wrapped(false);

    // Back buffers
    _back_buffers[0] = Texture2("BackBuffer0", width, height);
    _back_buffers[0].set_pixel_format(PixelFormat::Rgb32);
    _back_buffers[0].set_min_filter(TextureFilter::Nearest);
    _back_buffers[0].set_mag_filter(TextureFilter::Nearest);
    _back_buffers[0].set_mipmapped(false);
    _back_buffers[0].set_wrapped(false);
    _back_buffers[1] = Texture2("BackBuffer1", width, height);
    _back_buffers[1].set_pixel_format(PixelFormat::Rgb32);
    _back_buffers[1].set_min_filter(TextureFilter::Nearest);
    _back_buffers[1].set_mag_filter(TextureFilter::Nearest);
    _back_buffers[1].set_mipmapped(false);
    _back_buffers[1].set_wrapped(false);

    // Geometry frame buffer
    _frame_buffer = FrameBuffer(width, height);
    _frame_buffer.attach(FrameBufferSlot::Depth, _depth_buffer);
    _frame_buffer.attach(FrameBufferSlot::Color0, _diffuse_buffer);
    _frame_buffer.attach(FrameBufferSlot::Color1, _material_buffer);
    _frame_buffer.attach(FrameBufferSlot::Color2, _position_buffer);
    _frame_buffer.attach(FrameBufferSlot::Color3, _normal_buffer);

    // Back frame buffers
    _back_frame_buffers[0] = FrameBuffer(width, height);
    _back_frame_buffers[0].attach(FrameBufferSlot::Depth, _depth_buffer);
    _back_frame_buffers[0].attach(FrameBufferSlot::Color0, _back_buffers[0]);
    _back_frame_buffers[1] = FrameBuffer(width, height);
    _back_frame_buffers[1].attach(FrameBufferSlot::Depth, _depth_buffer);
    _back_frame_buffers[1].attach(FrameBufferSlot::Color0, _back_buffers[1]);
}

FrameBuffer& GeometryBuffer::frame_buffer()
{
    return _frame_buffer;
}

FrameBuffer& GeometryBuffer::back_frame_buffer()
{
    return _back_frame_buffers[_back_buffer_index];
}

Texture2& GeometryBuffer::depth_buffer()
{
    return _depth_buffer;
}

Texture2& GeometryBuffer::diffuse_buffer()
{
    return _diffuse_buffer;
}

Texture2& GeometryBuffer::material_buffer()
{
    return _material_buffer;
}

Texture2& GeometryBuffer::position_buffer()
{
    return _position_buffer;
}

Texture2& GeometryBuffer::normal_buffer()
{
    return _normal_buffer;
}

void GeometryBuffer::swap_back_buffers()
{
    _back_buffer_index = (_back_buffer_index + 1) % 2;
}

Texture2& GeometryBuffer::back_buffer()
{
    return _back_buffers[_back_buffer_index];
}

Texture2& GeometryBuffer::last_back_buffer()
{
    return _back_buffers[(_back_buffer_index + 1) % 2];
}
