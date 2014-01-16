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
#include "Hect.h"

#include <GL/glew.h>

using namespace hect;

#ifdef HECT_DEBUG
#define GL_ASSERT(expression) expression _checkGLError();
#else
#define GL_ASSERT(expression) expression
#endif

class ShaderModuleData :
    public RendererObjectData
{
public:
    GLuint id;
};

class ShaderData :
    public RendererObjectData
{
public:
    GLuint id;
};

class TextureData :
    public RendererObjectData
{
public:
    GLuint id;
};

class FrameBufferData :
    public RendererObjectData
{
public:
    GLuint frameBufferId;
    GLuint depthBufferId;
};

class MeshData :
    public RendererObjectData
{
public:
    GLuint vertexArrayId;
    GLuint vertexBufferId;
    GLuint indexBufferId;
};

void _checkGLError()
{
    GLenum errorCode;
    if ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        throw Error(format("OpenGL error: %s", gluErrorString(errorCode)));
    }
}

GLenum _vertexAttributeTypeLookUp[8] =
{
    GL_BYTE, // Byte
    GL_UNSIGNED_BYTE, // UnsignedByte
    GL_SHORT, // Short
    GL_UNSIGNED_SHORT, // UnsignedShort
    GL_INT, // Int
    GL_UNSIGNED_INT, // UnsignedInt
    GL_HALF_FLOAT, // Half
    GL_FLOAT // Float
};

GLenum _indexTypeLookUp[3] =
{
    GL_UNSIGNED_BYTE, // UnsignedByte
    GL_UNSIGNED_SHORT, // UnsignedShort
    GL_UNSIGNED_INT // UnsignedInt
};

GLenum _primitiveTypeLookUp[5] =
{
    GL_TRIANGLES, // Triangles
    GL_TRIANGLE_STRIP, // TriangleStrip
    GL_LINES, // Lines
    GL_LINE_STRIP, // LineStrip
    GL_POINTS // Points
};

GLenum _blendFactorLookUp[10] =
{
    GL_ZERO, // Zero
    GL_ONE, // One
    GL_SRC_COLOR, // SourceColor
    GL_ONE_MINUS_SRC_COLOR, // OneMinusSourceColor
    GL_DST_COLOR, // DestColor
    GL_ONE_MINUS_DST_COLOR, // OneMinusDestColor
    GL_SRC_ALPHA, // SourceAlpha
    GL_ONE_MINUS_SRC_ALPHA, // OneMinusSourceAlpha
    GL_DST_ALPHA, // DestAlpha
    GL_ONE_MINUS_DST_ALPHA // OneMinusDestAlpha
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
    GL_HALF_FLOAT, // Half
    GL_FLOAT, // Float
    GL_UNSIGNED_BYTE // Byte
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
            GL_RGB16F, // Half
            GL_RGB32F, // Float
            GL_SRGB8 // Byte
        },
        // Rgba
        {
            GL_RGBA16F, // Half
            GL_RGBA32F, // Float
            GL_SRGB8_ALPHA8 // Byte
        }
    },

    // Linear
    {
        // Rgb
        {
            GL_RGB16F, // Half
            GL_RGB32F, // Float
            GL_RGB8 // Byte
        },
        // Rgba
        {
            GL_RGBA16F, // Half
            GL_RGBA32F, // Float
            GL_RGBA8 // Byte
        }
    }
};

GLenum _shaderModuleTypeLookUp[3] =
{
    GL_VERTEX_SHADER, // Vertex
    GL_FRAGMENT_SHADER, // Pixel
    GL_GEOMETRY_SHADER // Geometry
};

Renderer::Renderer(Window& window) :
    _boundTarget(nullptr),
    _boundShader(nullptr),
    _boundMesh(nullptr)
{
    // This is a parameter only to ensure the window is created before the
    // renderer; I'm not sure if the renderer has any use for it
    window;

    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        const char* errorString = reinterpret_cast<const char*>(glewGetErrorString(error));
        throw Error(format("Failed to initialize OpenGL: %s", errorString));
    }

    LOG_INFO(format("OpenGL version %s", glGetString(GL_VERSION)));
    LOG_INFO(format("GLSL version %s", glGetString(GL_SHADING_LANGUAGE_VERSION)));
    LOG_INFO(format("%s", glGetString(GL_VENDOR)));
    LOG_INFO(format("%s", glGetString(GL_RENDERER)));

    GLint maxTextureUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    _capabilities.maxTextureUnits = (unsigned)maxTextureUnits;

    LOG_INFO(format("Max texture units: %d", _capabilities.maxTextureUnits));

    _boundTextures = std::vector<Texture*>(maxTextureUnits, nullptr);

    glGetError(); // Clear errors

    GL_ASSERT( glClearColor(0, 0, 0, 1); )

    // Set up our point rendering profile
    GL_ASSERT( glEnable(GL_PROGRAM_POINT_SIZE); )
    GL_ASSERT( glEnable(GL_POINT_SPRITE); )
    GL_ASSERT( glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT); )

    clear();
}

