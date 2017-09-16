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

#ifdef HECT_RENDERER_OPENGL

#include <set>
#include <GL/glew.h>

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"
#include "Hect/Graphics/FrameBuffer.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/MeshWriter.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Graphics/Shader.h"
#include "Hect/Graphics/Texture2.h"
#include "Hect/Graphics/Texture3.h"
#include "Hect/Graphics/TextureCube.h"
#include "Hect/Runtime/Window.h"

using namespace hect;

#ifdef HECT_DEBUG_BUILD
#define GL_ASSERT(expression) expression; check_g_l_error()
#else
#define GL_ASSERT(expression) expression
#endif

namespace
{

Mesh _viewport_mesh;
FrameBuffer* _current_frame_buffer { nullptr };
std::set<Mesh*> _uploaded_meshes;

Mesh create_viewport_mesh()
{
    Mesh viewport_mesh("Viewport");

    VertexLayout vertex_layout;
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));
    vertex_layout.add_attribute(VertexAttribute(VertexAttributeSemantic::TextureCoords0, VertexAttributeType::Float32, 3));

    viewport_mesh.set_vertex_layout(vertex_layout);

    MeshWriter mesh_writer(viewport_mesh);
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(-1, -1, 0));
    mesh_writer.write_attribute_data(VertexAttributeSemantic::TextureCoords0, Vector2(0, 0));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(1, -1, 0));
    mesh_writer.write_attribute_data(VertexAttributeSemantic::TextureCoords0, Vector2(1, 0));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(1, 1, 0));
    mesh_writer.write_attribute_data(VertexAttributeSemantic::TextureCoords0, Vector2(1, 1));
    mesh_writer.add_vertex();
    mesh_writer.write_attribute_data(VertexAttributeSemantic::Position, Vector3(-1, 1, 0));
    mesh_writer.write_attribute_data(VertexAttributeSemantic::TextureCoords0, Vector2(0, 1));
    mesh_writer.add_index(0);
    mesh_writer.add_index(1);
    mesh_writer.add_index(2);
    mesh_writer.add_index(2);
    mesh_writer.add_index(3);
    mesh_writer.add_index(0);

    return viewport_mesh;
}

// OpenGL-specific data for a shader program
class ShaderData :
    public Renderer::Data<Shader>
{
public:
    ShaderData(Renderer& renderer, Shader& object, GLuint program_id, const std::vector<GLuint>& shader_ids) :
        Renderer::Data<Shader>(renderer, object),
        program_id(program_id),
        shader_ids(shader_ids)
    {
    }

    ~ShaderData()
    {
        // Destroy the shader if it is uploaded
        if (object && object->is_uploaded())
        {
            renderer->destroy_shader(*object);
        }
    }

    GLuint program_id;
    std::vector<GLuint> shader_ids;
};

// OpenGL-specific data for a texture
class Texture2Data :
    public Renderer::Data<Texture2>
{
public:
    Texture2Data(Renderer& renderer, Texture2& object, GLuint texture_id) :
        Renderer::Data<Texture2>(renderer, object),
        texture_id(texture_id)
    {
    }

    ~Texture2Data()
    {
        // Destroy the texture if it is uploaded
        if (object && object->is_uploaded())
        {
            renderer->destroy_texture(*object);
        }
    }

    GLuint texture_id;
};

// OpenGL-specific data for a texture
class Texture3Data :
    public Renderer::Data<Texture3>
{
public:
    Texture3Data(Renderer& renderer, Texture3& object, GLuint texture_id) :
        Renderer::Data<Texture3>(renderer, object),
        texture_id(texture_id)
    {
    }

    ~Texture3Data()
    {
        // Destroy the texture if it is uploaded
        if (object && object->is_uploaded())
        {
            renderer->destroy_texture(*object);
        }
    }

    GLuint texture_id;
};

// OpenGL-specific data for a cubic texture
class TextureCubeData :
    public Renderer::Data<TextureCube>
{
public:
    TextureCubeData(Renderer& renderer, TextureCube& object, GLuint texture_id) :
        Renderer::Data<TextureCube>(renderer, object),
        texture_id(texture_id)
    {
    }

    ~TextureCubeData()
    {
        // Destroy the texture if it is uploaded
        if (object && object->is_uploaded())
        {
            renderer->destroy_texture(*object);
        }
    }

    GLuint texture_id;
};

// OpenGL-specific data for a frame buffer
class FrameBufferData :
    public Renderer::Data<FrameBuffer>
{
public:
    FrameBufferData(Renderer& renderer, FrameBuffer& object, GLuint frame_buffer_id) :
        Renderer::Data<FrameBuffer>(renderer, object),
        frame_buffer_id(frame_buffer_id)
    {
    }

    ~FrameBufferData()
    {
        // Destroy the frame buffer if it is uploaded
        if (object && object->is_uploaded())
        {
            renderer->destroy_frame_buffer(*object);
        }
    }

    GLuint frame_buffer_id;
};

// OpenGL-specific data for a mesh
class MeshData :
    public Renderer::Data<Mesh>
{
public:
    MeshData(Renderer& renderer, Mesh& object, GLuint vertex_array_id, GLuint vertex_buffer_id, GLuint index_buffer_id) :
        Renderer::Data<Mesh>(renderer, object),
        vertex_array_id(vertex_array_id),
        vertex_buffer_id(vertex_buffer_id),
        index_buffer_id(index_buffer_id)
    {
    }

    ~MeshData()
    {
        // Destroy the mesh if it is uploaded
        if (object && object->is_uploaded())
        {
            renderer->destroy_mesh(*object);
        }
    }

    GLuint vertex_array_id;
    GLuint vertex_buffer_id;
    GLuint index_buffer_id;
};

