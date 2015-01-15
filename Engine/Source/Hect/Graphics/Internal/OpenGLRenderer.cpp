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
#include "OpenGLRenderer.h"

#ifdef HECT_RENDERER_OPENGL

#define NANOVG_GLEW
#define NANOVG_GL3_IMPLEMENTATION

#include <algorithm>
#include <cassert>
#include <GL/glew.h>
#include <nanovg_gl.h>

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"
#include "Hect/Graphics/Font.h"
#include "Hect/Graphics/FrameBuffer.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Graphics/Shader.h"
#include "Hect/Graphics/Texture.h"
#include "Hect/Runtime/Window.h"

using namespace hect;

#ifdef HECT_DEBUG_BUILD
#define GL_ASSERT(expression) expression; _checkGLError()
#else
#define GL_ASSERT(expression) expression
#endif

namespace
{

// OpenGL-specific data for a shader program
class ShaderData :
    public Renderer::Data<Shader>
{
public:
    ShaderData(Renderer& renderer, Shader& object, GLuint programId, const std::vector<GLuint>& shaderIds) :
        Renderer::Data<Shader>(renderer, object),
        programId(programId),
        shaderIds(shaderIds)
    {
    }

    ~ShaderData()
    {
        // Destroy the shader if it is uploaded
        if (object && object->isUploaded())
        {
            renderer->destroyShader(*object);
        }
    }

    GLuint programId;
    std::vector<GLuint> shaderIds;
};

// OpenGL-specific data for a texture
class TextureData :
    public Renderer::Data<Texture>
{
public:
    TextureData(Renderer& renderer, Texture& object, GLuint textureId) :
        Renderer::Data<Texture>(renderer, object),
        textureId(textureId)
    {
    }

    ~TextureData()
    {
        // Destroy the texture if it is uploaded
        if (object && object->isUploaded())
        {
            renderer->destroyTexture(*object);
        }
    }

    GLuint textureId;
};

// OpenGL-specific data for a frame buffer
class FrameBufferData :
    public Renderer::Data<FrameBuffer>
{
public:
    FrameBufferData(Renderer& renderer, FrameBuffer& object, GLuint frameBufferId) :
        Renderer::Data<FrameBuffer>(renderer, object),
        frameBufferId(frameBufferId)
    {
    }

    ~FrameBufferData()
    {
        // Destroy the frame buffer if it is uploaded
        if (object && object->isUploaded())
        {
            renderer->destroyFrameBuffer(*object);
        }
    }

    GLuint frameBufferId;
};

// OpenGL-specific data for a render buffer
class RenderBufferData :
    public Renderer::Data<RenderBuffer>
{
public:
    RenderBufferData(Renderer& renderer, RenderBuffer& object, GLuint renderBufferId) :
        Renderer::Data<RenderBuffer>(renderer, object),
        renderBufferId(renderBufferId)
    {
    }

    ~RenderBufferData()
    {
        // Destroy the frame buffer if it is uploaded
        if (object && object->isUploaded())
        {
            renderer->destroyRenderBuffer(*object);
        }
    }

    GLuint renderBufferId;
};

// OpenGL-specific data for a mesh
class MeshData :
    public Renderer::Data<Mesh>
{
public:
    MeshData(Renderer& renderer, Mesh& object, GLuint vertexArrayId, GLuint vertexBufferId, GLuint indexBufferId) :
        Renderer::Data<Mesh>(renderer, object),
        vertexArrayId(vertexArrayId),
        vertexBufferId(vertexBufferId),
        indexBufferId(indexBufferId)
    {
    }

    ~MeshData()
    {
        // Destroy the mesh if it is uploaded
        if (object && object->isUploaded())
        {
            renderer->destroyMesh(*object);
        }
    }

    GLuint vertexArrayId;
    GLuint vertexBufferId;
    GLuint indexBufferId;
};

class FontData :
    public Renderer::Data<Font>
{
public:
    FontData(Renderer& renderer, Font& object, int fontId) :
        Renderer::Data<Font>(renderer, object),
        fontId(fontId)
    {
    }

    ~FontData()
    {
        if (object && object->isUploaded())
        {
            renderer->destroyFont(*object);
        }
    }

    int fontId;
};

// Throws an error if an OpenGL error occurred
void _checkGLError()
{
    GLenum errorCode;
    if ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        throw FatalError(format("OpenGL error: %s", gluErrorString(errorCode)));
    }
}

