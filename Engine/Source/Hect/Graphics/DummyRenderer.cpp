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
#include "DummyRenderer.h"

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"
#include "Hect/Graphics/FrameBuffer.h"
#include "Hect/Graphics/Material.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Graphics/Shader.h"
#include "Hect/Graphics/Texture.h"
#include "Hect/Graphics/Window.h"

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

DummyRenderer::DummyRenderer(Window& window)
{
    window;
}

void DummyRenderer::beginFrame()
{
}

void DummyRenderer::endFrame()
{
}

void DummyRenderer::bindState(const RenderState& state)
{
    state;
}

void DummyRenderer::bindTarget(RenderTarget& renderTarget)
{
    renderTarget;
}

void DummyRenderer::bindWindow(Window& window)
{
    window;
}

void DummyRenderer::bindFrameBuffer(FrameBuffer& frameBuffer)
{
    if (!frameBuffer.isUploaded())
    {
        uploadFrameBuffer(frameBuffer);
    }
}

void DummyRenderer::uploadFrameBuffer(FrameBuffer& frameBuffer)
{
    if (frameBuffer.isUploaded())
    {
        return;
    }

    frameBuffer.setAsUploaded(*this, new FrameBufferData(*this, frameBuffer));
}

void DummyRenderer::destroyFrameBuffer(FrameBuffer& frameBuffer)
{
    if (!frameBuffer.isUploaded())
    {
        return;
    }

    frameBuffer.setAsDestroyed();
}

void DummyRenderer::bindShader(Shader& shader)
{
    if (!shader.isUploaded())
    {
        uploadShader(shader);
    }
}

void DummyRenderer::uploadShader(Shader& shader)
{
    if (shader.isUploaded())
    {
        return;
    }

    shader.setAsUploaded(*this, new ShaderData(*this, shader));
}

void DummyRenderer::destroyShader(Shader& shader)
{
    if (!shader.isUploaded())
    {
        return;
    }

    shader.setAsDestroyed();
}

void DummyRenderer::bindShaderParameter(const ShaderParameter& parameter, const ShaderValue& value)
{
    parameter;
    value;
}

void DummyRenderer::bindTexture(Texture& texture, unsigned index)
{
    texture;
    index;
}

void DummyRenderer::uploadTexture(Texture& texture)
{
    if (texture.isUploaded())
    {
        return;
    }

    texture.setAsUploaded(*this, new TextureData(*this, texture));
}

void DummyRenderer::destroyTexture(Texture& texture)
{
    if (!texture.isUploaded())
    {
        return;
    }

    texture.setAsDestroyed();
}

Image DummyRenderer::downloadTextureImage(const Texture& texture)
{
    return Image();
}

void DummyRenderer::bindMesh(Mesh& mesh)
{
    if (!mesh.isUploaded())
    {
        uploadMesh(mesh);
    }
}

void DummyRenderer::uploadMesh(Mesh& mesh)
{
    if (mesh.isUploaded())
    {
        return;
    }

    mesh.setAsUploaded(*this, new MeshData(*this, mesh));
}

void DummyRenderer::destroyMesh(Mesh& mesh)
{
    if (!mesh.isUploaded())
    {
        return;
    }

    mesh.setAsDestroyed();
}

void DummyRenderer::draw()
{
}

void DummyRenderer::clear()
{
}

const Renderer::Capabilities& DummyRenderer::capabilities() const
{
    return _capabilities;
}