GLenum _vertex_attribute_type_look_up[8] =
{
    GL_BYTE, // Int8
    GL_UNSIGNED_BYTE, // UInt8
    GL_SHORT, // Int16
    GL_UNSIGNED_SHORT, // UInt16
    GL_INT, // Int32
    GL_UNSIGNED_INT, // UInt32
    GL_HALF_FLOAT, // Float16
    GL_FLOAT // Float32
};

GLenum _index_type_look_up[3] =
{
    GL_UNSIGNED_BYTE, // UInt8
    GL_UNSIGNED_SHORT, // UInt16
    GL_UNSIGNED_INT // UInt32
};

GLenum _primitive_type_look_up[6] =
{
    GL_TRIANGLES, // Triangles
    GL_TRIANGLE_STRIP, // TriangleStrip
    GL_LINES, // Lines
    GL_LINE_STRIP, // LineStrip
    GL_POINTS, // Points
    GL_POINTS // PointSprites
};

GLenum _blend_function_look_up[2] =
{
    GL_FUNC_ADD, // Add
    GL_FUNC_SUBTRACT // Subtract
};

GLenum _blend_factor_look_up[10] =
{
    GL_ZERO, // Zero
    GL_ONE, // One
    GL_SRC_COLOR, // SourceColor
    GL_ONE_MINUS_SRC_COLOR, // OneMinusSourceColor
    GL_DST_COLOR, // DestinationColor
    GL_ONE_MINUS_DST_COLOR, // OneMinusDestinationColor
    GL_SRC_ALPHA, // SourceAlpha
    GL_ONE_MINUS_SRC_ALPHA, // OneMinusSourceAlpha
    GL_DST_ALPHA, // DestinationAlpha
    GL_ONE_MINUS_DST_ALPHA // OneMinusDestinationAlpha
};

GLenum _texture_filter_look_up[2] =
{
    GL_NEAREST, // Nearest
    GL_LINEAR // Linear
};

GLenum _texture_mipmap_filter_look_up[2] =
{
    GL_NEAREST_MIPMAP_NEAREST, // Nearest
    GL_LINEAR_MIPMAP_LINEAR // Linear
};

GLenum _pixel_type_look_up[3] =
{
    GL_UNSIGNED_BYTE, // Byte
    GL_HALF_FLOAT, // Float16
    GL_FLOAT // Float32
};

GLenum _pixel_format_look_up[5] =
{
    GLenum(-1), // 0
    GL_R, // 1
    GL_RG, // 2
    GL_RGB, // 3
    GL_RGBA // 4
};

GLenum _internal_image_format_look_up[2][5][3] =
{
    // NonLinear
    {
        // 0
        {
            GLenum(-1), // Byte
            GLenum(-1), // Float16
            GLenum(-1) // Float32
        },
        // 1
        {
            GLenum(-1), // Byte
            GLenum(-1), // Float16
            GLenum(-1) // Float32
        },
        // 2
        {
            GLenum(-1), // Byte
            GLenum(-1), // Float16
            GLenum(-1) // Float32
        },
        // 3
        {
            GL_SRGB8, // Byte
            GLenum(-1), // Float16
            GLenum(-1) // Float32
        },
        // 4
        {
            GL_SRGB8_ALPHA8, // Byte
            GLenum(-1), // Float16
            GLenum(-1) // Float32
        }
    },

    // Linear
    {
        // 0
        {
            GLenum(-1), // Byte
            GLenum(-1), // Float16
            GLenum(-1) // Float32
        },
        // 1
        {
            GL_R8, // Byte
            GL_R16F, // Float16
            GL_R32F // Float32
        },
        // 2
        {
            GL_RG8, // Byte
            GL_RG16F, // Float16
            GL_RG32F // Float32
        },
        // 3
        {
            GL_RGB8, // Byte
            GL_RGB16F, // Float16
            GL_RGB32F // Float32
        },
        // 4
        {
            GL_RGBA8, // Byte
            GL_RGBA16F, // Float16
            GL_RGBA32F // Float32
        }
    }
};

GLenum _shader_source_type_look_up[3] =
{
    GL_VERTEX_SHADER, // Vertex
    GL_FRAGMENT_SHADER, // Fragment
    GL_GEOMETRY_SHADER // Geometry
};

GLenum _frame_buffer_slot_look_up[18] =
{
    GL_COLOR_ATTACHMENT0, // Color0
    GL_COLOR_ATTACHMENT1, // Color1
    GL_COLOR_ATTACHMENT2, // Color2
    GL_COLOR_ATTACHMENT3, // Color3
    GL_COLOR_ATTACHMENT4, // Color4
    GL_COLOR_ATTACHMENT5, // Color5
    GL_COLOR_ATTACHMENT6, // Color6
    GL_COLOR_ATTACHMENT7, // Color7
    GL_COLOR_ATTACHMENT8, // Color8
    GL_COLOR_ATTACHMENT9, // Color9
    GL_COLOR_ATTACHMENT10, // Color10
    GL_COLOR_ATTACHMENT11, // Color11
    GL_COLOR_ATTACHMENT12, // Color12
    GL_COLOR_ATTACHMENT13, // Color13
    GL_COLOR_ATTACHMENT14, // Color14
    GL_COLOR_ATTACHMENT15, // Color15
    GL_DEPTH_ATTACHMENT // Depth
};

// Throws an error if an OpenGL error occurred
void check_g_l_error()
{
    GLenum error_code;
    if ((error_code = glGetError()) != GL_NO_ERROR)
    {
        throw FatalError(format("OpenGL error: %s", gluErrorString(error_code)));
    }
}

