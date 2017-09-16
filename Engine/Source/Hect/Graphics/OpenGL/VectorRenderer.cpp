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

#ifdef HECT_RENDERER_OPENGL

#define NANOVG_GLEW
#define NANOVG_GL3_IMPLEMENTATION

#include <cassert>
#include <GL/glew.h>
#include <nanovg.h>
#include <nanovg_gl.h>
#include <unordered_map>

using namespace hect;

namespace
{

NVGcontext* _nvg_context = nullptr;
std::unordered_map<const Font*, int> _font_to_id;

NVGcolor convert_color(Color color)
{
    NVGcolor result;
    result.r = static_cast<float>(color[0]);
    result.g = static_cast<float>(color[1]);
    result.b = static_cast<float>(color[2]);
    result.a = static_cast<float>(color[3]);
    return result;
}

}

void VectorRenderer::Frame::push_state()
{
    assert(_nvg_context);
    nvgSave(_nvg_context);
}

void VectorRenderer::Frame::pop_state()
{
    assert(_nvg_context);
    nvgRestore(_nvg_context);
}

void VectorRenderer::Frame::begin_path()
{
    assert(_nvg_context);
    nvgBeginPath(_nvg_context);
}

void VectorRenderer::Frame::translate(Vector2 translation)
{
    assert(_nvg_context);
    nvgTranslate(_nvg_context, static_cast<float>(translation.x), static_cast<float>(translation.y));
}

void VectorRenderer::Frame::set_fill_color(Color color)
{
    assert(_nvg_context);
    nvgFillColor(_nvg_context, convert_color(color));
}

void VectorRenderer::Frame::set_fill_gradient(Vector2 start_position, Vector2 end_position, Color start_color, Color end_color)
{
    assert(_nvg_context);

    // Create the gradient
    float sx = static_cast<float>(start_position.x);
    float sy = static_cast<float>(start_position.y);
    float ex = static_cast<float>(end_position.x);
    float ey = static_cast<float>(end_position.y);
    NVGpaint gradient = nvgLinearGradient(_nvg_context, sx, sy, ex, ey, convert_color(start_color), convert_color(end_color));

    // Fill the gradient
    nvgFillPaint(_nvg_context, gradient);
}

void VectorRenderer::Frame::fill()
{
    assert(_nvg_context);
    nvgFill(_nvg_context);
}

void VectorRenderer::Frame::set_stroke_color(Color color)
{
    assert(_nvg_context);
    nvgStrokeColor(_nvg_context, convert_color(color));
}

void VectorRenderer::Frame::stroke()
{
    assert(_nvg_context);
    nvgStroke(_nvg_context);
}

void VectorRenderer::Frame::rectangle(Rectangle rectangle)
{
    assert(_nvg_context);
    Vector2 position = rectangle.minimum();
    Vector2 dimensions = rectangle.size();
    nvgRect(_nvg_context, static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(dimensions.x), static_cast<float>(dimensions.y));
}

void VectorRenderer::Frame::set_clipping(Rectangle clipping)
{
    assert(_nvg_context);
    Vector2 position = clipping.minimum();
    Vector2 dimensions = clipping.size();
    nvgScissor(_nvg_context, static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(dimensions.x), static_cast<float>(dimensions.y));
}

void VectorRenderer::Frame::set_font(const Font& font, double size)
{
    assert(_nvg_context);
    _renderer.set_font(font, size);
}

void VectorRenderer::Frame::render_text(const std::string& text)
{
    assert(_nvg_context);

    // Compute the text bounds
    float measured_bounds[4];
    nvgTextBounds(_nvg_context, 0, 0, text.data(), nullptr, measured_bounds);
    Vector2 position(-measured_bounds[0], -measured_bounds[1]);

    // Render the text
    nvgText(_nvg_context, static_cast<float>(position.x), static_cast<float>(position.y), text.data(), nullptr);
}

Vector2 VectorRenderer::measure_text_dimensions(const std::string& text, const Font& font, double size)
{
    assert(_nvg_context);

    set_font(font, size);

    // Compute the text bounds
    float measured_bounds[4];
    nvgTextBounds(_nvg_context, 0, 0, text.data(), nullptr, measured_bounds);
    Vector2 dimensions(measured_bounds[2] - measured_bounds[0], measured_bounds[3] - measured_bounds[1]);

    return dimensions;
}

void VectorRenderer::initialize()
{
    // Initialize NanoVG
#ifdef HECT_DEBUG_BUILD
    _nvg_context = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#else
    _nvg_context = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#endif
    if (!_nvg_context)
    {
        throw FatalError("Failed to initialize NanoVG");
    }
}

void VectorRenderer::shutdown()
{
    assert(_nvg_context);
    nvgDeleteGL3(_nvg_context);
}

void VectorRenderer::on_begin_frame(RenderTarget& target)
{
    assert(_nvg_context);
    nvgBeginFrame(_nvg_context, target.width(), target.height(), 1);
}

void VectorRenderer::on_end_frame()
{
    assert(_nvg_context);
    nvgEndFrame(_nvg_context);
}

void VectorRenderer::set_font(const Font& font, double size)
{
    assert(_nvg_context);

    auto it = _font_to_id.find(&font);
    if (it == _font_to_id.end())
    {
        int id = nvgCreateFontMem(_nvg_context, font.name().data(), const_cast<unsigned char*>(&font._data[0]), static_cast<int>(font._data.size()), 0);
        _font_to_id[&font] = id;

        HECT_TRACE(format("Built font '%s'", font.name().data()));
    }

    // Set the font
    nvgFontSize(_nvg_context, static_cast<float>(size));
    nvgFontFaceId(_nvg_context, _font_to_id[&font]);
}

#endif
