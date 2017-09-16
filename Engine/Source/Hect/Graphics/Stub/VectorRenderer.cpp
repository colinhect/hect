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
#include "Hect/Graphics/VectorRenderer.h"

#include "Hect/Core/Configuration.h"

#ifndef HECT_RENDERER_OPENGL

using namespace hect;

void VectorRenderer::Frame::push_state()
{
}

void VectorRenderer::Frame::pop_state()
{
}

void VectorRenderer::Frame::translate(Vector2 translation)
{
    (void)translation;
}

void VectorRenderer::Frame::begin_path()
{
}

void VectorRenderer::Frame::set_fill_color(Color color)
{
    (void)color;
}

void VectorRenderer::Frame::set_fill_gradient(Vector2 start_position, Vector2 end_position, Color start_color, Color end_color)
{
    (void)start_position;
    (void)end_position;
    (void)start_color;
    (void)end_color;
}

void VectorRenderer::Frame::fill()
{
}

void VectorRenderer::Frame::set_stroke_color(Color color)
{
    (void)color;
}

void VectorRenderer::Frame::stroke()
{
}

void VectorRenderer::Frame::rectangle(Rectangle rectangle)
{
    (void)rectangle;
}

void VectorRenderer::Frame::set_clipping(Rectangle clipping)
{
    (void)clipping;
}

void VectorRenderer::Frame::set_font(const Font& font, double size)
{
    (void)font;
    (void)size;
}

void VectorRenderer::Frame::render_text(const std::string& text)
{
    (void)text;
}

Vector2 VectorRenderer::measure_text_dimensions(const std::string& text, const Font& font, double size)
{
    (void)text;
    (void)font;
    (void)size;

    return Vector2();
}

void VectorRenderer::initialize()
{
}

void VectorRenderer::shutdown()
{
}

void VectorRenderer::on_begin_frame(RenderTarget& target)
{
    (void)target;
}

void VectorRenderer::on_end_frame()
{
}

#endif
