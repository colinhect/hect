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
#include "GraphicsContext.h"

#include <GL/glew.h>

#include <algorithm>

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

#ifdef HECT_DEBUG_BUILD
#define GL_ASSERT(expression) expression; _checkGLError()
#else
#define GL_ASSERT(expression) expression
#endif

class ShaderData :
    public GraphicsContext::Data<Shader>
{
public:
    ShaderData(GraphicsContext& graphicsContext, Shader& object, GLuint programId, const std::vector<GLuint>& shaderIds) :
        GraphicsContext::Data<Shader>(graphicsContext, object),
        programId(programId),
        shaderIds(shaderIds)
    {
    }

    ~ShaderData()
    {
        if (object && object->isUploaded())
        {
            graphicsContext->destroyShader(*object);
        }
    }

    GLuint programId;
    std::vector<GLuint> shaderIds;
};

class TextureData :
    public GraphicsContext::Data<Texture>
{
public:
    TextureData(GraphicsContext& graphicsContext, Texture& object, GLuint textureId) :
        GraphicsContext::Data<Texture>(graphicsContext, object),
        textureId(textureId)
    {
    }

    ~TextureData()
    {
        if (object && object->isUploaded())
        {
            graphicsContext->destroyTexture(*object);
        }
    }

    GLuint textureId;
};

class FrameBufferData :
    public GraphicsContext::Data<FrameBuffer>
{
public:
    FrameBufferData(GraphicsContext& graphicsContext, FrameBuffer& object, GLuint frameBufferId, GLuint depthBufferId) :
        GraphicsContext::Data<FrameBuffer>(graphicsContext, object),
        frameBufferId(frameBufferId),
        depthBufferId(depthBufferId)
    {
    }

    ~FrameBufferData()
    {
        if (object && object->isUploaded())
        {
            graphicsContext->destroyFrameBuffer(*object);
        }
    }

    GLuint frameBufferId;
    GLuint depthBufferId;
};

class MeshData :
    public GraphicsContext::Data<Mesh>
{
public:
    MeshData(GraphicsContext& graphicsContext, Mesh& object, GLuint vertexArrayId, GLuint vertexBufferId, GLuint indexBufferId) :
        GraphicsContext::Data<Mesh>(graphicsContext, object),
        vertexArrayId(vertexArrayId),
        vertexBufferId(vertexBufferId),
        indexBufferId(indexBufferId)
    {
    }

    ~MeshData()
    {
        if (object && object->isUploaded())
        {
            graphicsContext->destroyMesh(*object);
        }
    }

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

GLenum _shaderModuleTypeLookUp[3] =
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

GraphicsContext::GraphicsContext(Window& window) :
    _boundTarget(nullptr),
    _boundShader(nullptr),
    _boundMesh(nullptr)
{
    // This is a parameter only to ensure the window is created before the
    // GPU context; I'm not sure if the GPU context has any use for it
    window;

    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        const char* errorString = reinterpret_cast<const char*>(glewGetErrorString(error));
        throw Error(format("Failed to initialize OpenGL: %s", errorString));
    }

    HECT_INFO(format("OpenGL version %s", glGetString(GL_VERSION)));
    HECT_INFO(format("GLSL version %s", glGetString(GL_SHADING_LANGUAGE_VERSION)));
    HECT_INFO(format("%s", glGetString(GL_VENDOR)));
    HECT_INFO(format("%s", glGetString(GL_RENDERER)));

    GLint maxTextureUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    _capabilities.maxTextureUnits = (unsigned)maxTextureUnits;

    HECT_INFO(format("Max texture units: %d", _capabilities.maxTextureUnits));

    _boundTextures = std::vector<Texture*>(maxTextureUnits, nullptr);

    glGetError(); // Clear errors

    GL_ASSERT(glClearColor(0, 0, 0, 0));

    // Set up the point rendering profile
    GL_ASSERT(glEnable(GL_PROGRAM_POINT_SIZE));
    GL_ASSERT(glEnable(GL_POINT_SPRITE));
    GL_ASSERT(glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT));

    // Set up the cube map rendering profile
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    clear();
}