void upload_texture(Renderer& renderer, Texture2& texture, bool depth_component)
{
    if (texture.is_uploaded())
    {
        return;
    }

    GLuint texture_id = 0;
    GL_ASSERT(glGenTextures(1, &texture_id));
    GL_ASSERT(glBindTexture(GL_TEXTURE_2D, texture_id));
    GL_ASSERT(
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            texture.is_mipmapped() ?
            _texture_mipmap_filter_look_up[(int)texture.min_filter()] :
            _texture_filter_look_up[(int)texture.min_filter()]
        )
    );

    GL_ASSERT(
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            _texture_filter_look_up[(int)texture.mag_filter()]
        )
    );

    if (texture.is_wrapped())
    {
        GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    }
    else
    {
        GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    }

    Image& image = texture.image();
    const PixelFormat& pixel_format = image.pixel_format();

    GLint internal_format = _internal_image_format_look_up[(int)image.color_space()][(int)pixel_format.cardinality()][(int)pixel_format.type()];
    if (depth_component)
    {
        if (pixel_format.type() == PixelType::Float16)
        {
            internal_format = GL_DEPTH_COMPONENT16;
        }
        else if (pixel_format.type() == PixelType::Float32)
        {
            internal_format = GL_DEPTH_COMPONENT32;
        }
    }

    GLenum format = _pixel_format_look_up[(int)pixel_format.cardinality()];
    if (depth_component)
    {
        format = GL_DEPTH_COMPONENT;
    }

    GLenum type = _pixel_type_look_up[(int)pixel_format.type()];

    GL_ASSERT(
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            internal_format,
            image.width(),
            image.height(),
            0,
            format,
            type,
            image.has_pixel_data() ? &image.pixel_data()[0] : 0
        )
    );

    if (texture.is_mipmapped())
    {
        GL_ASSERT(glGenerateMipmap(GL_TEXTURE_2D));
    }

    GL_ASSERT(glBindTexture(GL_TEXTURE_2D, 0));

    texture.set_as_uploaded(renderer, new Texture2Data(renderer, texture, texture_id));
    renderer.statistics().memory_usage += texture.width() * texture.height() * texture.pixel_format().size();

    HECT_TRACE(::format("Uploaded texture '%s'", texture.name().data()));

    texture.invalidate_local_image();
}

}

void Renderer::upload_frame_buffer(FrameBuffer& frame_buffer)
{
    if (frame_buffer.is_uploaded())
    {
        return;
    }

    GLuint frame_buffer_id = 0;

    GL_ASSERT(glGenFramebuffers(1, &frame_buffer_id));
    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id));

    GLenum mrt[16];
    int texture_index = 0;

    // Attach all textures
    for (FrameBufferAttachment& attachment : frame_buffer.attachments())
    {
        const FrameBufferAttachmentType type = attachment.type();
        const FrameBufferSlot slot = attachment.slot();

        if (type == FrameBufferAttachmentType::Texture2)
        {
            Texture2& texture = attachment.texture2();

            // Upload the texture if needed
            if (!texture.is_uploaded())
            {
                bool depth_component = attachment.slot() == FrameBufferSlot::Depth;
                ::upload_texture(*this, texture, depth_component);
            }

            GLenum frame_buffer_attachment = _frame_buffer_slot_look_up[static_cast<int>(slot)];

            auto target_data = texture.data_as<Texture2Data>();
            GL_ASSERT(glFramebufferTexture(GL_FRAMEBUFFER, frame_buffer_attachment, target_data->texture_id, 0));

            if (slot != FrameBufferSlot::Depth)
            {
                mrt[texture_index++] = frame_buffer_attachment;
            }
        }
        else if (type == FrameBufferAttachmentType::Texture3)
        {
            Texture3& texture = attachment.texture3();

            // Upload the texture if needed
            if (!texture.is_uploaded())
            {
                upload_texture(texture);
            }

            GLenum frame_buffer_attachment = _frame_buffer_slot_look_up[static_cast<int>(slot)];

            auto target_data = texture.data_as<Texture3Data>();
            GL_ASSERT(glFramebufferTexture(GL_FRAMEBUFFER, frame_buffer_attachment, target_data->texture_id, 0));

            mrt[texture_index++] = frame_buffer_attachment;
        }
        else if (type == FrameBufferAttachmentType::TextureCube)
        {
            CubeSide side = attachment.side();
            TextureCube& texture = attachment.texture_cube();

            // Upload the texture if needed
            if (!texture.is_uploaded())
            {
                upload_texture(texture);
            }

            GLenum frame_buffer_attachment = _frame_buffer_slot_look_up[static_cast<int>(slot)];
            GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<int>(side);

            auto target_data = texture.data_as<TextureCubeData>();
            GL_ASSERT(glFramebufferTexture2D(GL_FRAMEBUFFER, frame_buffer_attachment, target, target_data->texture_id, 0));

            mrt[texture_index++] = frame_buffer_attachment;
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw InvalidOperation("Invalid frame buffer");
        }
    }

    GL_ASSERT(glDrawBuffers(texture_index, mrt));

    frame_buffer.set_as_uploaded(*this, new FrameBufferData(*this, frame_buffer, frame_buffer_id));

    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Renderer::destroy_frame_buffer(FrameBuffer& frame_buffer)
{
    if (!frame_buffer.is_uploaded())
    {
        return;
    }

    auto data = frame_buffer.data_as<FrameBufferData>();

    GL_ASSERT(glDeleteFramebuffers(1, &data->frame_buffer_id));

    frame_buffer.set_as_destroyed();
}

