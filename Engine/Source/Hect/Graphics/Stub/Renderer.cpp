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
#include "Hect/Graphics/Renderer.h"

#include "Hect/Core/Configuration.h"

#ifndef HECT_RENDERER_OPENGL

#include "Hect/Graphics/FrameBuffer.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Graphics/Shader.h"
#include "Hect/Graphics/Texture2.h"
#include "Hect/Graphics/Texture3.h"
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

class Texture2Data :
    public Renderer::Data<Texture2>
{
public:
    Texture2Data(Renderer& renderer, Texture2& object) :
        Renderer::Data<Texture2>(renderer, object),
        pixelData(object.image().pixelData())
    {
    }

    ~Texture2Data()
    {
        if (object && object->isUploaded())
        {
            renderer->destroyTexture(*object);
        }
    }

    ByteVector pixelData;
};

class Texture3Data :
    public Renderer::Data<Texture3>
{
public:
    Texture3Data(Renderer& renderer, Texture3& object) :
        Renderer::Data<Texture3>(renderer, object)
    {
        for (unsigned z = 0; z < object.depth(); ++z)
        {
            ByteVector& imagePixelData = object.image(z).pixelData();
            pixelData.push_back(imagePixelData);
        }
    }

    ~Texture3Data()
    {
        if (object && object->isUploaded())
        {
            renderer->destroyTexture(*object);
        }
    }

    std::vector<ByteVector> pixelData;
};

class TextureCubeData :
    public Renderer::Data<TextureCube>
{
public:
    TextureCubeData(Renderer& renderer, TextureCube& object) :
        Renderer::Data<TextureCube>(renderer, object)
    {
    }

    ~TextureCubeData()
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

void Renderer::Frame::setCullMode(CullMode cullMode)
{
    (void)cullMode;
}

void Renderer::Frame::setShader(Shader& shader)
{
    if (!shader.isUploaded())
    {
        _renderer.uploadShader(shader);
    }
}

void Renderer::Frame::setUniform(const Uniform& uniform, const UniformValue& value)
{
    (void)uniform;
    (void)value;
}

void Renderer::Frame::setUniform(const Uniform& uniform, int value)
{
    (void)uniform;
    (void)value;
}

void Renderer::Frame::setUniform(const Uniform& uniform, double value)
{
    (void)uniform;
    (void)value;
}

void Renderer::Frame::setUniform(const Uniform& uniform, const Vector2& value)
{
    (void)uniform;
    (void)value;
}

void Renderer::Frame::setUniform(const Uniform& uniform, const Vector3& value)
{
    (void)uniform;
    (void)value;
}

void Renderer::Frame::setUniform(const Uniform& uniform, const Vector4& value)
{
    (void)uniform;
    (void)value;
}

void Renderer::Frame::setUniform(const Uniform& uniform, const Matrix4& value)
{
    (void)uniform;
    (void)value;
}

void Renderer::Frame::setUniform(const Uniform& uniform, const Color& value)
{
    (void)uniform;
    (void)value;
}

void Renderer::Frame::setUniform(const Uniform& uniform, Texture2& value)
{
    (void)uniform;
    (void)value;
}

void Renderer::Frame::setUniform(const Uniform& uniform, Texture3& value)
{
    (void)uniform;
    (void)value;
}

void Renderer::Frame::setUniform(const Uniform& uniform, TextureCube& value)
{
    (void)uniform;
    (void)value;
}

void Renderer::Frame::renderMesh(Mesh& mesh)
{
    if (!mesh.isUploaded())
    {
        _renderer.uploadMesh(mesh);
    }
}

void Renderer::Frame::renderViewport()
{
}

void Renderer::Frame::clear(bool depth)
{
    (void)depth;
}

Renderer::Frame::Frame(Renderer& renderer, RenderTarget& target) :
    _renderer(renderer)
{
    _renderer.setTarget(target);
}

Renderer::~Renderer()
{
}

void Renderer::uploadFrameBuffer(FrameBuffer& frameBuffer)
{
    if (frameBuffer.isUploaded())
    {
        return;
    }

    frameBuffer.setAsUploaded(*this, new FrameBufferData(*this, frameBuffer));
}

void Renderer::destroyFrameBuffer(FrameBuffer& frameBuffer)
{
    if (!frameBuffer.isUploaded())
    {
        return;
    }

    frameBuffer.setAsDestroyed();
}

void Renderer::uploadShader(Shader& shader)
{
    if (shader.isUploaded())
    {
        return;
    }

    shader.setAsUploaded(*this, new ShaderData(*this, shader));
}

void Renderer::destroyShader(Shader& shader)
{
    if (!shader.isUploaded())
    {
        return;
    }

    shader.setAsDestroyed();
}

void Renderer::uploadTexture(Texture2& texture)
{
    if (texture.isUploaded())
    {
        return;
    }

    texture.setAsUploaded(*this, new Texture2Data(*this, texture));
}

void Renderer::uploadTexture(Texture3& texture)
{
    if (texture.isUploaded())
    {
        return;
    }

    texture.setAsUploaded(*this, new Texture3Data(*this, texture));
}

void Renderer::uploadTexture(TextureCube& texture)
{
    if (texture.isUploaded())
    {
        return;
    }

    texture.setAsUploaded(*this, new TextureCubeData(*this, texture));
}

void Renderer::destroyTexture(Texture2& texture, bool downloadImage)
{
    if (!texture.isUploaded())
    {
        return;
    }

    if (downloadImage)
    {
        // Force the texture to download its image
        texture.image();
    }

    texture.setAsDestroyed();
}

void Renderer::destroyTexture(Texture3& texture, bool downloadImage)
{
    if (!texture.isUploaded())
    {
        return;
    }

    if (downloadImage)
    {
        // Force the texture to download its images
        texture.image(0);
    }

    texture.setAsDestroyed();
}

void Renderer::destroyTexture(TextureCube& texture, bool downloadImage)
{
    (void)downloadImage;

    if (!texture.isUploaded())
    {
        return;
    }

    texture.setAsDestroyed();
}

void Renderer::downloadTextureImage(Texture2& texture)
{
    if (!texture.isUploaded())
    {
        throw InvalidOperation("The texture is not uploaded");
    }

    auto data = texture.dataAs<Texture2Data>();
    ByteVector pixelData = data->pixelData;
    texture.image().setPixelData(std::move(pixelData));
}

void Renderer::downloadTextureImages(Texture3& texture)
{
    if (!texture.isUploaded())
    {
        throw InvalidOperation("The texture is not uploaded");
    }

    auto data = texture.dataAs<Texture3Data>();
    for (unsigned z = 0; z < texture.depth(); ++z)
    {
        ByteVector pixelData = data->pixelData[z];
        texture.image(z).setPixelData(std::move(pixelData));
    }
}

void Renderer::uploadMesh(Mesh& mesh)
{
    if (mesh.isUploaded())
    {
        return;
    }

    mesh.setAsUploaded(*this, new MeshData(*this, mesh));
}

void Renderer::destroyMesh(Mesh& mesh)
{
    if (!mesh.isUploaded())
    {
        return;
    }

    mesh.setAsDestroyed();
}

void Renderer::setTarget(RenderTarget& target)
{
    (void)target;
}

void Renderer::setTarget(Window& window)
{
    (void)window;
}

void Renderer::setTarget(FrameBuffer& frameBuffer)
{
    if (!frameBuffer.isUploaded())
    {
        uploadFrameBuffer(frameBuffer);
    }
}

Renderer::Renderer()
{
}

#endif
