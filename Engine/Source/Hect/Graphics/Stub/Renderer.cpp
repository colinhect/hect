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
        if (object && object->is_uploaded())
        {
            renderer->destroy_shader(*object);
        }
    }
};

class Texture2Data :
    public Renderer::Data<Texture2>
{
public:
    Texture2Data(Renderer& renderer, Texture2& object) :
        Renderer::Data<Texture2>(renderer, object),
        pixel_data(object.image().pixel_data())
    {
    }

    ~Texture2Data()
    {
        if (object && object->is_uploaded())
        {
            renderer->destroy_texture(*object);
        }
    }

    ByteVector pixel_data;
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
            ByteVector& image_pixel_data = object.image(z).pixel_data();
            pixel_data.push_back(image_pixel_data);
        }
    }

    ~Texture3Data()
    {
        if (object && object->is_uploaded())
        {
            renderer->destroy_texture(*object);
        }
    }

    std::vector<ByteVector> pixel_data;
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
        if (object && object->is_uploaded())
        {
            renderer->destroy_texture(*object);
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
        if (object && object->is_uploaded())
        {
            renderer->destroy_frame_buffer(*object);
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
        if (object && object->is_uploaded())
        {
            renderer->destroy_mesh(*object);
        }
    }
};

}

void Renderer::upload_frame_buffer(FrameBuffer& frame_buffer)
{
    if (frame_buffer.is_uploaded())
    {
        return;
    }

    frame_buffer.set_as_uploaded(*this, new FrameBufferData(*this, frame_buffer));
}

void Renderer::destroy_frame_buffer(FrameBuffer& frame_buffer)
{
    if (!frame_buffer.is_uploaded())
    {
        return;
    }

    frame_buffer.set_as_destroyed();
}

void Renderer::upload_shader(Shader& shader)
{
    if (shader.is_uploaded())
    {
        return;
    }

    shader.set_as_uploaded(*this, new ShaderData(*this, shader));
}

void Renderer::destroy_shader(Shader& shader)
{
    if (!shader.is_uploaded())
    {
        return;
    }

    shader.set_as_destroyed();
}

void Renderer::upload_texture(Texture2& texture)
{
    if (texture.is_uploaded())
    {
        return;
    }

    texture.set_as_uploaded(*this, new Texture2Data(*this, texture));
}

void Renderer::upload_texture(Texture3& texture)
{
    if (texture.is_uploaded())
    {
        return;
    }

    texture.set_as_uploaded(*this, new Texture3Data(*this, texture));
}

void Renderer::upload_texture(TextureCube& texture)
{
    if (texture.is_uploaded())
    {
        return;
    }

    texture.set_as_uploaded(*this, new TextureCubeData(*this, texture));
}

void Renderer::destroy_texture(Texture2& texture, bool download_image)
{
    if (!texture.is_uploaded())
    {
        return;
    }

    if (download_image)
    {
        // Force the texture to download its image
        texture.image();
    }

    texture.set_as_destroyed();
}

void Renderer::destroy_texture(Texture3& texture, bool download_image)
{
    if (!texture.is_uploaded())
    {
        return;
    }

    if (download_image)
    {
        // Force the texture to download its images
        texture.image(0);
    }

    texture.set_as_destroyed();
}

void Renderer::destroy_texture(TextureCube& texture, bool download_image)
{
    (void)download_image;

    if (!texture.is_uploaded())
    {
        return;
    }

    texture.set_as_destroyed();
}

void Renderer::download_texture_image(Texture2& texture)
{
    if (!texture.is_uploaded())
    {
        throw InvalidOperation("The texture is not uploaded");
    }

    auto data = texture.data_as<Texture2Data>();
    ByteVector pixel_data = data->pixel_data;
    texture.image().set_pixel_data(std::move(pixel_data));
}

void Renderer::download_texture_images(Texture3& texture)
{
    if (!texture.is_uploaded())
    {
        throw InvalidOperation("The texture is not uploaded");
    }

    auto data = texture.data_as<Texture3Data>();
    for (unsigned z = 0; z < texture.depth(); ++z)
    {
        ByteVector pixel_data = data->pixel_data[z];
        texture.image(z).set_pixel_data(std::move(pixel_data));
    }
}

void Renderer::upload_mesh(Mesh& mesh)
{
    if (mesh.is_uploaded())
    {
        return;
    }

    mesh.set_as_uploaded(*this, new MeshData(*this, mesh));
}

void Renderer::destroy_mesh(Mesh& mesh)
{
    if (!mesh.is_uploaded())
    {
        return;
    }

    mesh.set_as_destroyed();
}

void Renderer::initialize()
{
}

void Renderer::shutdown()
{
}

void Renderer::on_begin_frame(RenderTarget& target)
{
    (void)target;
}

void Renderer::on_end_frame()
{
}

void Renderer::set_target(RenderTarget& target)
{
    (void)target;
}

void Renderer::set_target(Window& window)
{
    (void)window;
}

void Renderer::set_target(FrameBuffer& frame_buffer)
{
    if (!frame_buffer.is_uploaded())
    {
        upload_frame_buffer(frame_buffer);
    }
}


void Renderer::set_cull_mode(CullMode cull_mode)
{
    (void)cull_mode;
}

void Renderer::set_shader(Shader& shader)
{
    if (!shader.is_uploaded())
    {
        upload_shader(shader);
    }
}

void Renderer::set_uniform(const Uniform& uniform, int value)
{
    (void)uniform;
    (void)value;
}

void Renderer::set_uniform(const Uniform& uniform, double value)
{
    (void)uniform;
    (void)value;
}

void Renderer::set_uniform(const Uniform& uniform, Vector2 value)
{
    (void)uniform;
    (void)value;
}

void Renderer::set_uniform(const Uniform& uniform, Vector3 value)
{
    (void)uniform;
    (void)value;
}

void Renderer::set_uniform(const Uniform& uniform, Vector4 value)
{
    (void)uniform;
    (void)value;
}

void Renderer::set_uniform(const Uniform& uniform, const Matrix4& value)
{
    (void)uniform;
    (void)value;
}

void Renderer::set_uniform(const Uniform& uniform, Color value)
{
    (void)uniform;
    (void)value;
}

void Renderer::set_uniform(const Uniform& uniform, Texture2& value)
{
    (void)uniform;
    (void)value;
}

void Renderer::set_uniform(const Uniform& uniform, Texture3& value)
{
    (void)uniform;
    (void)value;
}

void Renderer::set_uniform(const Uniform& uniform, TextureCube& value)
{
    (void)uniform;
    (void)value;
}

void Renderer::render_mesh(Mesh& mesh)
{
    (void)mesh;
}

void Renderer::render_viewport()
{
}

void Renderer::clear(Color color, bool depth)
{
    (void)color;
    (void)depth;
}

#endif