GLenum _vertexAttributeTypeLookUp[8] =
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

GLenum _indexTypeLookUp[3] =
{
    GL_UNSIGNED_BYTE, // UInt8
    GL_UNSIGNED_SHORT, // UInt16
    GL_UNSIGNED_INT // UInt32
};

GLenum _primitiveTypeLookUp[5] =
{
    GL_TRIANGLES, // Triangles
    GL_TRIANGLE_STRIP, // TriangleStrip
    GL_LINES, // Lines
    GL_LINE_STRIP, // LineStrip
    GL_POINTS // Points
};

GLenum _blendFunctionLookUp[2] =
{
    GL_FUNC_ADD, // Add
    GL_FUNC_SUBTRACT // Subtract
};

GLenum _blendFactorLookUp[10] =
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

GLenum _textureFilterLookUp[2] =
{
    GL_NEAREST, // Nearest
    GL_LINEAR // Linear
};

GLenum _textureMipmapFilterLookUp[2] =
{
    GL_NEAREST_MIPMAP_NEAREST, // Nearest
    GL_LINEAR_MIPMAP_LINEAR // Linear
};

GLenum _pixelTypeLookUp[3] =
{
    GL_UNSIGNED_BYTE, // Byte
    GL_HALF_FLOAT, // Float16
    GL_FLOAT // Float32
};

GLenum _pixelFormatLookUp[2] =
{
    GL_RGB, // Rgb
    GL_RGBA // Rgba
};

GLenum _internalImageFormatLookUp[2][2][3] =
{
    // NonLinear
    {
        // Rgb
        {
            GL_SRGB8, // Byte
            GL_RGB16F, // Float16
            GL_RGB32F // Float32
        },
        // Rgba
        {
            GL_SRGB8_ALPHA8, // Byte
            GL_RGBA16F, // Float16
            GL_RGBA32F // Float32
        }
    },

    // Linear
    {
        // Rgb
        {
            GL_RGB8, // Byte
            GL_RGB16F, // Float16
            GL_RGB32F // Float32
        },
        // Rgba
        {
            GL_RGBA8, // Byte
            GL_RGBA16F, // Float16
            GL_RGBA32F // Float32
        }
    }
};

GLenum _shaderSourceTypeLookUp[3] =
{
    GL_VERTEX_SHADER, // Vertex
    GL_FRAGMENT_SHADER, // Fragment
    GL_GEOMETRY_SHADER // Geometry
};

GLenum _textureTypeLookUp[3] =
{
    GL_TEXTURE_2D,
    GL_TEXTURE_CUBE_MAP
};

GLenum _renderBufferFormatLookUp[1] =
{
    GL_DEPTH_COMPONENT // DepthComponent
};

GLenum _frameBufferSlotLookUp[18] =
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
    GL_DEPTH_ATTACHMENT, // Depth
    GL_STENCIL_ATTACHMENT // Stencil
};

}

OpenGLRenderer::OpenGLRenderer()
{
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        const char* errorString = reinterpret_cast<const char*>(glewGetErrorString(error));
        throw FatalError(format("Failed to initialize OpenGL: %s", errorString));
    }

    HECT_INFO(format("OpenGL version %s", glGetString(GL_VERSION)));
    HECT_INFO(format("GLSL version %s", glGetString(GL_SHADING_LANGUAGE_VERSION)));
    HECT_INFO(format("%s", glGetString(GL_VENDOR)));
    HECT_INFO(format("%s", glGetString(GL_RENDERER)));

    GLint maxTextureUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    capabilities().maxTextureUnits = (unsigned)maxTextureUnits;

    HECT_INFO(format("Max texture units: %d", capabilities().maxTextureUnits));

    glGetError(); // Clear errors

    GL_ASSERT(glClearColor(0, 0, 0, 0));

    // Set up the point rendering profile
    GL_ASSERT(glEnable(GL_PROGRAM_POINT_SIZE));
    GL_ASSERT(glEnable(GL_POINT_SPRITE));
    GL_ASSERT(glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT));

    // Set up the cube map rendering profile
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Initialize NanoVG
#ifdef HECT_DEBUG_BUILD
    _nvgContext = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#else
    _nvgContext = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#endif
    if (!_nvgContext)
    {
        throw FatalError("Failed to initialize NanoVG");
    }
}