void GraphicsContext::beginFrame()
{
}

void GraphicsContext::endFrame()
{
    // Clear the bound target
    if (_boundTarget)
    {
        _boundTarget = nullptr;
    }

    // Clear the bound shader and unbind
    if (_boundShader)
    {
        GL_ASSERT(glUseProgram(0));
        _boundShader = nullptr;
    }

    // Clear the bound mesh and unbind
    if (_boundMesh)
    {
        GL_ASSERT(glBindVertexArray(0));
        _boundMesh = nullptr;
    }

    // Clear all bound textures and unbind
    for (unsigned i = 0; i < _boundTextures.size(); ++i)
    {
        if (_boundTextures[i])
        {
            GL_ASSERT(glActiveTexture(GL_TEXTURE0 + i));
            GL_ASSERT(glBindTexture(GL_TEXTURE_2D, 0));
            _boundTextures[i] = nullptr;
        }
    }
}

void GraphicsContext::bindState(const RenderState& state)
{
    if (state.isEnabled(RenderStateFlag_DepthTest))
    {
        GL_ASSERT(glEnable(GL_DEPTH_TEST));
    }
    else
    {
        GL_ASSERT(glDisable(GL_DEPTH_TEST));
    }

    if (state.isEnabled(RenderStateFlag_CullFace))
    {
        GL_ASSERT(glEnable(GL_CULL_FACE));
    }
    else
    {
        GL_ASSERT(glDisable(GL_CULL_FACE));
    }

    if (state.isEnabled(RenderStateFlag_Blend))
    {
        GL_ASSERT(glEnable(GL_BLEND));

        GLuint sourceFactor = _blendFactorLookUp[(int)state.sourceBlendFactor()];
        GLuint destFactor = _blendFactorLookUp[(int)state.destinationBlendFactor()];

        GL_ASSERT(glBlendFunc(sourceFactor, destFactor));
    }
    else
    {
        GL_ASSERT(glDisable(GL_BLEND));
    }
}

void GraphicsContext::bindTarget(RenderTarget& renderTarget)
{
    renderTarget.bind(*this);
}

void GraphicsContext::bindWindow(Window& window)
{
    // Avoid binding an already bound target
    if (&window == _boundTarget)
    {
        return;
    }
    _boundTarget = &window;

    GL_ASSERT(glViewport(0, 0, window.width(), window.height()));
    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void GraphicsContext::bindFrameBuffer(FrameBuffer& frameBuffer)
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

    auto data = frameBuffer.dataAs<FrameBufferData>();

    GL_ASSERT(glViewport(0, 0, frameBuffer.width(), frameBuffer.height()));
    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, data->frameBufferId));
}

void GraphicsContext::uploadFrameBuffer(FrameBuffer& frameBuffer)
{
    if (frameBuffer.isUploaded())
    {
        return;
    }

    GLuint frameBufferId = 0;
    GLuint depthBufferId = 0;

    GL_ASSERT(glGenFramebuffers(1, &frameBufferId));
    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId));

    if (frameBuffer.hasDepthComponent())
    {
        GL_ASSERT(glGenRenderbuffers(1, &depthBufferId));
        GL_ASSERT(glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId));
        GL_ASSERT(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, frameBuffer.width(), frameBuffer.height()));
        GL_ASSERT(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferId));
    }

    GLenum mrt[8];

    int targetIndex = 0;
    for (Texture& target : frameBuffer.targets())
    {
        uploadTexture(target);

        auto targetData = target.dataAs<TextureData>();
        GL_ASSERT(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + targetIndex, GL_TEXTURE_2D, targetData->textureId, 0));

        mrt[targetIndex++] = GL_COLOR_ATTACHMENT0 + targetIndex;

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw Error("Invalid frame buffer");
        }
    }

    GL_ASSERT(glDrawBuffers(targetIndex, mrt));

    frameBuffer.setAsUploaded(*this, new FrameBufferData(*this, frameBuffer, frameBufferId, depthBufferId));

    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void GraphicsContext::destroyFrameBuffer(FrameBuffer& frameBuffer)
{
    if (!frameBuffer.isUploaded())
    {
        return;
    }

    auto data = frameBuffer.dataAs<FrameBufferData>();

    GL_ASSERT(glDeleteFramebuffers(1, &data->frameBufferId));
    GL_ASSERT(glDeleteRenderbuffers(1, &data->depthBufferId));

    frameBuffer.setAsDestroyed();
}