void Renderer::upload_shader(Shader& shader)
{
    if (shader.is_uploaded())
    {
        return;
    }

    // Create the shader
    GLuint program_id = GL_ASSERT(glCreateProgram());

    // Attach each shader to the program
    std::vector<GLuint> shader_ids;
    for (const ShaderModule& module : shader.modules())
    {
        GLuint shader_id;

        // Create the shader
        GL_ASSERT(shader_id = glCreateShader(_shader_source_type_look_up[static_cast<int>(module.type())]));

        // Compile shader
        const GLchar* source = module.source().data();
        GL_ASSERT(glShaderSource(shader_id, 1, &source, nullptr));
        GL_ASSERT(glCompileShader(shader_id));

        // Report errors
        int log_length = 0;
        GL_ASSERT(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length));
        if (log_length > 1)
        {
            int chars_written = 0;
            std::string info_log(log_length, ' ');
            GL_ASSERT(glGetShaderInfoLog(shader_id, log_length, &chars_written, &info_log[0]));

            if (info_log.size() > 0)
            {
                throw InvalidOperation(format("Invalid shader: Failed to compile GLSL shader source file '%s': %s", module.name().data(), info_log.data()));
            }
        }

        GL_ASSERT(glAttachShader(program_id, shader_id));
        shader_ids.push_back(shader_id);
    }

    // Link program
    GL_ASSERT(glLinkProgram(program_id));

    // Report errors
    int log_length = 0;
    GL_ASSERT(glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length));
    if (log_length > 1)
    {
        int chars_written = 0;
        std::string info_log(log_length, ' ');
        GL_ASSERT(glGetProgramInfoLog(program_id, log_length, &chars_written, &info_log[0]));

        if (info_log.size() > 0)
        {
            throw InvalidOperation(format("Invalid shader: Failed to link GLSL shaders for shader '%s': %s", shader.name().data(), info_log.data()));
        }
    }

    GL_ASSERT(glUseProgram(program_id));

    // Get the locations of each uniform
    for (Uniform& uniform : shader.uniforms())
    {
        GL_ASSERT(int location = glGetUniformLocation(program_id, uniform.name().data()));

        if (location != -1)
        {
            uniform.set_location(location);
        }
        else
        {
            HECT_WARNING(format("Uniform '%s' is not referenced in shader '%s'", uniform.name().data(), shader.name().data()));
        }
    }

    GL_ASSERT(glUseProgram(0));

    shader.set_as_uploaded(*this, new ShaderData(*this, shader, program_id, shader_ids));

    HECT_TRACE(format("Uploaded shader '%s'", shader.name().data()));
}

void Renderer::destroy_shader(Shader& shader)
{
    if (!shader.is_uploaded())
    {
        return;
    }

    auto data = shader.data_as<ShaderData>();

    // Destroy all shaders in the program
    for (GLuint shader_id : data->shader_ids)
    {
        GL_ASSERT(glDetachShader(data->program_id, shader_id));
        GL_ASSERT(glDeleteShader(shader_id));
    }

    // Delete the program
    GL_ASSERT(glDeleteProgram(data->program_id));

    shader.set_as_destroyed();

    HECT_TRACE(format("Destroyed shader '%s'", shader.name().data()));
}

void Renderer::upload_texture(Texture2& texture)
{
    ::upload_texture(*this, texture, false);
}