OpenGLRenderer::~OpenGLRenderer()
{
    nvgDeleteGL3(_nvgContext);
}

void OpenGLRenderer::uploadFrameBuffer(FrameBuffer& frameBuffer)
{
    if (frameBuffer.isUploaded())
    {
        return;
    }

    GLuint frameBufferId = 0;

    GL_ASSERT(glGenFramebuffers(1, &frameBufferId));
    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId));

    // Attach all render buffers
    for (FrameBuffer::RenderBufferAttachment& attachment : frameBuffer.renderBufferAttachments())
    {
        RenderBuffer& renderBuffer = attachment.renderBuffer();

        // Upload the render buffer if needed
        if (!renderBuffer.isUploaded())
        {
            uploadRenderBuffer(renderBuffer);
        }

        // Attach the render buffer to the frame buffer
        auto renderBufferData = renderBuffer.dataAs<RenderBufferData>();
        GL_ASSERT(glFramebufferRenderbuffer(GL_FRAMEBUFFER, _frameBufferSlotLookUp[attachment.slot()], GL_RENDERBUFFER, renderBufferData->renderBufferId));
    }

    GLenum mrt[16];

    // Attach all textures
    int textureIndex = 0;
    for (FrameBuffer::TextureAttachment& attachment : frameBuffer.textureAttachments())
    {
        Texture& texture = attachment.texture();

        // Upload the texture if needed
        if (!texture.isUploaded())
        {
            uploadTexture(texture);
        }

        auto targetData = texture.dataAs<TextureData>();
        GL_ASSERT(glFramebufferTexture2D(GL_FRAMEBUFFER, _frameBufferSlotLookUp[attachment.slot()], GL_TEXTURE_2D, targetData->textureId, 0));

        mrt[textureIndex++] = _frameBufferSlotLookUp[attachment.slot()];

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw InvalidOperation("Invalid frame buffer");
        }
    }

    GL_ASSERT(glDrawBuffers(textureIndex, mrt));

    frameBuffer.setAsUploaded(*this, new FrameBufferData(*this, frameBuffer, frameBufferId));

    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void OpenGLRenderer::destroyFrameBuffer(FrameBuffer& frameBuffer)
{
    if (!frameBuffer.isUploaded())
    {
        return;
    }

    auto data = frameBuffer.dataAs<FrameBufferData>();

    GL_ASSERT(glDeleteFramebuffers(1, &data->frameBufferId));

    frameBuffer.setAsDestroyed();
}

