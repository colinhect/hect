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
#include "NanoVGVectorRenderer.h"

#include "Hect/Timing/Timer.h"

#ifdef HECT_RENDERER_OPENGL

#define NANOVG_GLEW
#define NANOVG_GL3_IMPLEMENTATION

#include <cassert>
#include <GL/glew.h>
#include <nanovg_gl.h>

using namespace hect;

NanoVGVectorRenderer::NanoVGVectorRenderer(Renderer& renderer) :
    _renderer(renderer)
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

NanoVGVectorRenderer::~NanoVGVectorRenderer()
{
    nvgDeleteGL3(_nvgContext);
}

void NanoVGVectorRenderer::beginFrame(RenderTarget& target)
{
    _renderer.beginFrame();
    _renderer.setTarget(target);

    assert(_nvgContext);
    nvgBeginFrame(_nvgContext, target.width(), target.height(), 1);
}

void NanoVGVectorRenderer::endFrame()
{
    nvgEndFrame(_nvgContext);

    _renderer.endFrame();
}

void NanoVGVectorRenderer::pushState()
{
    nvgSave(_nvgContext);
}

void NanoVGVectorRenderer::popState()
{
    nvgRestore(_nvgContext);
}

void NanoVGVectorRenderer::beginPath()
{
    nvgBeginPath(_nvgContext);
}

void NanoVGVectorRenderer::setFillColor(const Color& color)
{
    nvgFillColor(_nvgContext, convertColor(color));
}

void NanoVGVectorRenderer::fill()
{
    nvgFill(_nvgContext);
}

void NanoVGVectorRenderer::rectangle(const Rectangle& bounds)
{
    Vector2T<float> position = bounds.minimum();
    Vector2T<float> size = bounds.size();
    nvgRect(_nvgContext, position.x, position.y, size.x, size.y);
}

void NanoVGVectorRenderer::setFont(Font& font, Real size)
{
    // If the font was not loaded in NanoVG yet
    if (font.id() == FontId(-1))
    {
        Timer timer;

        // Create the NanoVG font and set the id
        FontId id = nvgCreateFontMem(_nvgContext, font.name().c_str(), const_cast<unsigned char*>(&font.rawData()[0]), static_cast<int>(font.rawData().size()), 0);
        font.setId(id);

        HECT_TRACE(format("Built font '%s' in %ims", font.name().c_str(), timer.elapsed().milliseconds()));
    }

    // Set the font
    nvgFontSize(_nvgContext, static_cast<float>(size));
    nvgFontFaceId(_nvgContext, font.id());
}

void NanoVGVectorRenderer::text(const std::string& text, const Rectangle& bounds, HorizontalAlign horizontalAlign, VerticalAlign verticalAlign)
{
    Vector2T<float> position = bounds.minimum();
    Vector2T<float> size = bounds.size();

    // Compute the text bounds
    float measuredBounds[4];
    nvgTextBounds(_nvgContext, 0, 0, text.c_str(), nullptr, measuredBounds);
    Vector2T<float> textPosition(measuredBounds[0], measuredBounds[1]);
    Vector2T<float> textSize(measuredBounds[2] - measuredBounds[0], measuredBounds[3] - measuredBounds[1]);
    Vector2T<float> actual(position - textPosition);

    // Align horizontally
    switch (horizontalAlign)
    {
    case HorizontalAlign_Left:
        break;
    case HorizontalAlign_Center:
        actual.x += size.x / 2 - textSize.x / 2;
        break;
    case HorizontalAlign_Right:
        actual.x += size.x - textSize.x;
        break;
    }

    // Align vertically
    switch (verticalAlign)
    {
    case VerticalAlign_Bottom:
        actual.y += size.y - textSize.y;
        break;
    case VerticalAlign_Center:
        actual.y += size.y / 2 - textSize.y / 2;
        break;
    case VerticalAlign_Top:
        break;
    }

    // Render the text
    nvgScissor(_nvgContext, position.x, position.y, size.x, size.y);
    nvgText(_nvgContext, actual.x, actual.y, text.c_str(), nullptr);
    nvgResetScissor(_nvgContext);
}

NVGcolor NanoVGVectorRenderer::convertColor(const Color& color) const
{
    NVGcolor result;
    result.r = static_cast<float>(color[0]);
    result.g = static_cast<float>(color[1]);
    result.b = static_cast<float>(color[2]);
    result.a = static_cast<float>(color[3]);
    return result;
}

#endif