void Renderer::upload_texture(Texture3& texture)
{
    if (texture.is_uploaded())
    {
        return;
    }

    GLuint texture_id = 0;
    GL_ASSERT(glGenTextures(1, &texture_id));
    GL_ASSERT(glBindTexture(GL_TEXTURE_3D, texture_id));
    GL_ASSERT(
        glTexParameteri(
            GL_TEXTURE_3D,
            GL_TEXTURE_MIN_FILTER,
            texture.is_mipmapped() ?
            _texture_mipmap_filter_look_up[(int)texture.min_filter()] :
            _texture_filter_look_up[(int)texture.min_filter()]
        )
    );

    GL_ASSERT(
        glTexParameteri(
            GL_TEXTURE_3D,
            GL_TEXTURE_MAG_FILTER,
            _texture_filter_look_up[(int)texture.mag_filter()]
        )
    );

    if (texture.is_wrapped())
    {
        GL_ASSERT(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GL_ASSERT(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GL_ASSERT(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT));
    }
    else
    {
        GL_ASSERT(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GL_ASSERT(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        GL_ASSERT(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER));
    }

    const PixelFormat& pixel_format = texture.pixel_format();

    unsigned width = texture.width();
    unsigned height = texture.height();
    unsigned depth = texture.depth();

    GLint internal_format = _internal_image_format_look_up[(int)ColorSpace::Linear][(int)pixel_format.cardinality()][(int)pixel_format.type()];
    GLenum format = _pixel_format_look_up[(int)pixel_format.cardinality()];
    GLenum type = _pixel_type_look_up[(int)pixel_format.type()];

    // Build a vector of all pixel data for each depth in the 3-dimensional
    // texture
    ByteVector pixel_data;
    for (unsigned i = 0; i < depth; ++i)
    {
        // Get the image at this depth
        Image& image = texture.image(i);

        // If any of the images do not have pixel data then don't upload any
        // pixel data
        if (!image.has_pixel_data())
        {
            pixel_data.clear();
            break;
        }
        else
        {
            // Append the images pixel data to the vector
            ByteVector& image_pixel_data = image.pixel_data();
            pixel_data.insert(pixel_data.end(), image_pixel_data.begin(), image_pixel_data.end());
        }
    }

    GL_ASSERT(glTexImage3D(GL_TEXTURE_3D, 0, internal_format, width, height, depth, 0, format, type, !pixel_data.empty() ? &pixel_data[0] : nullptr));

    if (texture.is_mipmapped())
    {
        GL_ASSERT(glGenerateMipmap(GL_TEXTURE_3D));
    }

    GL_ASSERT(glBindTexture(GL_TEXTURE_3D, 0));

    texture.set_as_uploaded(*this, new Texture3Data(*this, texture, texture_id));
    statistics().memory_usage += texture.width() * texture.height() * texture.depth() * texture.pixel_format().size();

    HECT_TRACE(::format("Uploaded texture '%s'", texture.name().data()));

    texture.invalidate_local_images();
}

void Renderer::upload_texture(TextureCube& texture)
{
    if (texture.is_uploaded())
    {
        return;
    }

    GLenum type = GL_TEXTURE_CUBE_MAP;

    GLuint texture_id = 0;
    GL_ASSERT(glGenTextures(1, &texture_id));
    GL_ASSERT(glBindTexture(type, texture_id));
    GL_ASSERT(
        glTexParameteri(
            type,
            GL_TEXTURE_MIN_FILTER,
            texture.is_mipmapped() ?
            _texture_mipmap_filter_look_up[(int)texture.min_filter()] :
            _texture_filter_look_up[(int)texture.min_filter()]
        )
    );

    GL_ASSERT(
        glTexParameteri(
            type,
            GL_TEXTURE_MAG_FILTER,
            _texture_filter_look_up[(int)texture.mag_filter()]
        )
    );

    GL_ASSERT(glTexParameterf(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_ASSERT(glTexParameterf(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    static const std::vector<CubeSide> sides =
    {
        CubeSide::PositiveX,
        CubeSide::NegativeX,
        CubeSide::PositiveY,
        CubeSide::NegativeY,
        CubeSide::PositiveZ,
        CubeSide::NegativeZ
    };

    for (CubeSide side : sides)
    {
        Image& image = texture.image(side);
        const PixelFormat& pixel_format = image.pixel_format();

        GL_ASSERT(
            glTexImage2D(
                target,
                0,
                _internal_image_format_look_up[(int)image.color_space()][(int)pixel_format.cardinality()][(int)pixel_format.type()],
                image.width(),
                image.height(),
                0,
                _pixel_format_look_up[(int)pixel_format.cardinality()],
                _pixel_type_look_up[(int)pixel_format.type()],
                image.has_pixel_data() ? &image.pixel_data()[0] : 0
            )
        );

        ++target;
    }

    if (texture.is_mipmapped())
    {
        GL_ASSERT(glGenerateMipmap(type));
    }

    GL_ASSERT(glBindTexture(type, 0));

    texture.set_as_uploaded(*this, new TextureCubeData(*this, texture, texture_id));
    statistics().memory_usage += texture.width() * texture.height() * texture.pixel_format().size();

    HECT_TRACE(format("Uploaded texture '%s'", texture.name().data()));

    texture.invalidate_local_images();
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

    auto data = texture.data_as<Texture2Data>();
    GL_ASSERT(glDeleteTextures(1, &data->texture_id));

    statistics().memory_usage -= texture.width() * texture.height() * texture.pixel_format().size();
    texture.set_as_destroyed();

    HECT_TRACE(format("Destroyed texture '%s'", texture.name().data()));
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

    auto data = texture.data_as<Texture3Data>();
    GL_ASSERT(glDeleteTextures(1, &data->texture_id));

    statistics().memory_usage -= texture.width() * texture.height() * texture.depth() * texture.pixel_format().size();
    texture.set_as_destroyed();

    HECT_TRACE(format("Destroyed texture '%s'", texture.name().data()));
}

void Renderer::destroy_texture(TextureCube& texture, bool download_image)
{
    if (download_image)
    {
        throw InvalidOperation();
    }

    if (!texture.is_uploaded())
    {
        return;
    }

    auto data = texture.data_as<TextureCubeData>();
    GL_ASSERT(glDeleteTextures(1, &data->texture_id));

    statistics().memory_usage -= texture.width() * texture.height() * texture.pixel_format().size();
    texture.set_as_destroyed();

    HECT_TRACE(format("Destroyed texture '%s'", texture.name().data()));
}

void Renderer::download_texture_image(Texture2& texture)
{
    if (!texture.is_uploaded())
    {
        throw InvalidOperation("The texture is not uploaded");
    }

    auto data = texture.data_as<Texture2Data>();

    GL_ASSERT(glBindTexture(GL_TEXTURE_2D, data->texture_id));

    unsigned width = texture.width();
    unsigned height = texture.height();
    const PixelFormat& pixel_format = texture.pixel_format();

    // Get the texture's image
    Image& image = texture.image();

    // If the image has no pixel data
    if (!image.has_pixel_data())
    {
        // Allocate the expected amount of pixel data
        ByteVector pixel_data(pixel_format.size() * width * height, 0);
        image.set_pixel_data(std::move(pixel_data));
    }
    GL_ASSERT(
        glGetTexImage(
            GL_TEXTURE_2D,
            0,
            _pixel_format_look_up[(int)texture.pixel_format().cardinality()],
            _pixel_type_look_up[(int)texture.pixel_format().type()],
            &image.pixel_data()[0]
        )
    );

    GL_ASSERT(glBindTexture(GL_TEXTURE_2D, 0));
}

void Renderer::download_texture_images(Texture3& texture)
{
    if (!texture.is_uploaded())
    {
        throw InvalidOperation("The texture is not uploaded");
    }

    auto data = texture.data_as<Texture3Data>();

    GL_ASSERT(glBindTexture(GL_TEXTURE_3D, data->texture_id));

    unsigned width = texture.width();
    unsigned height = texture.height();
    unsigned depth = texture.depth();
    const PixelFormat& pixel_format = texture.pixel_format();

    // Allocate the expected amount of pixel data
    ByteVector pixel_data(width * height * depth * pixel_format.size());

    GL_ASSERT(
        glGetTexImage(
            GL_TEXTURE_3D,
            0,
            _pixel_format_look_up[(int)texture.pixel_format().cardinality()],
            _pixel_type_look_up[(int)texture.pixel_format().type()],
            &pixel_data[0]
        )
    );

    // For each layer
    unsigned size_per_layer = width * height * pixel_format.size();
    for (unsigned z = 0; z < depth; ++z)
    {
        // Copy the pixel data for this layer
        auto first = pixel_data.begin() + size_per_layer * z;
        auto last = pixel_data.begin() + size_per_layer * (z + 1);
        ByteVector image_pixel_data(first, last);

        // Set the pixel data for this layer
        Image& image = texture.image(z);
        image.set_pixel_data(std::move(image_pixel_data));
    }

    GL_ASSERT(glBindTexture(GL_TEXTURE_3D, 0));
}

void Renderer::upload_mesh(Mesh& mesh)
{
    if (mesh.is_uploaded())
    {
        return;
    }

    GLuint vertex_array_id = 0;
    GLuint vertex_buffer_id = 0;
    GLuint index_buffer_id = 0;

    // Generate and bind the vertex array
    GL_ASSERT(glGenVertexArrays(1, &vertex_array_id));
    GL_ASSERT(glBindVertexArray(vertex_array_id));

    // Generate vertex and index buffers
    GL_ASSERT(glGenBuffers(1, &vertex_buffer_id));
    GL_ASSERT(glGenBuffers(1, &index_buffer_id));

    // Upload the vertex data
    GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id));
    GL_ASSERT(
        glBufferData(
            GL_ARRAY_BUFFER,
            mesh.vertex_layout().vertex_size() * mesh.vertex_count(),
            !mesh.vertex_data().empty() ? &mesh.vertex_data()[0] : nullptr,
            GL_STATIC_DRAW
        )
    );

    // Describe the vertex layout
    GLuint attribute_index = 0;
    const VertexLayout& vertex_layout = mesh.vertex_layout();
    for (const VertexAttribute& attribute : vertex_layout.attributes())
    {
        GL_ASSERT(glEnableVertexAttribArray(attribute_index));

        size_t offset = attribute.offset();

        if (attribute.type() == VertexAttributeType::Float32)
        {

            GL_ASSERT(
                glVertexAttribPointer(
                    attribute_index,
                    attribute.cardinality(),
                    _vertex_attribute_type_look_up[(int)attribute.type()],
                    GL_FALSE,
                    vertex_layout.vertex_size(),
                    reinterpret_cast<GLvoid*>(offset)
                )
            );
        }
        else
        {
            GL_ASSERT(
                glVertexAttribIPointer(
                    attribute_index,
                    attribute.cardinality(),
                    _vertex_attribute_type_look_up[(int)attribute.type()],
                    vertex_layout.vertex_size(),
                    reinterpret_cast<GLvoid*>(offset)
                )
            );
        }

        ++attribute_index;
    }

    // Upload the index data
    GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id));
    GL_ASSERT(
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            mesh.index_size() * mesh.index_count(),
            !mesh.index_data().empty() ? &mesh.index_data()[0] : nullptr,
            GL_STATIC_DRAW
        )
    );

    GL_ASSERT(glBindVertexArray(0));

    mesh.set_as_uploaded(*this, new MeshData(*this, mesh, vertex_array_id, vertex_buffer_id, index_buffer_id));
    _uploaded_meshes.insert(&mesh);

    HECT_TRACE(format("Uploaded mesh '%s'", mesh.name().data()));
}

void Renderer::destroy_mesh(Mesh& mesh)
{
    if (!mesh.is_uploaded())
    {
        return;
    }

    auto data = mesh.data_as<MeshData>();

    // Delete vertex and index buffers
    GL_ASSERT(glDeleteBuffers(2, &data->vertex_buffer_id));

    // Delete the vertex array object
    GL_ASSERT(glDeleteVertexArrays(1, &data->vertex_array_id));

    mesh.set_as_destroyed();
    _uploaded_meshes.erase(&mesh);

    HECT_TRACE(format("Destroyed mesh '%s'", mesh.name().data()));
}

void Renderer::initialize()
{
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        const char* error_string = reinterpret_cast<const char*>(glewGetErrorString(error));
        throw FatalError(format("Failed to initialize OpenGL: %s", error_string));
    }

    HECT_INFO(format("OpenGL version %s", glGetString(GL_VERSION)));
    HECT_INFO(format("GLSL version %s", glGetString(GL_SHADING_LANGUAGE_VERSION)));
    HECT_INFO(format("%s", glGetString(GL_VENDOR)));
    HECT_INFO(format("%s", glGetString(GL_RENDERER)));

    GLint max_texture_units = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
    capabilities().max_texture_units = (unsigned)max_texture_units;

    HECT_INFO(format("Max texture units: %d", capabilities().max_texture_units));

    glGetError(); // Clear errors

    // Enable 3-dimensional texturing
    glEnable(GL_TEXTURE_3D);

    // Set up the cube map rendering profile
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Create the mesh used to in Renderer::Frame::render_viewport()
    _viewport_mesh = create_viewport_mesh();

    check_g_l_error();
}

