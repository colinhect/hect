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
#include <Hect/Graphics/FrameBuffer.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct a frame buffer without size", "[FrameBuffer]")
{
    FrameBuffer frame_buffer;

    REQUIRE(frame_buffer.width() == 0);
    REQUIRE(frame_buffer.height() == 0);
}

TEST_CASE("Construct frame buffer with size", "[FrameBuffer]")
{
    FrameBuffer frame_buffer(512, 1024);

    REQUIRE(frame_buffer.width() == 512);
    REQUIRE(frame_buffer.height() == 1024);
}

TEST_CASE("Attach a 2-dimensional texture to a frame buffer of the same size", "[FrameBuffer]")
{
    Texture2 texture("Test", 512, 1024);

    FrameBuffer frame_buffer(512, 1024);

    REQUIRE(frame_buffer.attachments().size() == 0);
    frame_buffer.attach(FrameBufferSlot::Color0, texture);
    REQUIRE(frame_buffer.attachments().size() == 1);
}

TEST_CASE("Attach a 2-dimensional texture to a frame buffer of a different size", "[FrameBuffer]")
{
    Texture2 texture("Test", 256, 1024);

    FrameBuffer frame_buffer(512, 1024);

    REQUIRE(frame_buffer.attachments().size() == 0);
    REQUIRE_THROWS_AS(frame_buffer.attach(FrameBufferSlot::Color0, texture), InvalidOperation);
    REQUIRE(frame_buffer.attachments().size() == 0);
}

TEST_CASE("Attach a 2-dimensional texture to a non-empty slot of a frame buffer", "[FrameBuffer]")
{
    Texture2 texture_a("TestA", 512, 1024);
    Texture2 texture_b("TestB", 512, 1024);

    FrameBuffer frame_buffer(512, 1024);

    REQUIRE(frame_buffer.attachments().size() == 0);
    frame_buffer.attach(FrameBufferSlot::Color0, texture_a);
    REQUIRE(frame_buffer.attachments().size() == 1);
    REQUIRE_THROWS_AS(frame_buffer.attach(FrameBufferSlot::Color0, texture_b), InvalidOperation);
    REQUIRE(frame_buffer.attachments().size() == 1);
}

TEST_CASE("Attach a 3-dimensional texture to a frame buffer of the same size", "[FrameBuffer]")
{
    Texture3 texture("Test", 512, 1024, 8);

    FrameBuffer frame_buffer(512, 1024);

    REQUIRE(frame_buffer.attachments().size() == 0);
    frame_buffer.attach(FrameBufferSlot::Color0, texture);
    REQUIRE(frame_buffer.attachments().size() == 1);
}

TEST_CASE("Attach a 3-dimensional texture to a frame buffer of a different size", "[FrameBuffer]")
{
    Texture3 texture("Test", 256, 1024, 8);

    FrameBuffer frame_buffer(512, 1024);

    REQUIRE(frame_buffer.attachments().size() == 0);
    REQUIRE_THROWS_AS(frame_buffer.attach(FrameBufferSlot::Color0, texture), InvalidOperation);
    REQUIRE(frame_buffer.attachments().size() == 0);
}

TEST_CASE("Attach a 3-dimensional texture to a non-empty slot of a frame buffer", "[FrameBuffer]")
{
    Texture3 texture_a("TestA", 512, 1024, 8);
    Texture3 texture_b("TestB", 512, 1024, 8);

    FrameBuffer frame_buffer(512, 1024);

    REQUIRE(frame_buffer.attachments().size() == 0);
    frame_buffer.attach(FrameBufferSlot::Color0, texture_a);
    REQUIRE(frame_buffer.attachments().size() == 1);
    REQUIRE_THROWS_AS(frame_buffer.attach(FrameBufferSlot::Color0, texture_b), InvalidOperation);
    REQUIRE(frame_buffer.attachments().size() == 1);
}