void OpenGLRenderer::uploadRenderBuffer(RenderBuffer& renderBuffer)
{
    if (renderBuffer.isUploaded())
    {
        return;
    }

    GLuint renderBufferId = 0;

    GL_ASSERT(glGenRenderbuffers(1, &renderBufferId));
    GL_ASSERT(glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId));
    GL_ASSERT(glRenderbufferStorage(GL_RENDERBUFFER, _renderBufferFormatLookUp[renderBuffer.format()], renderBuffer.width(), renderBuffer.height()));

    renderBuffer.setAsUploaded(*this, new RenderBufferData(*this, renderBuffer, renderBufferId));
    statistics().memoryUsage += renderBuffer.width() * renderBuffer.height() * renderBuffer.bytesPerPixel();

    GL_ASSERT(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void OpenGLRenderer::destroyRenderBuffer(RenderBuffer& renderBuffer)
{
    if (!renderBuffer.isUploaded())
    {
        return;
    }

    auto data = renderBuffer.dataAs<RenderBufferData>();

    GL_ASSERT(glDeleteRenderbuffers(1, &data->renderBufferId));

    statistics().memoryUsage += renderBuffer.width() * renderBuffer.height() * renderBuffer.bytesPerPixel();
    renderBuffer.setAsDestroyed();
}

void OpenGLRenderer::uploadShader(Shader& shader)
{
    if (shader.isUploaded())
    {
        return;
    }

    // Create the shader.
    GLuint programId = 0;
    programId = GL_ASSERT(glCreateProgram());

    // Attach each shader to the program
    std::vector<GLuint> shaderIds;
    for (const ShaderModule& module : shader.modules())
    {
        GLuint shaderId;

        // Create the shader
        GL_ASSERT(shaderId = glCreateShader(_shaderSourceTypeLookUp[static_cast<int>(module.type())]));

        // Compile shader
        const GLchar* source = module.source().c_str();
        GL_ASSERT(glShaderSource(shaderId, 1, &source, nullptr));
        GL_ASSERT(glCompileShader(shaderId));

        // Report errors
        int logLength = 0;
        GL_ASSERT(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength));
        if (logLength > 1)
        {
            int charsWritten = 0;
            std::string infoLog(logLength, ' ');
            GL_ASSERT(glGetShaderInfoLog(shaderId, logLength, &charsWritten, &infoLog[0]));

            if (infoLog.size() > 0)
            {
                throw InvalidOperation(format("Invalid shader: Failed to compile GLSL shader source file '%s': %s", module.name().c_str(), infoLog.c_str()));
            }
        }

        GL_ASSERT(glAttachShader(programId, shaderId));
        shaderIds.push_back(shaderId);
    }

    // Link program
    GL_ASSERT(glLinkProgram(programId));

    // Report errors
    int logLength = 0;
    GL_ASSERT(glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength));
    if (logLength > 1)
    {
        int charsWritten = 0;
        std::string infoLog(logLength, ' ');
        GL_ASSERT(glGetProgramInfoLog(programId, logLength, &charsWritten, &infoLog[0]));

        if (infoLog.size() > 0)
        {
            throw InvalidOperation(format("Invalid shader: Failed to link GLSL shaders for shader '%s': %s", shader.name().c_str(), infoLog.c_str()));
        }
    }

    GL_ASSERT(glUseProgram(programId));

    // Get the locations of each uniform
    for (Uniform& uniform : shader.uniforms())
    {
        GL_ASSERT(int location = glGetUniformLocation(programId, uniform.name().c_str()));

        if (location != -1)
        {
            uniform.setLocation(location);
        }
        else
        {
            HECT_WARNING(format("Uniform '%s' is not referenced in shader '%s'", uniform.name().c_str(), shader.name().c_str()));
        }
    }

    GL_ASSERT(glUseProgram(0));

    shader.setAsUploaded(*this, new ShaderData(*this, shader, programId, shaderIds));

    HECT_TRACE(format("Uploaded shader '%s'", shader.name().c_str()));
}

void OpenGLRenderer::destroyShader(Shader& shader)
{
    if (!shader.isUploaded())
    {
        return;
    }

    auto data = shader.dataAs<ShaderData>();

    // Destroy all shaders in the program
    for (GLuint shaderId : data->shaderIds)
    {
        GL_ASSERT(glDetachShader(data->programId, shaderId));
        GL_ASSERT(glDeleteShader(shaderId));
    }

    // Delete the program
    GL_ASSERT(glDeleteProgram(data->programId));

    shader.setAsDestroyed();

    HECT_TRACE(format("Destroyed shader '%s'", shader.name().c_str()));
}

