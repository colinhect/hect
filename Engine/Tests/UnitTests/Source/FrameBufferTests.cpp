///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
#include <Hect/Graphics/FrameBuffer.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct a frame buffer without size", "[FrameBuffer]")
{
    FrameBuffer frameBuffer;

    REQUIRE(frameBuffer.width() == 0);
    REQUIRE(frameBuffer.height() == 0);
}

TEST_CASE("Construct frame buffer with size", "[FrameBuffer]")
{
    FrameBuffer frameBuffer(512, 1024);

    REQUIRE(frameBuffer.width() == 512);
    REQUIRE(frameBuffer.height() == 1024);
}

TEST_CASE("Attach a texture to a frame buffer of the same size", "[FrameBuffer]")
{
    Texture texture("Test", 512, 1024, PixelFormat(PixelType_Float32, 3), TextureFilter_Nearest, TextureFilter_Nearest, false, false);

    FrameBuffer frameBuffer(512, 1024);

    REQUIRE(frameBuffer.textureAttachments().size() == 0);
    frameBuffer.attachTexture(FrameBufferSlot_Color0, texture);
    REQUIRE(frameBuffer.textureAttachments().size() == 1);
}

TEST_CASE("Attach a texture to a frame buffer of a different size", "[FrameBuffer]")
{
    Texture texture("Test", 256, 1024, PixelFormat(PixelType_Float32, 3), TextureFilter_Nearest, TextureFilter_Nearest, false, false);

    FrameBuffer frameBuffer(512, 1024);

    REQUIRE(frameBuffer.textureAttachments().size() == 0);
    REQUIRE_THROWS_AS(frameBuffer.attachTexture(FrameBufferSlot_Color0, texture), InvalidOperation);
    REQUIRE(frameBuffer.textureAttachments().size() == 0);
}

TEST_CASE("Attach a texture to a non-empty slot of a frame buffer", "[FrameBuffer]")
{
    Texture textureA("TestA", 512, 1024, PixelFormat(PixelType_Float32, 3), TextureFilter_Nearest, TextureFilter_Nearest, false, false);
    Texture textureB("TestB", 512, 1024, PixelFormat(PixelType_Float32, 3), TextureFilter_Nearest, TextureFilter_Nearest, false, false);

    FrameBuffer frameBuffer(512, 1024);

    REQUIRE(frameBuffer.textureAttachments().size() == 0);
    frameBuffer.attachTexture(FrameBufferSlot_Color0, textureA);
    REQUIRE(frameBuffer.textureAttachments().size() == 1);
    REQUIRE_THROWS_AS(frameBuffer.attachTexture(FrameBufferSlot_Color0, textureB), InvalidOperation);
    REQUIRE(frameBuffer.textureAttachments().size() == 1);
}

TEST_CASE("Attach a render buffer to a frame buffer of the same size", "[FrameBuffer]")
{
    RenderBuffer renderBuffer(RenderBufferFormat_DepthComponent, 512, 1024);

    FrameBuffer frameBuffer(512, 1024);

    REQUIRE(frameBuffer.renderBufferAttachments().size() == 0);
    frameBuffer.attachRenderBuffer(FrameBufferSlot_Color0, renderBuffer);
    REQUIRE(frameBuffer.renderBufferAttachments().size() == 1);
}

TEST_CASE("Attach a render buffer to a frame buffer of a different size", "[FrameBuffer]")
{
    RenderBuffer renderBuffer(RenderBufferFormat_DepthComponent, 256, 1024);

    FrameBuffer frameBuffer(512, 1024);

    REQUIRE(frameBuffer.renderBufferAttachments().size() == 0);
    REQUIRE_THROWS_AS(frameBuffer.attachRenderBuffer(FrameBufferSlot_Color0, renderBuffer), InvalidOperation);
    REQUIRE(frameBuffer.renderBufferAttachments().size() == 0);
}

TEST_CASE("Attach a render buffer to a non-empty slot of a frame buffer", "[FrameBuffer]")
{
    RenderBuffer renderBufferA(RenderBufferFormat_DepthComponent, 512, 1024);
    RenderBuffer renderBufferB(RenderBufferFormat_DepthComponent, 512, 1024);

    FrameBuffer frameBuffer(512, 1024);

    REQUIRE(frameBuffer.renderBufferAttachments().size() == 0);
    frameBuffer.attachRenderBuffer(FrameBufferSlot_Color0, renderBufferA);
    REQUIRE(frameBuffer.renderBufferAttachments().size() == 1);
    REQUIRE_THROWS_AS(frameBuffer.attachRenderBuffer(FrameBufferSlot_Color0, renderBufferB), InvalidOperation);
    REQUIRE(frameBuffer.renderBufferAttachments().size() == 1);
}