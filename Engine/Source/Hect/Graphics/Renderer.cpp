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

#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/Shader.h"

using namespace hect;

Renderer::Frame::~Frame()
{
    _renderer.on_end_frame();
    _renderer._in_frame = false;
}

Renderer::Frame::Frame(Frame&& frame) :
    _renderer(frame._renderer)
{
}

Renderer::Frame::Frame(Renderer& renderer, RenderTarget& target) :
    _renderer(renderer)
{
    renderer.on_begin_frame(target);
}

void Renderer::Frame::set_cull_mode(CullMode cull_mode)
{
    _renderer.set_cull_mode(cull_mode);
}

void Renderer::Frame::set_shader(Shader& shader)
{
    _renderer.set_shader(shader);

    // Set the values for each unbound uniform
    for (const Uniform& uniform : shader.uniforms())
    {
        if (uniform.binding() == UniformBinding::None)
        {
            set_uniform(uniform, uniform.value());
        }
    }
}

void Renderer::Frame::set_uniform(const Uniform& uniform, const UniformValue& value)
{
    switch (value.type())
    {
    case UniformType::Int:
        set_uniform(uniform, value.as_int());
        break;
    case UniformType::Float:
        set_uniform(uniform, value.as_double());
        break;
    case UniformType::Vector2:
        set_uniform(uniform, value.as_vector2());
        break;
    case UniformType::Vector3:
        set_uniform(uniform, value.as_vector3());
        break;
    case UniformType::Vector4:
        set_uniform(uniform, value.as_vector4());
        break;
    case UniformType::Matrix4:
        set_uniform(uniform, value.as_matrix4());
        break;
    case UniformType::Color:
        set_uniform(uniform, value.as_color());
        break;
    case UniformType::Texture2:
    {
        Texture2::Handle texture = value.as_texture2();
        if (texture)
        {
            set_uniform(uniform, *texture);
        }
    }
    break;
    case UniformType::Texture3:
    {
        Texture3::Handle texture = value.as_texture3();
        if (texture)
        {
            set_uniform(uniform, *texture);
        }
    }
    break;
    case UniformType::TextureCube:
    {
        TextureCube::Handle texture = value.as_texture_cube();
        if (texture)
        {
            set_uniform(uniform, *texture);
        }
    }
    break;
    }
}

void Renderer::Frame::set_uniform(const Uniform& uniform, int value)
{
    if (uniform.type() != UniformType::Int)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.set_uniform(uniform, value);
}

void Renderer::Frame::set_uniform(const Uniform& uniform, double value)
{
    if (uniform.type() != UniformType::Float)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.set_uniform(uniform, value);
}

void Renderer::Frame::set_uniform(const Uniform& uniform, Vector2 value)
{
    if (uniform.type() != UniformType::Vector2)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.set_uniform(uniform, value);
}

void Renderer::Frame::set_uniform(const Uniform& uniform, Vector3 value)
{
    if (uniform.type() != UniformType::Vector3)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.set_uniform(uniform, value);
}

void Renderer::Frame::set_uniform(const Uniform& uniform, Vector4 value)
{
    if (uniform.type() != UniformType::Vector4)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.set_uniform(uniform, value);
}

void Renderer::Frame::set_uniform(const Uniform& uniform, const Matrix4& value)
{
    if (uniform.type() != UniformType::Matrix4)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.set_uniform(uniform, value);
}

void Renderer::Frame::set_uniform(const Uniform& uniform, Color value)
{
    if (uniform.type() != UniformType::Color)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.set_uniform(uniform, value);
}

void Renderer::Frame::set_uniform(const Uniform& uniform, Texture2& value)
{
    if (uniform.type() != UniformType::Texture2)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.set_uniform(uniform, value);
}

void Renderer::Frame::set_uniform(const Uniform& uniform, Texture3& value)
{
    if (uniform.type() != UniformType::Texture3)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.set_uniform(uniform, value);
}

void Renderer::Frame::set_uniform(const Uniform& uniform, TextureCube& value)
{
    if (uniform.type() != UniformType::TextureCube)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    _renderer.set_uniform(uniform, value);
}

void Renderer::Frame::render_mesh(Mesh& mesh)
{
    _renderer.render_mesh(mesh);
}

void Renderer::Frame::render_viewport()
{
    _renderer.render_viewport();
}

void Renderer::Frame::clear(Color color, bool depth)
{
    _renderer.clear(color, depth);
}

Renderer::~Renderer()
{
    shutdown();
}

Renderer::Frame Renderer::begin_frame(RenderTarget& target)
{
    if (_in_frame)
    {
        throw InvalidOperation("Cannot begin a new frame when there is an active frame");
    }
    _in_frame = true;

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