void OpenGLRenderer::uploadTexture(Texture& texture)
{
    if (texture.isUploaded())
    {
        return;
    }

    GLenum type = _textureTypeLookUp[texture.type()];

    GLuint textureId = 0;
    GL_ASSERT(glGenTextures(1, &textureId));
    GL_ASSERT(glBindTexture(type, textureId));
    GL_ASSERT(
        glTexParameteri(
            type,
            GL_TEXTURE_MIN_FILTER,
            texture.isMipmapped() ?
            _textureMipmapFilterLookUp[(int)texture.minFilter()] :
            _textureFilterLookUp[(int)texture.minFilter()]
        )
    );

    GL_ASSERT(
        glTexParameteri(
            type,
            GL_TEXTURE_MAG_FILTER,
            _textureFilterLookUp[(int)texture.magFilter()]
        )
    );

    if (texture.isWrapped())
    {
        GL_ASSERT(glTexParameterf(type, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GL_ASSERT(glTexParameterf(type, GL_TEXTURE_WRAP_T, GL_REPEAT));
    }
    else
    {
        GL_ASSERT(glTexParameterf(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_ASSERT(glTexParameterf(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    }

    GLenum target = GL_TEXTURE_2D;

    if (texture.type() == TextureType_CubeMap)
    {
        target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    }

    for (AssetHandle<Image>& imageHandle : texture.sourceImages())
    {
        Image& image = *imageHandle;

        GL_ASSERT(
            glTexImage2D(
                target,
                0,
                _internalImageFormatLookUp[(int)image.colorSpace()][(int)image.pixelFormat()][(int)image.pixelType()],
                image.width(),
                image.height(),
                0,
                _pixelFormatLookUp[(int)image.pixelFormat()],
                _pixelTypeLookUp[(int)image.pixelType()],
                image.hasPixelData() ? &image.pixelData()[0] : 0
            )
        );

        if (texture.type() == TextureType_CubeMap)
        {
            ++target;
        }
    }
    texture.clearSourceImages();

    if (texture.isMipmapped())
    {
        GL_ASSERT(glGenerateMipmap(type));
    }

    GL_ASSERT(glBindTexture(type, 0));

    texture.setAsUploaded(*this, new TextureData(*this, texture, textureId));
    statistics().memoryUsage += texture.width() * texture.height() * texture.bytesPerPixel();

    HECT_TRACE(format("Uploaded texture '%s'", texture.name().c_str()));
}

void OpenGLRenderer::destroyTexture(Texture& texture)
{
    if (!texture.isUploaded())
    {
        return;
    }

    auto data = texture.dataAs<TextureData>();
    GL_ASSERT(glDeleteTextures(1, &data->textureId));

    statistics().memoryUsage -= texture.width() * texture.height() * texture.bytesPerPixel();
    texture.setAsDestroyed();

    HECT_TRACE(format("Destroyed texture '%s'", texture.name().c_str()));
}

Image OpenGLRenderer::downloadTextureImage(const Texture& texture)
{
    if (!texture.isUploaded())
    {
        throw InvalidOperation("The texture is not uploaded");
    }

    auto data = texture.dataAs<TextureData>();

    GL_ASSERT(glBindTexture(GL_TEXTURE_2D, data->textureId));

    Image image;
    image.setWidth(texture.width());
    image.setHeight(texture.height());
    image.setPixelType(texture.pixelType());
    image.setPixelFormat(texture.pixelFormat());

    ByteVector pixelData(image.bytesPerPixel() * image.width() * image.height(), 0);

    GL_ASSERT(
        glGetTexImage(
            GL_TEXTURE_2D,
            0,
            _pixelFormatLookUp[(int)texture.pixelFormat()],
            _pixelTypeLookUp[(int)texture.pixelType()],
            &pixelData[0]
        )
    );

    image.setPixelData(std::move(pixelData));

    GL_ASSERT(glBindTexture(GL_TEXTURE_2D, 0));

    return image;
}

void OpenGLRenderer::uploadMesh(Mesh& mesh)
{
    if (mesh.isUploaded())
    {
        return;
    }

    GLuint vertexArrayId = 0;
    GLuint vertexBufferId = 0;
    GLuint indexBufferId = 0;

    // Generate and bind the vertex array
    GL_ASSERT(glGenVertexArrays(1, &vertexArrayId));
    GL_ASSERT(glBindVertexArray(vertexArrayId));

    // Generate vertex and index buffers
    GL_ASSERT(glGenBuffers(1, &vertexBufferId));
    GL_ASSERT(glGenBuffers(1, &indexBufferId));

    // Upload the vertex data
    GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId));
    GL_ASSERT(
        glBufferData(
            GL_ARRAY_BUFFER,
            mesh.vertexLayout().vertexSize() * mesh.vertexCount(),
            &mesh.vertexData()[0],
            GL_STATIC_DRAW
        )
    );

    // Describe the vertex layout
    GLuint attributeIndex = 0;
    const VertexLayout& vertexLayout = mesh.vertexLayout();
    for (const VertexAttribute& attribute : vertexLayout.attributes())
    {
        GL_ASSERT(glEnableVertexAttribArray(attributeIndex));

        if (attribute.type() == VertexAttributeType_Float32)
        {
            GL_ASSERT(
                glVertexAttribPointer(
                    attributeIndex,
                    attribute.cardinality(),
                    _vertexAttributeTypeLookUp[(int)attribute.type()],
                    GL_FALSE,
                    vertexLayout.vertexSize(),
                    (GLfloat*)attribute.offset()
                )
            );
        }
        else
        {
            GL_ASSERT(
                glVertexAttribIPointer(
                    attributeIndex,
                    attribute.cardinality(),
                    _vertexAttributeTypeLookUp[(int)attribute.type()],
                    vertexLayout.vertexSize(),
                    (GLfloat*)attribute.offset()
                )
            );
        }

        ++attributeIndex;
    }

    // Upload the index data
    GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId));
    GL_ASSERT(
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            mesh.indexSize() * mesh.indexCount(),
            &mesh.indexData()[0],
            GL_STATIC_DRAW
        )
    );

    GL_ASSERT(glBindVertexArray(0));

    mesh.setAsUploaded(*this, new MeshData(*this, mesh, vertexArrayId, vertexBufferId, indexBufferId));

    HECT_TRACE(format("Uploaded mesh '%s'", mesh.name().c_str()));
}

void OpenGLRenderer::destroyMesh(Mesh& mesh)
{
    if (!mesh.isUploaded())
    {
        return;
    }

    auto data = mesh.dataAs<MeshData>();

    // Delete vertex and index buffers
    GL_ASSERT(glDeleteBuffers(2, &data->vertexBufferId));

    // Delete the vertex array object
    GL_ASSERT(glDeleteVertexArrays(1, &data->vertexArrayId));

    mesh.setAsDestroyed();

    HECT_TRACE(format("Destroyed mesh '%s'", mesh.name().c_str()));
}

void OpenGLRenderer::uploadFont(Font& font)
{
    if (font.isUploaded())
    {
        return;
    }

    // Create the NanoVG font and set the id
    int fontId = nvgCreateFontMem(_nvgContext, font.name().c_str(), const_cast<unsigned char*>(&font.rawData()[0]), static_cast<int>(font.rawData().size()), 0);
    font.setAsUploaded(*this, new FontData(*this, font, fontId));

    HECT_TRACE(format("Uploaded font '%s'", font.name().c_str()));
}

void OpenGLRenderer::destroyFont(Font& font)
{
    if (!font.isUploaded())
    {
        return;
    }

    font.setAsDestroyed();

    HECT_TRACE(format("Destroyed font '%s'", font.name().c_str()));
}

void OpenGLRenderer::setTarget(RenderTarget& renderTarget)
{
    renderTarget.bind(*this);
}

void OpenGLRenderer::setTarget(Window& window)
{
    GL_ASSERT(glViewport(0, 0, window.width(), window.height()));
    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void OpenGLRenderer::setTarget(FrameBuffer& frameBuffer)
{
    if (!frameBuffer.isUploaded())
    {
        uploadFrameBuffer(frameBuffer);
    }

    auto data = frameBuffer.dataAs<FrameBufferData>();

    GL_ASSERT(glViewport(0, 0, frameBuffer.width(), frameBuffer.height()));
    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, data->frameBufferId));
}

