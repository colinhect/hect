///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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

#include "Hect/Graphics/Shader.h"

using namespace hect;

Renderer::Frame::~Frame()
{
    _renderer.onEndFrame();
    _renderer._inFrame = false;
}

Renderer::Frame::Frame(Frame&& frame) :
    _renderer(frame._renderer)
{
}

Renderer::Frame::Frame(Renderer& renderer, RenderTarget& target) :
    _renderer(renderer)
{
    renderer.onBeginFrame(target);
}

void Renderer::Frame::setCullMode(CullMode cullMode)
{
    _renderer.setCullMode(cullMode);
}

void Renderer::Frame::setShader(Shader& shader)
{
    // Upload the shader if needed
    if (!shader.isUploaded())
    {
        _renderer.uploadShader(shader);
    }

    _renderer.setShader(shader);
}

void Renderer::Frame::setUniform(const Uniform& uniform, const UniformValue& value)
{
    switch (value.type())
    {
    case UniformType::Int:
        setUniform(uniform, value.asInt());
        break;
    case UniformType::Float:
        setUniform(uniform, value.asDouble());
        break;
    case UniformType::Vector2:
        setUniform(uniform, value.asVector2());
        break;
    case UniformType::Vector3:
        setUniform(uniform, value.asVector3());
        break;
    case UniformType::Vector4:
        setUniform(uniform, value.asVector4());
        break;
    case UniformType::Matrix4:
        setUniform(uniform, value.asMatrix4());
        break;
    case UniformType::Color:
        setUniform(uniform, value.asColor());
        break;
    case UniformType::Texture2:
    {
        Texture2::Handle texture = value.asTexture2();
        if (texture)
        {
            setUniform(uniform, *texture);
        }
    }
    break;
    case UniformType::Texture3:
    {
        Texture3::Handle texture = value.asTexture3();
        if (texture)
        {
            setUniform(uniform, *texture);
        }
    }
    break;
    case UniformType::TextureCube:
    {
        TextureCube::Handle texture = value.asTextureCube();
        if (texture)
        {
            setUniform(uniform, *texture);
        }
    }
    break;
    }
}

void Renderer::Frame::setUniform(const Uniform& uniform, int value)
{
    if (uniform.type() != UniformType::Int)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.setUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, double value)
{
    if (uniform.type() != UniformType::Float)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.setUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, Vector2 value)
{
    if (uniform.type() != UniformType::Vector2)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.setUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, Vector3 value)
{
    if (uniform.type() != UniformType::Vector3)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.setUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, Vector4 value)
{
    if (uniform.type() != UniformType::Vector4)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.setUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, const Matrix4& value)
{
    if (uniform.type() != UniformType::Matrix4)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.setUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, Color value)
{
    if (uniform.type() != UniformType::Color)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.setUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, Texture2& value)
{
    if (uniform.type() != UniformType::Texture2)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.setUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, Texture3& value)
{
    if (uniform.type() != UniformType::Texture3)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.setUniform(uniform, value);
}

void Renderer::Frame::setUniform(const Uniform& uniform, TextureCube& value)
{
    if (uniform.type() != UniformType::TextureCube)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.setUniform(uniform, value);
}

Renderer::~Renderer()
{
    shutdown();
}

Renderer::Frame Renderer::beginFrame(RenderTarget& target)
{
    if (_inFrame)
    {
        throw InvalidOperation("Cannot begin a new frame when there is an active frame");
    }
    _inFrame = true;

    return Frame(*this, target);
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

Renderer::Renderer()
{
    initialize();
}