void Renderer::shutdown()
{
    const std::vector<Mesh*> meshes_to_destroy(_uploaded_meshes.begin(), _uploaded_meshes.end());
    for (Mesh* mesh : meshes_to_destroy)
    {
        if (mesh->is_uploaded())
        {
            destroy_mesh(*mesh);
        }
    }
}

void Renderer::on_begin_frame(RenderTarget& target)
{
    GL_ASSERT(glUseProgram(0));
    GL_ASSERT(glBindVertexArray(0));

    for (unsigned i = 0; i < capabilities().max_texture_units; ++i)
    {
        GL_ASSERT(glActiveTexture(GL_TEXTURE0 + i));
        GL_ASSERT(glBindTexture(GL_TEXTURE_2D, 0));
    }

    GL_ASSERT(glDisable(GL_BLEND));
    GL_ASSERT(glEnable(GL_DEPTH_TEST));
    GL_ASSERT(glDepthMask(GL_TRUE));

    set_target(target);
}

void Renderer::on_end_frame()
{
    if (_current_frame_buffer)
    {
        for (FrameBufferAttachment& attachment : _current_frame_buffer->attachments())
        {
            const FrameBufferAttachmentType type = attachment.type();
            if (type == FrameBufferAttachmentType::Texture2)
            {
                Texture2& texture = attachment.texture2();
                if (texture.is_mipmapped())
                {
                    GLenum texture_type = GL_TEXTURE_2D;

                    auto texture_data = texture.data_as<Texture2Data>();
                    GL_ASSERT(glBindTexture(texture_type, texture_data->texture_id));
                    GL_ASSERT(glGenerateMipmap(texture_type));
                }
            }
            else if (type == FrameBufferAttachmentType::Texture3)
            {
                Texture3& texture = attachment.texture3();
                if (texture.is_mipmapped())
                {
                    GLenum texture_type = GL_TEXTURE_3D;

                    auto texture_data = texture.data_as<Texture3Data>();
                    GL_ASSERT(glBindTexture(texture_type, texture_data->texture_id));
                    GL_ASSERT(glGenerateMipmap(texture_type));
                }
            }
            else if (type == FrameBufferAttachmentType::TextureCube)
            {
                TextureCube& texture = attachment.texture_cube();
                if (texture.is_mipmapped())
                {
                    GLenum texture_type = GL_TEXTURE_CUBE_MAP;

                    auto texture_data = texture.data_as<TextureCubeData>();
                    GL_ASSERT(glBindTexture(texture_type, texture_data->texture_id));
                    GL_ASSERT(glGenerateMipmap(texture_type));
                }
            }
        }

        _current_frame_buffer = nullptr;
    }
}

