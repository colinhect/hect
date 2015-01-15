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
#include "Renderer.h"

using namespace hect;

Renderer::Frame::Frame(Frame&& frame) :
    _renderer(frame._renderer)
{
    frame._renderer = nullptr;
}

Renderer::Frame::~Frame()
{
    assert(_renderer);
    _renderer->frameEnd();
}

void Renderer::Frame::setCullMode(CullMode cullMode)
{
    assert(_renderer);
    _renderer->frameSetCullMode(cullMode);
}

void Renderer::Frame::setShader(Shader& shader)
{
    assert(_renderer);
    _renderer->frameSetShader(shader);
}

void Renderer::Frame::setUniform(const Uniform& uniform, const UniformValue& value)
{
    assert(_renderer);
    _renderer->frameSetUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, int value)
{
    assert(_renderer);
    _renderer->frameSetUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, Real value)
{
    assert(_renderer);
    _renderer->frameSetUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, const Vector2& value)
{
    assert(_renderer);
    _renderer->frameSetUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, const Vector3& value)
{
    assert(_renderer);
    _renderer->frameSetUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, const Vector4& value)
{
    assert(_renderer);
    _renderer->frameSetUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, const Matrix4& value)
{
    assert(_renderer);
    _renderer->frameSetUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, const Color& value)
{
    assert(_renderer);
    _renderer->frameSetUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, Texture& value)
{
    assert(_renderer);
    _renderer->frameSetUniform(uniform, value);
}

void Renderer::Frame::renderMesh(Mesh& mesh)
{
    assert(_renderer);
    _renderer->frameRenderMesh(mesh);
}

void Renderer::Frame::clear()
{
    assert(_renderer);
    _renderer->frameClear();
}

Renderer::Frame::Frame(Renderer& renderer, RenderTarget& target) :
    _renderer(&renderer)
{
    assert(_renderer);
    _renderer->vectorFrameBegin(target);
}

Renderer::VectorFrame::VectorFrame(VectorFrame&& frame) :
    _renderer(frame._renderer)
{
    frame._renderer = nullptr;
}

Renderer::VectorFrame::~VectorFrame()
{
    assert(_renderer);
    _renderer->vectorFrameEnd();
}

void Renderer::VectorFrame::setFillColor(const Color& color)
{
    assert(_renderer);
    _renderer->vectorFrameSetFillColor(color);
}

void Renderer::VectorFrame::rectangle(const Rectangle& bounds)
{
    assert(_renderer);
    _renderer->vectorFrameRectangle(bounds);
}

void Renderer::VectorFrame::renderPath()
{
    assert(_renderer);
    _renderer->vectorFrameRenderPath();
}

void Renderer::VectorFrame::setFont(Font& font, Real size)
{
    assert(_renderer);
    _renderer->vectorFrameSetFont(font, size);
}

void Renderer::VectorFrame::renderText(const std::string& text, const Rectangle& bounds, HorizontalAlign horizontalAlign, VerticalAlign verticalAlign)
{
    assert(_renderer);
    _renderer->vectorFrameRenderText(text, bounds, horizontalAlign, verticalAlign);
}

Renderer::VectorFrame::VectorFrame(Renderer& renderer, RenderTarget& target) :
    _renderer(&renderer)
{
    assert(_renderer);
    _renderer->vectorFrameBegin(target);
}

Renderer::Frame Renderer::beginFrame(RenderTarget& target)
{
    return Frame(*this, target);
}

Renderer::VectorFrame Renderer::beginVectorFrame(RenderTarget& target)
{
    return VectorFrame(*this, target);
}

Renderer::Capabilities& Renderer::capabilities()
{
    return _capabilities;
}

const Renderer::Capabilities& Renderer::capabilities() const
{
    return _capabilities;
}

Renderer::Statistics& Renderer::statistics()
{
    return _statistics;
}

const Renderer::Statistics& Renderer::statistics() const
{
    return _statistics;
}