void OpenGLRenderer::frameBegin(RenderTarget& target)
{
    beginFrame();
    setTarget(target);
}

void OpenGLRenderer::frameEnd()
{
    endFrame();
}

void OpenGLRenderer::frameSetCullMode(CullMode cullMode)
{
    switch (cullMode)
    {
    case CullMode_CounterClockwise:
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        break;
    case CullMode_Clockwise:
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        break;
    case CullMode_None:
        glDisable(GL_CULL_FACE);
        break;
    }
}

void OpenGLRenderer::frameSetShader(Shader& shader)
{
    // Upload the shader if needed
    if (!shader.isUploaded())
    {
        uploadShader(shader);
    }

    // Bind the shader
    auto data = shader.dataAs<ShaderData>();
    GL_ASSERT(glUseProgram(data->programId));

    // If the blend mode is non-trivial
    const BlendMode& blendMode = shader.blendMode();
    if (blendMode != BlendMode())
    {
        GL_ASSERT(glEnable(GL_BLEND));

        // Set the blend function
        GLenum function = _blendFunctionLookUp[static_cast<GLenum>(blendMode.function())];
        GL_ASSERT(glBlendEquation(function));

        // Set the blend factors
        GLenum sourceFactor = _blendFactorLookUp[static_cast<GLenum>(blendMode.sourceFactor())];
        GLenum destinationFactor = _blendFactorLookUp[static_cast<GLenum>(blendMode.destinationFactor())];
        GL_ASSERT(glBlendFunc(sourceFactor, destinationFactor));
    }
    else
    {
        GL_ASSERT(glDisable(GL_BLEND));
    }

    // Enable or disable depth testing
    if (shader.isDepthTested())
    {
        GL_ASSERT(glEnable(GL_DEPTH_TEST));
    }
    else
    {
        GL_ASSERT(glDisable(GL_DEPTH_TEST));
    }

    // Set the values for each unbound uniform
    for (const Uniform& uniform : shader.uniforms())
    {
        if (uniform.binding() == UniformBinding_None)
        {
            frameSetUniform(uniform, uniform.value());
        }
    }
}