void Renderer::beginFrame()
{
}

void Renderer::endFrame()
{
    // Clear the bound target
    if (_boundTarget)
    {
        _boundTarget = nullptr;
    }

    // Clear the bound shader and unbind
    if (_boundShader)
    {
        GL_ASSERT( glUseProgram(0); )
        _boundShader = nullptr;
    }

    // Clear the bound mesh and unbind
    if (_boundMesh)
    {
        GL_ASSERT( glBindVertexArray(0); )
        _boundMesh = nullptr;
    }

    // Clear all bound textures and unbind
    for (unsigned i = 0; i < _boundTextures.size(); ++i)
    {
        if (_boundTextures[i])
        {
            GL_ASSERT( glActiveTexture(GL_TEXTURE0 + i); )
            GL_ASSERT( glBindTexture(GL_TEXTURE_2D, 0); )
            _boundTextures[i] = nullptr;
        }
    }
}

void Renderer::bindMode(const RenderMode& mode)
{
    if (mode.isStateEnabled(RenderState::DepthTest))
    {
        GL_ASSERT( glEnable(GL_DEPTH_TEST); )
    }
    else
    {
        GL_ASSERT( glDisable(GL_DEPTH_TEST); )
    }

    if (mode.isStateEnabled(RenderState::CullFace))
    {
        GL_ASSERT( glEnable(GL_CULL_FACE); )
    }
    else
    {
        GL_ASSERT( glDisable(GL_CULL_FACE); )
    }

    if (mode.isStateEnabled(RenderState::Blend))
    {
        GL_ASSERT( glEnable(GL_BLEND); )

        GLuint sourceFactor = _blendFactorLookUp[(int)mode.sourceBlendFactor()];
        GLuint destFactor = _blendFactorLookUp[(int)mode.destBlendFactor()];

        GL_ASSERT( glBlendFunc(sourceFactor, destFactor); )
    }
    else
    {
        GL_ASSERT( glDisable(GL_BLEND); )
    }
}

void Renderer::bindTarget(RenderTarget& renderTarget)
{
    renderTarget.bind(this);
}

void Renderer::bindWindow(Window& window)
{
    // Avoid binding an already bound target
    if (&window == _boundTarget)
    {
        return;
    }
    _boundTarget = &window;

    GL_ASSERT( glViewport(0, 0, window.width(), window.height()); )
    GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, 0); )
}

void Renderer::bindFrameBuffer(FrameBuffer& frameBuffer)
{
    // Avoid binding an already bound target
    if (&frameBuffer == _boundTarget)
    {
        return;
    }
    _boundTarget = &frameBuffer;

    if (!frameBuffer.isUploaded())
    {
        uploadFrameBuffer(frameBuffer);
    }

    auto data = (FrameBufferData*)frameBuffer._data;

    GL_ASSERT( glViewport(0, 0, frameBuffer.width(), frameBuffer.height()); )
    GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, data->frameBufferId); )
}

void Renderer::uploadFrameBuffer(FrameBuffer& frameBuffer)
{
    if (frameBuffer.isUploaded())
    {
        return;
    }

    auto data = new FrameBufferData();
    GL_ASSERT( glGenFramebuffers(1, &data->frameBufferId); )
    GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, data->frameBufferId); )

    if (frameBuffer.hasDepthComponent())
    {
        GL_ASSERT( glGenRenderbuffers(1, &data->depthBufferId); )
        GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, data->depthBufferId); )
        GL_ASSERT( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, frameBuffer.width(), frameBuffer.height()); )
        GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, data->depthBufferId); )
    }

    GLenum mrt[8];

    int targetIndex = 0;
    for (Texture& target : frameBuffer.targets())
    {
        uploadTexture(target);

        auto targetData = (TextureData*)target._data;
        GL_ASSERT( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + targetIndex, GL_TEXTURE_2D, targetData->id, 0); )

        mrt[targetIndex++] = GL_COLOR_ATTACHMENT0 + targetIndex;

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw Error("Invalid frame buffer");
        }
    }

    GL_ASSERT( glDrawBuffers(targetIndex, mrt); )

    frameBuffer._uploaded = true;
    frameBuffer._data = data;
    frameBuffer._renderer = this;

    GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, 0); )
}

