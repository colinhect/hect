///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2014 Colin Hill
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
#include "Hect/Graphics/GraphicsContext.h"

#ifdef HECT_RENDERER_OPENGL

namespace hect
{

class OpenGLGraphicsContext :
    public GraphicsContext
{
public:
    OpenGLGraphicsContext(Window& window);

    void beginFrame() override;
    void endFrame() override;
    void bindState(const RenderState& state) override;
    void bindTarget(RenderTarget& renderTarget) override;
    void bindWindow(Window& window) override;
    void bindFrameBuffer(FrameBuffer& frameBuffer) override;
    void uploadFrameBuffer(FrameBuffer& frameBuffer) override;
    void destroyFrameBuffer(FrameBuffer& frameBuffer) override;
    void bindShader(Shader& shader) override;
    void uploadShader(Shader& shader) override;
    void destroyShader(Shader& shader) override;
    void bindShaderParameter(const ShaderParameter& parameter, const ShaderValue& value) override;
    void bindTexture(Texture& texture, unsigned index) override;
    void uploadTexture(Texture& texture) override;
    void destroyTexture(Texture& texture) override;
    Image downloadTextureImage(const Texture& texture) override;
    void bindMesh(Mesh& mesh) override;
    void uploadMesh(Mesh& mesh) override;
    void destroyMesh(Mesh& mesh) override;
    void draw() override;
    void clear() override;
    const Capabilities& capabilities() const override;

private:
    void initialize();

    Capabilities _capabilities;

    RenderTarget* _boundTarget{ nullptr };
    Shader* _boundShader{ nullptr };
    Mesh* _boundMesh{ nullptr };
    std::vector<Texture*> _boundTextures;
};

}

#endif