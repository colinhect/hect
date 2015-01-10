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
#include <nanovg.h>
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
    _renderer.selectTarget(target);

    assert(_nvgContext);
    nvgBeginFrame(_nvgContext, target.width(), target.height(), 1);
}

void NanoVGVectorRenderer::endFrame()
{
    nvgEndFrame(_nvgContext);

    _renderer.endFrame();
}

void NanoVGVectorRenderer::selectFont(Font& font, Real size)
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

    // Select the font
    nvgFontSize(_nvgContext, static_cast<float>(size));
    nvgFontFaceId(_nvgContext, font.id());
}

void NanoVGVectorRenderer::drawText(const Vector2& position, const std::string& text)
{
    nvgText(_nvgContext, static_cast<float>(position.x), static_cast<float>(position.y), text.c_str(), nullptr);
}

#endif