void OpenGLRenderer::frameSetUniform(const Uniform& uniform, const UniformValue& value)
{
    switch (value.type())
    {
    case UniformType_Int:
        frameSetUniform(uniform, value.asInt());
        break;
    case UniformType_Float:
        frameSetUniform(uniform, value.asReal());
        break;
    case UniformType_Vector2:
        frameSetUniform(uniform, value.asVector2());
        break;
    case UniformType_Vector3:
        frameSetUniform(uniform, value.asVector3());
        break;
    case UniformType_Vector4:
        frameSetUniform(uniform, value.asVector4());
        break;
    case UniformType_Matrix4:
        frameSetUniform(uniform, value.asMatrix4());
        break;
    case UniformType_Color:
        frameSetUniform(uniform, value.asColor());
        break;
    case UniformType_Texture:
    {
        AssetHandle<Texture> texture = value.asTexture();
        if (texture)
        {
            frameSetUniform(uniform, *texture);
        }
    }
    break;
    }
}


void OpenGLRenderer::frameSetUniform(const Uniform& uniform, int value)
{
    if (uniform.type() != UniformType_Int)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    int location = uniform.location();
    if (location >= 0)
    {
        GL_ASSERT(glUniform1i(location, value));
    }
}

void OpenGLRenderer::frameSetUniform(const Uniform& uniform, Real value)
{
    if (uniform.type() != UniformType_Float)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    int location = uniform.location();
    if (location >= 0)
    {
        GLfloat temp = GLfloat(value);
        GL_ASSERT(glUniform1f(location, temp));
    }
}

void OpenGLRenderer::frameSetUniform(const Uniform& uniform, const Vector2& value)
{
    if (uniform.type() != UniformType_Vector2)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    int location = uniform.location();
    if (location >= 0)
    {
        GLfloat temp[2] = { GLfloat(value[0]), GLfloat(value[1]) };
        GL_ASSERT(glUniform2fv(location, 1, temp));
    }
}

void OpenGLRenderer::frameSetUniform(const Uniform& uniform, const Vector3& value)
{
    if (uniform.type() != UniformType_Vector3)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    int location = uniform.location();
    if (location >= 0)
    {
        GLfloat temp[3] = { GLfloat(value[0]), GLfloat(value[1]), GLfloat(value[2]) };
        GL_ASSERT(glUniform3fv(location, 1, temp));
    }
}

void OpenGLRenderer::frameSetUniform(const Uniform& uniform, const Vector4& value)
{
    if (uniform.type() != UniformType_Vector4)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    int location = uniform.location();
    if (location >= 0)
    {
        GLfloat temp[4] = { GLfloat(value[0]), GLfloat(value[1]), GLfloat(value[2]), GLfloat(value[3]) };
        GL_ASSERT(glUniform4fv(location, 1, temp));
    }
}