void Renderer::destroyFrameBuffer(FrameBuffer& frameBuffer)
{
    if (!frameBuffer.isUploaded())
    {
        return;
    }

    auto data = (FrameBufferData*)frameBuffer._data;

    GL_ASSERT( glDeleteFramebuffers(1, &data->frameBufferId); )
    GL_ASSERT( glDeleteRenderbuffers(1, &data->depthBufferId); )

    delete data;
    frameBuffer._data = nullptr;
}

void Renderer::bindShader(Shader& shader)
{
    // Avoid binding an already bound shader
    if (&shader == _boundShader)
    {
        return;
    }
    _boundShader = &shader;

    // Upload the shader if needed
    if (!shader.isUploaded())
    {
        uploadShader(shader);
    }

    auto data = (ShaderData*)shader._data;
    GL_ASSERT( glUseProgram(data->id); )

    // Pass the default values for each uniform
    for (const Uniform& uniform : shader.uniforms())
    {
        if (uniform.hasDefaultValue())
        {
            setUniform(uniform, uniform.defaultValue());
        }
    }
}

void Renderer::uploadShader(Shader& shader)
{
    if (shader.isUploaded())
    {
        return;
    }

    LOG_DEBUG(format("Uploading shader '%s'...", shader.name().c_str()));

    // Create the shader.
    auto data = new ShaderData();
    data->id = GL_ASSERT( glCreateProgram(); )

    // Attach each shader to the program
    for (const AssetHandle<ShaderModule>& module : shader.modules())
    {
        if (!module.get().isUploaded())
        {
            uploadShaderModule(module.get());
        }

        auto moduleData = (ShaderModuleData*)module.get()._data;
        GL_ASSERT( glAttachShader(data->id, moduleData->id); )
    }

    // Link program
    GL_ASSERT( glLinkProgram(data->id); )

    // Report errors
    int logLength = 0;
    GL_ASSERT( glGetProgramiv(data->id, GL_INFO_LOG_LENGTH, &logLength); )
    if (logLength > 1)
    {
        int charsWritten = 0;
        std::string infoLog(logLength, ' ');
        GL_ASSERT( glGetProgramInfoLog(data->id, logLength, &charsWritten, &infoLog[0]); )

        if (infoLog.size() > 0)
        {
            throw Error(format("Failed to link shader '%s': %s", shader.name().c_str(), infoLog.c_str()));
        }
    }

    GL_ASSERT( glUseProgram(data->id); )

    // Get the locations of each uniform
    for (Uniform& uniform : shader.uniforms())
    {
        GL_ASSERT( int location = glGetUniformLocation(data->id, uniform.name().c_str()); )

        if (location != -1)
        {
            uniform.setLocation(location);
        }
        else
        {
            LOG_WARNING(format("Uniform '%s' is not referenced in shader '%s'", uniform.name().c_str(), shader.name().c_str()));
        }
    }

    GL_ASSERT( glUseProgram(0); )

    shader._uploaded = true;
    shader._data = data;
    shader._renderer = this;
}

void Renderer::destroyShader(Shader& shader)
{
    if (!shader.isUploaded())
    {
        return;
    }

    LOG_DEBUG(format("Destroying shader '%s'...", shader.name().c_str()));

    auto data = (ShaderData*)shader._data;

    // Detach all attached shaders.
    for (const AssetHandle<ShaderModule>& module : shader.modules())
    {
        auto moduleData = (ShaderModuleData*)module.get()._data;
        GL_ASSERT( glDetachShader(data->id, moduleData->id); )
    }

    GL_ASSERT( glDeleteProgram(data->id); )

    delete data;
    shader._data = nullptr;
}