void Renderer::set_target(RenderTarget& render_target)
{
    render_target.bind(*this);
}

void Renderer::set_target(Window& window)
{
    GL_ASSERT(glViewport(0, 0, window.width(), window.height()));
    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Renderer::set_target(FrameBuffer& frame_buffer)
{
    _current_frame_buffer = &frame_buffer;

    if (!frame_buffer.is_uploaded())
    {
        upload_frame_buffer(frame_buffer);
    }

    // Invalidate local images for textures
    for (FrameBufferAttachment& attachment : frame_buffer.attachments())
    {
        FrameBufferAttachmentType type = attachment.type();
        if (type == FrameBufferAttachmentType::Texture2)
        {
            attachment.texture2().invalidate_local_image();
        }
        else if (type == FrameBufferAttachmentType::Texture2)
        {
            attachment.texture3().invalidate_local_images();
        }
        else if (type == FrameBufferAttachmentType::Texture2)
        {
            attachment.texture_cube().invalidate_local_images();
        }
    }

    auto data = frame_buffer.data_as<FrameBufferData>();

    GL_ASSERT(glViewport(0, 0, frame_buffer.width(), frame_buffer.height()));
    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, data->frame_buffer_id));
}

void Renderer::set_cull_mode(CullMode cull_mode)
{
    switch (cull_mode)
    {
    case CullMode::CounterClockwise:
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        break;
    case CullMode::Clockwise:
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        break;
    case CullMode::None:
        glDisable(GL_CULL_FACE);
        break;
    }
}

void Renderer::set_shader(Shader& shader)
{
    // Upload the shader if needed
    if (!shader.is_uploaded())
    {
        upload_shader(shader);
    }

    // Bind the shader
    auto data = shader.data_as<ShaderData>();
    GL_ASSERT(glUseProgram(data->program_id));

    // If the blend mode is non-trivial
    const BlendMode& blend_mode = shader.blend_mode();
    if (blend_mode != BlendMode())
    {
        GL_ASSERT(glEnable(GL_BLEND));

        // Set the blend function
        GLenum function = _blend_function_look_up[static_cast<GLenum>(blend_mode.function())];
        GL_ASSERT(glBlendEquation(function));

        // Set the blend factors
        GLenum source_factor = _blend_factor_look_up[static_cast<GLenum>(blend_mode.source_factor())];
        GLenum destination_factor = _blend_factor_look_up[static_cast<GLenum>(blend_mode.destination_factor())];
        GL_ASSERT(glBlendFunc(source_factor, destination_factor));
    }
    else
    {
        GL_ASSERT(glDisable(GL_BLEND));
    }

    // Enable or disable depth testing
    if (shader.is_depth_tested())
    {
        GL_ASSERT(glEnable(GL_DEPTH_TEST));
        GL_ASSERT(glDepthMask(GL_TRUE));
    }
    else
    {
        GL_ASSERT(glDisable(GL_DEPTH_TEST));
        GL_ASSERT(glDepthMask(GL_FALSE));
    }
}

void Renderer::set_uniform(const Uniform& uniform, int value)
{
    const int location = uniform.location();
    if (location >= 0)
    {
        GL_ASSERT(glUniform1i(location, value));
    }
}

void Renderer::set_uniform(const Uniform& uniform, double value)
{
    const int location = uniform.location();
    if (location >= 0)
    {
        GLfloat temp = GLfloat(value);
        GL_ASSERT(glUniform1f(location, temp));
    }
}