void GraphicsContext::bindShader(Shader& shader)
{
    // Avoid binding an already bound shader
    if (&shader == _boundShader && shader.isUploaded())
    {
        return;
    }
    _boundShader = &shader;

    // Upload the shader if needed
    if (!shader.isUploaded())
    {
        uploadShader(shader);
    }

    auto data = shader.dataAs<ShaderData>();
    GL_ASSERT(glUseProgram(data->programId));

    // Pass the default values for each uniform
    for (const Uniform& uniform : shader.uniforms())
    {
        if (uniform.hasDefaultValue())
        {
            setUniform(uniform, uniform.defaultValue());
        }
    }
}

void GraphicsContext::uploadShader(Shader& shader)
{
    if (shader.isUploaded())
    {
        return;
    }

    HECT_TRACE(format("Uploading shader '%s'...", shader.name().c_str()));

    // Create the shader.
    GLuint programId = 0;
    programId = GL_ASSERT(glCreateProgram());

    // Attach each shader to the program
    std::vector<GLuint> shaderIds;
    for (ShaderModule& module : shader.modules())
    {
        GLuint shaderId;

        // Create the shader
        GL_ASSERT(shaderId = glCreateShader(_shaderModuleTypeLookUp[(int)module.type()]));

        // Compile shader
        std::string preprocessSource;
        switch (module.type())
        {
        case ShaderModuleType_Vertex:
            preprocessSource = "#define VERTEX\n" + module.source();
            break;
        case ShaderModuleType_Fragment:
            preprocessSource = "#define FRAGMENT\n" + module.source();
            break;
        case ShaderModuleType_Geometry:
            preprocessSource = "#define GEOMETRY\n" + module.source();
            break;
        }
        const GLchar* source = preprocessSource.c_str();
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
                throw Error(format("Failed to compile shader module '%s': %s", module.sourcePath().asString().c_str(), infoLog.c_str()));
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
            throw Error(format("Failed to link shader '%s': %s", shader.name().c_str(), infoLog.c_str()));
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
}

void GraphicsContext::destroyShader(Shader& shader)
{
    if (!shader.isUploaded())
    {
        return;
    }

    HECT_TRACE(format("Destroying shader '%s'...", shader.name().c_str()));

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
}

void GraphicsContext::setUniform(const Uniform& uniform, const UniformValue& value)
{
    if (!_boundShader)
    {
        throw Error("Cannot set uniform with no shader bound");
    }

    int location = uniform.location();
    if (location < 0)
    {
        return;
    }

    switch (value.type())
    {
    case UniformType_Int:
    case UniformType_Texture:
        GL_ASSERT(glUniform1i(location, *(GLint*)value.data()));
        break;
    case UniformType_Float:
        GL_ASSERT(glUniform1f(location, *(GLfloat*)value.data()));
        break;
    case UniformType_Vector2:
        GL_ASSERT(glUniform2fv(location, 1, (GLfloat*)value.data()));
        break;
    case UniformType_Vector3:
        GL_ASSERT(glUniform3fv(location, 1, (GLfloat*)value.data()));
        break;
    case UniformType_Vector4:
        GL_ASSERT(glUniform4fv(location, 1, (GLfloat*)value.data()));
        break;
    case UniformType_Matrix4:
        GL_ASSERT(glUniformMatrix4fv(location, 1, false, (GLfloat*)value.data()));
        break;
    }
}

void GraphicsContext::bindTexture(Texture& texture, unsigned index)
{
    if (index >= _capabilities.maxTextureUnits)
    {
        throw Error("Cannot bind a texture unit beyond hardware capabilities");
    }

    if (_boundTextures[index] == &texture)
    {
        return;
    }

    if (!texture.isUploaded())
    {
        uploadTexture(texture);
    }

    auto data = texture.dataAs<TextureData>();

    GL_ASSERT(glActiveTexture(GL_TEXTURE0 + index));
    GL_ASSERT(glBindTexture(_textureTypeLookUp[texture.type()], data->textureId));
}

void GraphicsContext::uploadTexture(Texture& texture)
{
    if (texture.isUploaded())
    {
        return;
    }

    HECT_TRACE(format("Uploading texture '%s'...", texture.name().c_str()));

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

    if (texture.isMipmapped())
    {
        GL_ASSERT(glGenerateMipmap(type));
    }

    GL_ASSERT(glBindTexture(type, 0));

    texture.setAsUploaded(*this, new TextureData(*this, texture, textureId));
}

void GraphicsContext::destroyTexture(Texture& texture)
{
    if (!texture.isUploaded())
    {
        return;
    }

    HECT_TRACE(format("Destroying texture '%s'...", texture.name().c_str()));

    auto data = texture.dataAs<TextureData>();
    GL_ASSERT(glDeleteTextures(1, &data->textureId));

    texture.setAsDestroyed();
}

Image GraphicsContext::downloadTextureImage(const Texture& texture)
{
    if (!texture.isUploaded())
    {
        throw Error("The texture is not uploaded");
    }

    auto data = texture.dataAs<TextureData>();

    GL_ASSERT(glBindTexture(GL_TEXTURE_2D, data->textureId));

    Image image;
    image.setWidth(texture.width());
    image.setHeight(texture.height());
    image.setPixelType(texture.pixelType());
    image.setPixelFormat(texture.pixelFormat());

    Image::PixelData pixelData(image.bytesPerPixel() * image.width() * image.height(), 0);

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

void GraphicsContext::bindMesh(Mesh& mesh)
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

    auto data = mesh.dataAs<MeshData>();
    GL_ASSERT(glBindVertexArray(data->vertexArrayId));
}

void GraphicsContext::uploadMesh(Mesh& mesh)
{
    if (mesh.isUploaded())
    {
        return;
    }

    HECT_TRACE(format("Uploading mesh '%s'...", mesh.name().c_str()));

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

        if (attribute.type() == VertexAttributeType_Float16 || attribute.type() == VertexAttributeType_Float32)
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
}

void GraphicsContext::destroyMesh(Mesh& mesh)
{
    if (!mesh.isUploaded())
    {
        return;
    }

    HECT_TRACE(format("Destroying mesh '%s'...", mesh.name().c_str()));

    auto data = mesh.dataAs<MeshData>();

    // Delete vertex and index buffers
    GL_ASSERT(glDeleteBuffers(2, &data->vertexBufferId));

    // Delete the vertex array object
    GL_ASSERT(glDeleteVertexArrays(1, &data->vertexArrayId));

    mesh.setAsDestroyed();
}

void GraphicsContext::draw()
{
    if (!_boundMesh)
    {
        throw Error("Cannot draw without a mesh bound");
    }

    GL_ASSERT(
        glDrawElements(
            _primitiveTypeLookUp[(int)_boundMesh->primitiveType()],
            (GLsizei)_boundMesh->indexCount(),
            _indexTypeLookUp[(int)_boundMesh->indexType()],
            0
        )
    );
}

void GraphicsContext::clear()
{
    GL_ASSERT(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

const GraphicsContext::Capabilities& GraphicsContext::capabilities() const
{
    return _capabilities;
}