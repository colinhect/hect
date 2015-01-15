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
#pragma once

#include "Hect/Core/Configuration.h"
#include "Hect/Core/Export.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/Renderer.h"

#ifdef HECT_RENDERER_OPENGL

#include <nanovg.h>

namespace hect
{

class HECT_EXPORT OpenGLRenderer :
    public Renderer
{
public:
    OpenGLRenderer();
    ~OpenGLRenderer();

    void uploadFrameBuffer(FrameBuffer& frameBuffer) override;
    void destroyFrameBuffer(FrameBuffer& frameBuffer) override;
    void uploadRenderBuffer(RenderBuffer& renderBuffer) override;
    void destroyRenderBuffer(RenderBuffer& renderBuffer) override;
    void uploadShader(Shader& shader) override;
    void destroyShader(Shader& shader) override;
    void uploadTexture(Texture& texture) override;
    void destroyTexture(Texture& texture) override;
    Image downloadTextureImage(const Texture& texture) override;
    void uploadMesh(Mesh& mesh) override;
    void destroyMesh(Mesh& mesh) override;
    void uploadFont(Font& font) override;
    void destroyFont(Font& font) override;

protected:
    void setTarget(RenderTarget& target) override;
    void setTarget(Window& window) override;
    void setTarget(FrameBuffer& frameBuffer) override;

    void frameBegin(RenderTarget& target) override;
    void frameEnd() override;
    void frameSetCullMode(CullMode cullMode) override;
    void frameSetShader(Shader& shader) override;
    void frameSetUniform(const Uniform& uniform, const UniformValue& value) override;
    void frameSetUniform(const Uniform& uniform, int value) override;
    void frameSetUniform(const Uniform& uniform, Real value) override;
    void frameSetUniform(const Uniform& uniform, const Vector2& value) override;
    void frameSetUniform(const Uniform& uniform, const Vector3& value) override;
    void frameSetUniform(const Uniform& uniform, const Vector4& value) override;
    void frameSetUniform(const Uniform& uniform, const Matrix4& value) override;
    void frameSetUniform(const Uniform& uniform, const Color& value) override;
    void frameSetUniform(const Uniform& uniform, Texture& value) override;
    void frameRenderMesh(Mesh& mesh) override;
    void frameClear() override;

    void vectorFrameBegin(RenderTarget& target) override;
    void vectorFrameEnd() override;
    void vectorFrameSetFillColor(const Color& color) override;
    void vectorFrameRectangle(const Rectangle& bounds) override;
    void vectorFrameRenderPath() override;
    void vectorFrameSetFont(Font& font, Real size) override;
    void vectorFrameRenderText(const std::string& text, const Rectangle& bounds, HorizontalAlign horizontalAlign, VerticalAlign verticalAlign) override;

private:
    void beginFrame();
    void endFrame();
    void bindTexture(Texture& texture, size_t index);
    NVGcolor convertColor(const Color& color) const;

    NVGcontext* _nvgContext { nullptr };
};

}

#endif