void Renderer::setUniform(const Uniform& uniform, const UniformValue& value)
{
    if (!_boundShader)
    {
        throw Error("Attempt to set uniform with no shader bound");
    }

    int location = uniform.location();
    if (location < 0)
    {
        return;
    }

    switch (value.type())
    {
    case UniformType::Int:
    case UniformType::Texture:
        GL_ASSERT( glUniform1i(location, *(GLint*)value.data()); )
        break;
    case UniformType::Float:
        GL_ASSERT( glUniform1f(location, *(GLfloat*)value.data()); )
        break;
    case UniformType::Vector2:
        GL_ASSERT( glUniform2fv(location, 1, (GLfloat*)value.data()); )
        break;
    case UniformType::Vector3:
        GL_ASSERT( glUniform3fv(location, 1, (GLfloat*)value.data()); )
        break;
    case UniformType::Vector4:
        GL_ASSERT( glUniform4fv(location, 1, (GLfloat*)value.data()); )
        break;
    case UniformType::Matrix4:
        GL_ASSERT( glUniformMatrix4fv(location, 1, false, (GLfloat*)value.data()); )
        break;
    }
}

void Renderer::uploadShaderModule(ShaderModule& module)
{
    if (module.isUploaded())
    {
        return;
    }

    LOG_DEBUG(format("Uploading shader module '%s'...", module.name().c_str()));

    // Create the shader
    auto data = new ShaderModuleData();
    GL_ASSERT( data->id = glCreateShader(_shaderModuleTypeLookUp[(int)module.type()]); )

    // Compile shader
    const GLchar* source = module.source().c_str();
    GL_ASSERT( glShaderSource(data->id, 1, &source, nullptr); )
    GL_ASSERT( glCompileShader(data->id); )

    // Report errors
    int logLength = 0;
    GL_ASSERT( glGetShaderiv(data->id, GL_INFO_LOG_LENGTH, &logLength); )
    if (logLength > 1)
    {
        int charsWritten = 0;
        std::string infoLog(logLength, ' ');
        GL_ASSERT( glGetShaderInfoLog(data->id, logLength, &charsWritten, &infoLog[0]); )

        if (infoLog.size() > 0)
        {
            throw Error(format("Failed to compile shader module '%s': %s", module.name().c_str(), infoLog.c_str()));
        }
    }

    module._uploaded = true;
    module._data = data;
    module._renderer = this;
}

void Renderer::destroyShaderModule(ShaderModule& module)
{
    if (!module.isUploaded())
    {
        return;
    }

    LOG_DEBUG(format("Destroying shader module '%s'...", module.name().c_str()));

    auto data = (ShaderModuleData*)module._data;
    GL_ASSERT( glDeleteShader(data->id); )

    delete data;
    module._data = nullptr;
}

void Renderer::bindTexture(Texture& texture, unsigned index)
{
    if (index >= _capabilities.maxTextureUnits)
    {
        throw Error("Attempt to bind a texture unit beyond hardware capabilities");
    }

    if (_boundTextures[index] == &texture)
    {
        return;
    }

    if (!texture.isUploaded())
    {
        uploadTexture(texture);
    }

    auto data = (TextureData*)texture._data;

    GL_ASSERT( glActiveTexture(GL_TEXTURE0 + index); )
    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, data->id); )
}

void Renderer::uploadTexture(Texture& texture)
{
    if (texture.isUploaded())
    {
        return;
    }

    LOG_DEBUG(format("Uploading texture '%s'...", texture.name().c_str()));

    auto data = new TextureData();
    GL_ASSERT( glGenTextures(1, &data->id); )
    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, data->id); )
    GL_ASSERT(
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            texture.isMipmapped() ?
            _textureMipmapFilterLookUp[(int)texture.minFilter()] :
            _textureFilterLookUp[(int)texture.minFilter()]
        );
    )
    GL_ASSERT(
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            _textureFilterLookUp[(int)texture.magFilter()]
        );
    )

    if (texture.isWrapped())
    {
        GL_ASSERT( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); )
        GL_ASSERT( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); )
    }
    else
    {
        GL_ASSERT( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); )
        GL_ASSERT( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); )
    }

    Image* image = texture._image.get();
    assert(image); // The texture should always have an image at this point

    GL_ASSERT(
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            _internalImageFormatLookUp[(int)image->colorSpace()][(int)image->pixelFormat()][(int)image->pixelType()],
            image->width(),
            image->height(),
            0,
            _pixelFormatLookUp[(int)image->pixelFormat()],
            _pixelTypeLookUp[(int)image->pixelType()],
            &image->pixelData()[0]
        );
    )

    // The texture no longer needs the source image
    texture._image = Image::Ref();

    if (texture.isMipmapped())
    {
        GL_ASSERT( glGenerateMipmap(GL_TEXTURE_2D); )
    }

    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, 0); )

    texture._uploaded = true;
    texture._data = data;
    texture._renderer = this;
}