void OpenGLRenderer::frameSetUniform(const Uniform& uniform, const Matrix4& value)
{
    if (uniform.type() != UniformType_Matrix4)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    int location = uniform.location();
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

void OpenGLRenderer::frameSetUniform(const Uniform& uniform, const Color& value)
{
    if (uniform.type() != UniformType_Color)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    int location = uniform.location();
    if (location >= 0)
    {
        GLfloat temp[4] = { GLfloat(value[0]), GLfloat(value[1]), GLfloat(value[2]), GLfloat(value[3]) };
        GL_ASSERT(glUniform4fv(location, 1, temp));
    }
}

void OpenGLRenderer::frameSetUniform(const Uniform& uniform, Texture& value)
{
    if (uniform.type() != UniformType_Texture)
    {
        throw InvalidOperation("Invalid value for uniform");
    }

    int location = uniform.location();
    if (location >= 0)
    {
        GL_ASSERT(glUniform1i(location, static_cast<GLint>(uniform.textureIndex())));
        bindTexture(value, uniform.textureIndex());
    }
}

void OpenGLRenderer::frameRenderMesh(Mesh& mesh)
{
    if (!mesh.isUploaded())
    {
        uploadMesh(mesh);
    }

    auto data = mesh.dataAs<MeshData>();
    GL_ASSERT(glBindVertexArray(data->vertexArrayId));

    GL_ASSERT(
        glDrawElements(
            _primitiveTypeLookUp[static_cast<int>(mesh.primitiveType())],
            static_cast<GLsizei>(mesh.indexCount()),
            _indexTypeLookUp[static_cast<int>(mesh.indexType())],
            0
        )
    );
}

void OpenGLRenderer::frameClear()
{
    GL_ASSERT(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void OpenGLRenderer::vectorFrameBegin(RenderTarget& target)
{
    beginFrame();
    setTarget(target);

    assert(_nvgContext);
    nvgBeginFrame(_nvgContext, target.width(), target.height(), 1);
}

void OpenGLRenderer::vectorFrameEnd()
{
    nvgEndFrame(_nvgContext);
    endFrame();
}

void OpenGLRenderer::vectorFrameSetFillColor(const Color& color)
{
    nvgFillColor(_nvgContext, convertColor(color));
}

void OpenGLRenderer::vectorFrameRectangle(const Rectangle& bounds)
{
    Vector2T<float> position = bounds.minimum();
    Vector2T<float> size = bounds.size();
    nvgRect(_nvgContext, position.x, position.y, size.x, size.y);
}

void OpenGLRenderer::vectorFrameRenderPath()
{
    nvgFill(_nvgContext);
}

void OpenGLRenderer::vectorFrameSetFont(Font& font, Real size)
{
    if (!font.isUploaded())
    {
        uploadFont(font);
    }

    // Set the font
    nvgFontSize(_nvgContext, static_cast<float>(size));
    nvgFontFaceId(_nvgContext, font.dataAs<FontData>()->fontId);
}

void OpenGLRenderer::vectorFrameRenderText(const std::string& text, const Rectangle& bounds, HorizontalAlign horizontalAlign, VerticalAlign verticalAlign)
{
    Vector2T<float> position = bounds.minimum();
    Vector2T<float> size = bounds.size();

    // Compute the text bounds
    float measuredBounds[4];
    nvgTextBounds(_nvgContext, 0, 0, text.c_str(), nullptr, measuredBounds);
    Vector2T<float> textPosition(measuredBounds[0], measuredBounds[1]);
    Vector2T<float> textSize(measuredBounds[2] - measuredBounds[0], measuredBounds[3] - measuredBounds[1]);
    Vector2T<float> actual(position - textPosition);

    // Align horizontally
    switch (horizontalAlign)
    {
    case HorizontalAlign_Left:
        break;
    case HorizontalAlign_Center:
        actual.x += size.x / 2 - textSize.x / 2;
        break;
    case HorizontalAlign_Right:
        actual.x += size.x - textSize.x;
        break;
    }

    // Align vertically
    switch (verticalAlign)
    {
    case VerticalAlign_Bottom:
        actual.y += size.y - textSize.y;
        break;
    case VerticalAlign_Center:
        actual.y += size.y / 2 - textSize.y / 2;
        break;
    case VerticalAlign_Top:
        break;
    }

    // Render the text
    nvgScissor(_nvgContext, position.x, position.y, size.x, size.y);
    nvgText(_nvgContext, actual.x, actual.y, text.c_str(), nullptr);
    nvgResetScissor(_nvgContext);
}

void OpenGLRenderer::beginFrame()
{
    GL_ASSERT(glUseProgram(0));
    GL_ASSERT(glBindVertexArray(0));

    for (unsigned i = 0; i < capabilities().maxTextureUnits; ++i)
    {
        GL_ASSERT(glActiveTexture(GL_TEXTURE0 + i));
        GL_ASSERT(glBindTexture(GL_TEXTURE_2D, 0));
    }
}

void OpenGLRenderer::endFrame()
{
}

void OpenGLRenderer::bindTexture(Texture& texture, size_t index)
{
    if (index >= capabilities().maxTextureUnits)
    {
        throw InvalidOperation("Cannot bind a texture unit beyond hardware capabilities");
    }

    if (!texture.isUploaded())
    {
        uploadTexture(texture);
    }

    auto data = texture.dataAs<TextureData>();

    GL_ASSERT(glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(index)));
    GL_ASSERT(glBindTexture(_textureTypeLookUp[texture.type()], data->textureId));
}

NVGcolor OpenGLRenderer::convertColor(const Color& color) const
{
    NVGcolor result;
    result.r = static_cast<float>(color[0]);
    result.g = static_cast<float>(color[1]);
    result.b = static_cast<float>(color[2]);
    result.a = static_cast<float>(color[3]);
    return result;
}

#endif