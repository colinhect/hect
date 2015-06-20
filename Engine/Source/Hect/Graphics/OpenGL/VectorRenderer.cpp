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

void VectorRenderer::Frame::setFont(const Font& font, double size)
{
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

void VectorRenderer::Frame::renderText(const std::string& text, const Vector2& position)
{
    nvgText(_nvgContext, static_cast<float>(position.x), static_cast<float>(position.y), text.c_str(), nullptr);
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
    nvgDeleteGL3(_nvgContext);
}

void VectorRenderer::onBeginFrame(RenderTarget& target)
{
    nvgBeginFrame(_nvgContext, target.width(), target.height(), 1);
}

void VectorRenderer::onEndFrame()
{
    nvgEndFrame(_nvgContext);
}

#endif