void Renderer::destroyTexture(Texture& texture)
{
    if (!texture.isUploaded())
    {
        return;
    }

    LOG_DEBUG(format("Destroying texture '%s'...", texture.name().c_str()));

    auto data = (TextureData*)texture._data;
    GL_ASSERT( glDeleteTextures(1, &data->id); )

    delete data;
    texture._data = nullptr;
}

Image Renderer::downloadTextureImage(const Texture& texture)
{
    if (!texture.isUploaded())
    {
        throw Error("The texture is not uploaded");
    }

    auto data = (TextureData*)texture._data;

    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, data->id); )

    Image image(texture.width(), texture.height(), texture.pixelType(), texture.pixelFormat());

    GL_ASSERT(
        glGetTexImage(
            GL_TEXTURE_2D,
            0,
            _pixelFormatLookUp[(int)texture.pixelFormat()],
            _pixelTypeLookUp[(int)texture.pixelType()],
            (GLvoid*)&image.pixelData()[0]
        );
    )

    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, 0); )

    return image;
}

void Renderer::bindMesh(Mesh& mesh)
{
    if (&mesh == _boundMesh)
    {
        return;
    }
    _boundMesh = &mesh;

    if (!mesh.isUploaded())
    {
        uploadMesh(mesh);
    }

    auto data = (MeshData*)(mesh._data);
    GL_ASSERT( glBindVertexArray(data->vertexArrayId); )
}

void Renderer::uploadMesh(Mesh& mesh)
{
    if (mesh.isUploaded())
    {
        return;
    }

    LOG_DEBUG(format("Uploading mesh '%s'...", mesh.name().c_str()));

    auto data = new MeshData();

    // Generate and bind the vertex array
    GL_ASSERT( glGenVertexArrays(1, &data->vertexArrayId); )
    GL_ASSERT( glBindVertexArray(data->vertexArrayId); )

    // Generate vertex and index buffers
    GL_ASSERT( glGenBuffers(2, &data->vertexBufferId); )

    // Upload the vertex data
    GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, data->vertexBufferId); )
    GL_ASSERT(
        glBufferData(
            GL_ARRAY_BUFFER,
            mesh.vertexLayout().vertexSize() * mesh.vertexCount(),
            &mesh.vertexData()[0],
            GL_STATIC_DRAW
        );
    )

    // Describe the vertex layout
    GLuint attributeIndex = 0;
    const VertexLayout& vertexLayout = mesh.vertexLayout();
    for (const VertexAttribute& attribute : vertexLayout.attributes())
    {
        GL_ASSERT( glEnableVertexAttribArray(attributeIndex); )

        if (attribute.type() == VertexAttributeType::Half || attribute.type() == VertexAttributeType::Float)
        {
            GL_ASSERT(
                glVertexAttribPointer(
                    attributeIndex,
                    attribute.cardinality(),
                    _vertexAttributeTypeLookUp[(int)attribute.type()],
                    GL_FALSE,
                    vertexLayout.vertexSize(),
                    (GLfloat*)attribute.offset()
                );
            )
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
                );
            )
        }

        ++attributeIndex;
    }

    // Upload the index data
    GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->indexBufferId); )
    GL_ASSERT(
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            mesh.indexSize() * mesh.indexCount(),
            &mesh.indexData()[0],
            GL_STATIC_DRAW
        );
    )

    GL_ASSERT( glBindVertexArray(0); )

    mesh._uploaded = true;
    mesh._data = data;
    mesh._renderer = this;
}

void Renderer::destroyMesh(Mesh& mesh)
{
    if (!mesh.isUploaded())
    {
        return;
    }

    LOG_DEBUG(format("Destroying mesh '%s'...", mesh.name().c_str()));

    auto data = (MeshData*)mesh._data;

    // Delete vertex and index buffers
    GL_ASSERT( glDeleteBuffers(2, &data->vertexBufferId); )

    // Delete the vertex array object
    GL_ASSERT( glDeleteVertexArrays(1, &data->vertexArrayId); )

    delete data;
    mesh._data = nullptr;
}

void Renderer::draw()
{
    if (!_boundMesh)
    {
        throw Error("Attempt to draw without a mesh bound");
    }

    GL_ASSERT(
        glDrawElements(
            _primitiveTypeLookUp[(int)_boundMesh->primitiveType()],
            (GLsizei)_boundMesh->indexCount(),
            _indexTypeLookUp[(int)_boundMesh->indexType()],
            0
        );
    )
}

void Renderer::clear()
{
    GL_ASSERT( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); )
}

const Renderer::Capabilities& Renderer::capabilities() const
{
    return _capabilities;
}