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
#include "NullRenderer.h"

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"
#include "Hect/Graphics/FrameBuffer.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Graphics/Shader.h"
#include "Hect/Graphics/Texture.h"
#include "Hect/Runtime/Window.h"

using namespace hect;

namespace
{

class ShaderData :
    public Renderer::Data<Shader>
{
public:
    ShaderData(Renderer& renderer, Shader& object) :
        Renderer::Data<Shader>(renderer, object)
    {
    }

    ~ShaderData()
    {
        if (object && object->isUploaded())
        {
            renderer->destroyShader(*object);
        }
    }
};

class TextureData :
    public Renderer::Data<Texture>
{
public:
    TextureData(Renderer& renderer, Texture& object) :
        Renderer::Data<Texture>(renderer, object)
    {
    }

    ~TextureData()
    {
        if (object && object->isUploaded())
        {
            renderer->destroyTexture(*object);
        }
    }
};

class FrameBufferData :
    public Renderer::Data<FrameBuffer>
{
public:
    FrameBufferData(Renderer& renderer, FrameBuffer& object) :
        Renderer::Data<FrameBuffer>(renderer, object)
    {
    }

    ~FrameBufferData()
    {
        if (object && object->isUploaded())
        {
            renderer->destroyFrameBuffer(*object);
        }
    }
};

class RenderBufferData :
    public Renderer::Data<RenderBuffer>
{
public:
    RenderBufferData(Renderer& renderer, RenderBuffer& object) :
        Renderer::Data<RenderBuffer>(renderer, object)
    {
    }

    ~RenderBufferData()
    {
        if (object && object->isUploaded())
        {
            renderer->destroyRenderBuffer(*object);
        }
    }
};

class MeshData :
    public Renderer::Data<Mesh>
{
public:
    MeshData(Renderer& renderer, Mesh& object) :
        Renderer::Data<Mesh>(renderer, object)
    {
    }

    ~MeshData()
    {
        if (object && object->isUploaded())
        {
            renderer->destroyMesh(*object);
        }
    }
};

}

void NullRenderer::beginFrame()
{
}

void NullRenderer::endFrame()
{
}

void NullRenderer::setTarget(RenderTarget& renderTarget)
{
    (void)renderTarget;
}

void NullRenderer::setTarget(Window& window)
{
    (void)window;
}

void NullRenderer::setTarget(FrameBuffer& frameBuffer)
{
    if (!frameBuffer.isUploaded())
    {
        uploadFrameBuffer(frameBuffer);
    }
}

void NullRenderer::uploadFrameBuffer(FrameBuffer& frameBuffer)
{
    if (frameBuffer.isUploaded())
    {
        return;
    }

    frameBuffer.setAsUploaded(*this, new FrameBufferData(*this, frameBuffer));
}

void NullRenderer::destroyFrameBuffer(FrameBuffer& frameBuffer)
{
    if (!frameBuffer.isUploaded())
    {
        return;
    }

    frameBuffer.setAsDestroyed();
}

void NullRenderer::uploadRenderBuffer(RenderBuffer& renderBuffer)
{
    if (renderBuffer.isUploaded())
    {
        return;
    }

    renderBuffer.setAsUploaded(*this, new RenderBufferData(*this, renderBuffer));
}

void NullRenderer::destroyRenderBuffer(RenderBuffer& renderBuffer)
{
    if (!renderBuffer.isUploaded())
    {
        return;
    }

    renderBuffer.setAsDestroyed();
}

void NullRenderer::setShader(Shader& shader)
{
    if (!shader.isUploaded())
    {
        uploadShader(shader);
    }
}

void NullRenderer::uploadShader(Shader& shader)
{
    if (shader.isUploaded())
    {
        return;
    }

    shader.setAsUploaded(*this, new ShaderData(*this, shader));
}

void NullRenderer::destroyShader(Shader& shader)
{
    if (!shader.isUploaded())
    {
        return;
    }

    shader.setAsDestroyed();
}

void NullRenderer::setUniform(const Uniform& uniform, const UniformValue& value)
{
    (void)uniform;
    (void)value;
}

void NullRenderer::setUniform(const Uniform& uniform, int value)
{
    (void)uniform;
    (void)value;
}

void NullRenderer::setUniform(const Uniform& uniform, Real value)
{
    (void)uniform;
    (void)value;
}

void NullRenderer::setUniform(const Uniform& uniform, const Vector2& value)
{
    (void)uniform;
    (void)value;
}

void NullRenderer::setUniform(const Uniform& uniform, const Vector3& value)
{
    (void)uniform;
    (void)value;
}

void NullRenderer::setUniform(const Uniform& uniform, const Vector4& value)
{
    (void)uniform;
    (void)value;
}

void NullRenderer::setUniform(const Uniform& uniform, const Matrix4& value)
{
    (void)uniform;
    (void)value;
}

void NullRenderer::setUniform(const Uniform& uniform, Texture& value)
{
    (void)uniform;
    (void)value;
}

void NullRenderer::uploadTexture(Texture& texture)
{
    if (texture.isUploaded())
    {
        return;
    }

    texture.setAsUploaded(*this, new TextureData(*this, texture));
}

void NullRenderer::destroyTexture(Texture& texture)
{
    if (!texture.isUploaded())
    {
        return;
    }

    texture.setAsDestroyed();
}

Image NullRenderer::downloadTextureImage(const Texture& texture)
{
    (void)texture;

    return Image();
}

void NullRenderer::setMesh(Mesh& mesh)
{
    if (!mesh.isUploaded())
    {
        uploadMesh(mesh);
    }
}

void NullRenderer::uploadMesh(Mesh& mesh)
{
    if (mesh.isUploaded())
    {
        return;
    }

    mesh.setAsUploaded(*this, new MeshData(*this, mesh));
}

void NullRenderer::destroyMesh(Mesh& mesh)
{
    if (!mesh.isUploaded())
    {
        return;
    }

    mesh.setAsDestroyed();
}

void NullRenderer::setCullMode(CullMode cullMode)
{
    (void)cullMode;
}

void NullRenderer::render()
{
}

void NullRenderer::clear()
{
}