void Renderer::set_uniform(const Uniform& uniform, Vector2 value)
{
    const int location = uniform.location();
    if (location >= 0)
    {
        GLfloat temp[2] = { GLfloat(value[0]), GLfloat(value[1]) };
        GL_ASSERT(glUniform2fv(location, 1, temp));
    }
}

void Renderer::set_uniform(const Uniform& uniform, Vector3 value)
{
    const int location = uniform.location();
    if (location >= 0)
    {
        GLfloat temp[3] = { GLfloat(value[0]), GLfloat(value[1]), GLfloat(value[2]) };
        GL_ASSERT(glUniform3fv(location, 1, temp));
    }
}

void Renderer::set_uniform(const Uniform& uniform, Vector4 value)
{
    const int location = uniform.location();
    if (location >= 0)
    {
        GLfloat temp[4] = { GLfloat(value[0]), GLfloat(value[1]), GLfloat(value[2]), GLfloat(value[3]) };
        GL_ASSERT(glUniform4fv(location, 1, temp));
    }
}

void Renderer::set_uniform(const Uniform& uniform, const Matrix4& value)
{
    const int location = uniform.location();
    if (location >= 0)
    {
        GLfloat temp[16] =
        {
            GLfloat(value[0]), GLfloat(value[1]), GLfloat(value[2]), GLfloat(value[3]),
            GLfloat(value[4]), GLfloat(value[5]), GLfloat(value[6]), GLfloat(value[7]),
            GLfloat(value[8]), GLfloat(value[9]), GLfloat(value[10]), GLfloat(value[11]),
            GLfloat(value[12]), GLfloat(value[13]), GLfloat(value[14]), GLfloat(value[15])
        };

        GL_ASSERT(glUniformMatrix4fv(location, 1, false, temp));
    }
}

void Renderer::set_uniform(const Uniform& uniform, Color value)
{
    const int location = uniform.location();
    if (location >= 0)
    {
        GLfloat temp[4] = { GLfloat(value[0]), GLfloat(value[1]), GLfloat(value[2]), GLfloat(value[3]) };
        GL_ASSERT(glUniform4fv(location, 1, temp));
    }
}

void Renderer::set_uniform(const Uniform& uniform, Texture2& value)
{
    const int location = uniform.location();
    if (location >= 0)
    {
        GL_ASSERT(glUniform1i(location, static_cast<GLint>(uniform.texture_index())));
        TextureIndex index = uniform.texture_index();

        if (index >= capabilities().max_texture_units)
        {
            throw InvalidOperation("Cannot bind a texture unit beyond hardware capabilities");
        }

        if (!value.is_uploaded())
        {
            upload_texture(value);
        }

        auto data = value.data_as<Texture2Data>();

        GL_ASSERT(glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(index)));
        GL_ASSERT(glBindTexture(GL_TEXTURE_2D, data->texture_id));
    }
}

void Renderer::set_uniform(const Uniform& uniform, Texture3& value)
{
    const int location = uniform.location();
    if (location >= 0)
    {
        GL_ASSERT(glUniform1i(location, static_cast<GLint>(uniform.texture_index())));
        TextureIndex index = uniform.texture_index();

        if (index >= capabilities().max_texture_units)
        {
            throw InvalidOperation("Cannot bind a texture unit beyond hardware capabilities");
        }

        if (!value.is_uploaded())
        {
            upload_texture(value);
        }

        auto data = value.data_as<Texture3Data>();

        GL_ASSERT(glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(index)));
        GL_ASSERT(glBindTexture(GL_TEXTURE_3D, data->texture_id));
    }
}

void Renderer::set_uniform(const Uniform& uniform, TextureCube& value)
{
    const int location = uniform.location();
    if (location >= 0)
    {
        GL_ASSERT(glUniform1i(location, static_cast<GLint>(uniform.texture_index())));
        TextureIndex index = uniform.texture_index();

        if (index >= capabilities().max_texture_units)
        {
            throw InvalidOperation("Cannot bind a texture unit beyond hardware capabilities");
        }

        if (!value.is_uploaded())
        {
            upload_texture(value);
        }

        auto data = value.data_as<TextureCubeData>();

        GL_ASSERT(glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(index)));
        GL_ASSERT(glBindTexture(GL_TEXTURE_CUBE_MAP, data->texture_id));
    }
}

void Renderer::render_mesh(Mesh& mesh)
{
    if (!mesh.is_uploaded())
    {
        upload_mesh(mesh);
    }

    if (mesh.primitive_type() == PrimitiveType::PointSprites)
    {
        // Set up the point rendering profile
        GL_ASSERT(glEnable(GL_PROGRAM_POINT_SIZE));
        GL_ASSERT(glEnable(GL_POINT_SPRITE));
        GL_ASSERT(glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT));
    }
    else
    {
        GL_ASSERT(glDisable(GL_PROGRAM_POINT_SIZE));
        GL_ASSERT(glDisable(GL_POINT_SPRITE));
    }

    auto data = mesh.data_as<MeshData>();
    GL_ASSERT(glBindVertexArray(data->vertex_array_id));

    GL_ASSERT(
        glDrawElements(
            _primitive_type_look_up[static_cast<int>(mesh.primitive_type())],
            static_cast<GLsizei>(mesh.index_count()),
            _index_type_look_up[static_cast<int>(mesh.index_type())],
            0
        )
    );
}

void Renderer::render_viewport()
{
    render_mesh(_viewport_mesh);
}

void Renderer::clear(Color color, bool depth)
{
    GL_ASSERT(glClearColor(static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b), static_cast<float>(color.a)));
    GL_ASSERT(glClear(depth ? (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) : GL_COLOR_BUFFER_BIT));
}

#endif
