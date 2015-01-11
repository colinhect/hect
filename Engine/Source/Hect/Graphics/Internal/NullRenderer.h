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
#include "Hect/Graphics/Renderer.h"

namespace hect
{

class HECT_EXPORT NullRenderer :
    public Renderer
{
public:
    void beginFrame() override;
    void endFrame() override;
    void setTarget(RenderTarget& renderTarget) override;
    void setTarget(Window& window) override;
    void setTarget(FrameBuffer& frameBuffer) override;
    void uploadFrameBuffer(FrameBuffer& frameBuffer) override;
    void destroyFrameBuffer(FrameBuffer& frameBuffer) override;
    void uploadRenderBuffer(RenderBuffer& renderBuffer) override;
    void destroyRenderBuffer(RenderBuffer& renderBuffer) override;
    void setShader(Shader& shader) override;
    void uploadShader(Shader& shader) override;
    void destroyShader(Shader& shader) override;
    void setUniform(const Uniform& uniform, const UniformValue& value) override;
    void setUniform(const Uniform& uniform, int value) override;
    void setUniform(const Uniform& uniform, Real value) override;
    void setUniform(const Uniform& uniform, const Vector2& value) override;
    void setUniform(const Uniform& uniform, const Vector3& value) override;
    void setUniform(const Uniform& uniform, const Vector4& value) override;
    void setUniform(const Uniform& uniform, const Matrix4& value) override;
    void setUniform(const Uniform& uniform, Texture& value) override;
    void uploadTexture(Texture& texture) override;
    void destroyTexture(Texture& texture) override;
    Image downloadTextureImage(const Texture& texture) override;
    void setMesh(Mesh& mesh) override;
    void uploadMesh(Mesh& mesh) override;
    void destroyMesh(Mesh& mesh) override;
    void setCullMode(CullMode cullMode) override;
    void render() override;
    void clear() override;
};

}