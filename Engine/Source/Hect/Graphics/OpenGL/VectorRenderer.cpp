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

NVGcontext* _nvgContext = nullptr;
std::unordered_map<const Font*, int> _fontToId;

NVGcolor convertColor(const Color& color)
{
    NVGcolor result;
    result.r = static_cast<float>(color[0]);
    result.g = static_cast<float>(color[1]);
    result.b = static_cast<float>(color[2]);
    result.a = static_cast<float>(color[3]);
    return result;
}

}

void VectorRenderer::Frame::pushState()
{
    assert(_nvgContext);
    nvgSave(_nvgContext);
}

void VectorRenderer::Frame::popState()
{
    assert(_nvgContext);
    nvgRestore(_nvgContext);
}

void VectorRenderer::Frame::beginPath()
{
    assert(_nvgContext);
    nvgBeginPath(_nvgContext);
}

void VectorRenderer::Frame::setColor(const Color& color)
{
    assert(_nvgContext);
    nvgFillColor(_nvgContext, convertColor(color));
}

void VectorRenderer::Frame::fillPath()
{
    assert(_nvgContext);
    nvgFill(_nvgContext);
}

void VectorRenderer::Frame::rectangle(const Vector2& position, const Vector2& dimensions)
{
    assert(_nvgContext);
    nvgRect(_nvgContext, static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(dimensions.x), static_cast<float>(dimensions.y));
}

void VectorRenderer::Frame::setClipping(const Vector2& position, const Vector2& dimensions)
{
    assert(_nvgContext);
    nvgScissor(_nvgContext, static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(dimensions.x), static_cast<float>(dimensions.y));
}

void VectorRenderer::Frame::setFont(const Font& font, double size)
{
    assert(_nvgContext);

    auto it = _fontToId.find(&font);
    if (it == _fontToId.end())
    {
        int id = nvgCreateFontMem(_nvgContext, font.name().c_str(), const_cast<unsigned char*>(&font._data[0]), static_cast<int>(font._data.size()), 0);
        _fontToId[&font] = id;

        HECT_TRACE(format("Built font '%s'", font.name().c_str()));
    }

    // Set the font
    nvgFontSize(_nvgContext, static_cast<float>(size));
    nvgFontFaceId(_nvgContext, _fontToId[&font]);
}

void VectorRenderer::Frame::renderText(const std::string& text, const Vector2& position, const Vector2& dimensions, HorizontalAlign horizontalAlign, VerticalAlign verticalAlign)
{
    assert(_nvgContext);

    // Compute the text bounds
    float measuredBounds[4];
    nvgTextBounds(_nvgContext, 0, 0, text.c_str(), nullptr, measuredBounds);
    Vector2 textPosition(measuredBounds[0], measuredBounds[1]);
    Vector2 textDimensions(measuredBounds[2] - measuredBounds[0], measuredBounds[3] - measuredBounds[1]);
    Vector2 actual(position - textPosition);

    // Align horizontally
    switch (horizontalAlign)
    {
    case HorizontalAlign::Left:
        break;
    case HorizontalAlign::Center:
        actual.x += dimensions.x / 2 - textDimensions.x / 2;
        break;
    case HorizontalAlign::Right:
        actual.x += dimensions.x - textDimensions.x;
        break;
    }

    // Align vertically
    switch (verticalAlign)
    {
    case VerticalAlign::Bottom:
        actual.y += dimensions.y - textDimensions.y;
        break;
    case VerticalAlign::Center:
        actual.y += dimensions.y / 2 - textDimensions.y / 2;
        break;
    case VerticalAlign::Top:
        break;
    }

    // Render the text
    setClipping(position, dimensions);
    nvgText(_nvgContext, static_cast<float>(actual.x), static_cast<float>(actual.y), text.c_str(), nullptr);
    nvgResetScissor(_nvgContext);
}

void VectorRenderer::initialize()
{
    // Initialize NanoVG
#ifdef HECT_DEBUG_BUILD
    _nvgContext = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#else
    _nvgContext = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#endif
    if (!_nvgContext)
    {
        throw FatalError("Failed to initialize NanoVG");
    }
}

void VectorRenderer::shutdown()
{
    assert(_nvgContext);
    nvgDeleteGL3(_nvgContext);
}

void VectorRenderer::onBeginFrame(RenderTarget& target)
{
    assert(_nvgContext);
    nvgBeginFrame(_nvgContext, target.width(), target.height(), 1);
}

void VectorRenderer::onEndFrame()
{
    assert(_nvgContext);
    nvgEndFrame(_nvgContext);
}

#endif
