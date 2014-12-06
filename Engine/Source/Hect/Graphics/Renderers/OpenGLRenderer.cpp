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
#include "OpenGLRenderer.h"

#ifdef HECT_RENDERER_OPENGL

#include <GL/glew.h>
#include <algorithm>

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"
#include "Hect/Graphics/FrameBuffer.h"
#include "Hect/Graphics/Material.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/RenderTarget.h"
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
class MaterialData :
    public Renderer::Data<Material>
{
public:
    MaterialData(Renderer& renderer, Material& object) :
        Renderer::Data<Material>(renderer, object),
        programId(GLuint(-1))
    {
    }

    MaterialData(Renderer& renderer, Material& object, GLuint programId, const std::vector<GLuint>& shaderIds) :
        Renderer::Data<Material>(renderer, object),
        programId(programId),
        shaderIds(shaderIds)
    {
    }

    ~MaterialData()
    {
        // Destroy the shader if it is uploaded
        if (object && object->isUploaded())
        {
            renderer->destroyMaterial(*object);
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

// Throws an error if an OpenGL error occurred
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

void OpenGLRenderer::initialize(Window& window)
{
    (void)window;

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
    capabilities().maxTextureUnits = (unsigned)maxTextureUnits;

    HECT_INFO(format("Max texture units: %d", capabilities().maxTextureUnits));

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

void OpenGLRenderer::beginFrame()
{
}

void OpenGLRenderer::endFrame()
{
    // Clear the bound target
    setBoundTarget(nullptr);

    // Clear the bound shader and unbind
    if (boundMaterial())
    {
        GL_ASSERT(glUseProgram(0));
        setBoundMaterial(nullptr);
    }

    // Clear the bound mesh and unbind
    if (boundMesh())
    {
        GL_ASSERT(glBindVertexArray(0));
        setBoundMesh(nullptr);
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

void OpenGLRenderer::bindState(const RenderState& state)
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

void OpenGLRenderer::bindTarget(RenderTarget& renderTarget)
{
    renderTarget.bind(*this);
}

void OpenGLRenderer::bindWindow(Window& window)
{
    // Avoid binding an already bound target
    if (&window == boundTarget())
    {
        return;
    }
    setBoundTarget(&window);

    GL_ASSERT(glViewport(0, 0, window.width(), window.height()));
    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void OpenGLRenderer::bindFrameBuffer(FrameBuffer& frameBuffer)
{
    // Avoid binding an already bound target
    if (&frameBuffer == boundTarget())
    {
        return;
    }
    setBoundTarget(&frameBuffer);

    if (!frameBuffer.isUploaded())
    {
        uploadFrameBuffer(frameBuffer);
    }

    auto data = frameBuffer.dataAs<FrameBufferData>();

    GL_ASSERT(glViewport(0, 0, frameBuffer.width(), frameBuffer.height()));
    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, data->frameBufferId));
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
            throw Error("Invalid frame buffer");
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

void OpenGLRenderer::bindMaterial(Material& material)
{
    // Avoid binding an already bound material
    if (&material == boundMaterial() && material.isUploaded())
    {
        return;
    }

    // Upload the material if needed
    if (!material.isUploaded())
    {
        uploadMaterial(material);
    }

    // Bind the base material if needed
    if (material.base())
    {
        bindMaterial(*material.base());
        setBoundMaterial(&material);
    }
    else
    {
        auto data = material.dataAs<MaterialData>();
        GL_ASSERT(glUseProgram(data->programId));

        setBoundMaterial(&material);

        // Pass the default values for each parameter
        for (const MaterialParameter& parameter : material.parameters())
        {
            if (parameter.hasDefaultValue())
            {
                bindMaterialParameter(parameter, parameter.defaultValue());
            }
        }

        bindState(material.renderState());
    }

    // Pass the values for each argument
    for (const MaterialArgument& argument : material.arguments())
    {
        const MaterialParameter& parameter = material.parameterWithName(argument.name());
        bindMaterialParameter(parameter, argument.value());
    }
}

void OpenGLRenderer::uploadMaterial(Material& material)
{
    if (material.isUploaded())
    {
        return;
    }

    if (!material.base())
    {
        // Create the shader.
        GLuint programId = 0;
        programId = GL_ASSERT(glCreateProgram());

        // Attach each shader to the program
        std::vector<GLuint> shaderIds;
        for (ShaderModule& module : material.shaderModules())
        {
            GLuint shaderId;

            // Create the shader
            GL_ASSERT(shaderId = glCreateShader(_shaderModuleTypeLookUp[(int)module.type()]));

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
                    throw Error(format("Failed to compile GLSL shader module '%s': %s", module.path().asString().c_str(), infoLog.c_str()));
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
                throw Error(format("Failed to link GLSL shaders for material '%s': %s", material.name().c_str(), infoLog.c_str()));
            }
        }

        GL_ASSERT(glUseProgram(programId));

        // Get the parameter locations of each parameter
        for (MaterialParameter& parameter : material.parameters())
        {
            GL_ASSERT(int location = glGetUniformLocation(programId, parameter.name().c_str()));

            if (location != -1)
            {
                parameter.setLocation(location);
            }
            else
            {
                HECT_WARNING(format("Parameter '%s' is not referenced in GLSL of material '%s'", parameter.name().c_str(), material.name().c_str()));
            }
        }

        GL_ASSERT(glUseProgram(0));

        material.setAsUploaded(*this, new MaterialData(*this, material, programId, shaderIds));
    }
    else
    {
        material.setAsUploaded(*this, new MaterialData(*this, material));
    }

    HECT_TRACE(format("Uploaded material '%s'", material.name().c_str()));
}

void OpenGLRenderer::destroyMaterial(Material& material)
{
    if (!material.isUploaded())
    {
        return;
    }

    auto data = material.dataAs<MaterialData>();
    if (!material.base())
    {
        // Destroy all shaders in the program
        for (GLuint shaderId : data->shaderIds)
        {
            GL_ASSERT(glDetachShader(data->programId, shaderId));
            GL_ASSERT(glDeleteShader(shaderId));
        }

        // Delete the program
        GL_ASSERT(glDeleteProgram(data->programId));
    }

    material.setAsDestroyed();

    HECT_TRACE(format("Destroyed material '%s'", material.name().c_str()));
}

void OpenGLRenderer::bindMaterialParameter(const MaterialParameter& parameter, const MaterialValue& value)
{
    switch (value.type())
    {
    case MaterialValueType_Int:
        bindMaterialParameter(parameter, value.asInt());
        break;
    case MaterialValueType_Float:
        bindMaterialParameter(parameter, value.asReal());
        break;
    case MaterialValueType_Vector2:
        bindMaterialParameter(parameter, value.asVector2());
        break;
    case MaterialValueType_Vector3:
        bindMaterialParameter(parameter, value.asVector3());
        break;
    case MaterialValueType_Vector4:
        bindMaterialParameter(parameter, value.asVector4());
        break;
    case MaterialValueType_Matrix4:
        bindMaterialParameter(parameter, value.asMatrix4());
        break;
    case MaterialValueType_Texture:
    {
        AssetHandle<Texture> texture = value.asTexture();
        if (texture)
        {
            bindMaterialParameter(parameter, *texture);
        }
    }
    break;
    }
}

void OpenGLRenderer::bindMaterialParameter(const MaterialParameter& parameter, int value)
{
    if (!boundMaterial())
    {
        throw Error("No material bound");
    }

    if (parameter.type() != MaterialValueType_Int)
    {
        throw Error("Invalid value for material parameter");
    }

    int location = parameter.location();
    if (location >= 0)
    {
        GL_ASSERT(glUniform1i(location, value));
    }
}

void OpenGLRenderer::bindMaterialParameter(const MaterialParameter& parameter, Real value)
{
    if (!boundMaterial())
    {
        throw Error("No material bound");
    }

    if (parameter.type() != MaterialValueType_Float)
    {
        throw Error("Invalid value for material parameter");
    }

    int location = parameter.location();
    if (location >= 0)
    {
        GLfloat temp = GLfloat(value);
        GL_ASSERT(glUniform1f(location, temp));
    }
}

void OpenGLRenderer::bindMaterialParameter(const MaterialParameter& parameter, const Vector2& value)
{
    if (!boundMaterial())
    {
        throw Error("No material bound");
    }

    if (parameter.type() != MaterialValueType_Vector2)
    {
        throw Error("Invalid value for material parameter");
    }

    int location = parameter.location();
    if (location >= 0)
    {
        GLfloat temp[2] = { GLfloat(value[0]), GLfloat(value[1]) };
        GL_ASSERT(glUniform2fv(location, 1, temp));
    }
}

void OpenGLRenderer::bindMaterialParameter(const MaterialParameter& parameter, const Vector3& value)
{
    if (!boundMaterial())
    {
        throw Error("No material bound");
    }

    if (parameter.type() != MaterialValueType_Vector3)
    {
        throw Error("Invalid value for material parameter");
    }

    int location = parameter.location();
    if (location >= 0)
    {
        GLfloat temp[3] = { GLfloat(value[0]), GLfloat(value[1]), GLfloat(value[2]) };
        GL_ASSERT(glUniform3fv(location, 1, temp));
    }
}

void OpenGLRenderer::bindMaterialParameter(const MaterialParameter& parameter, const Vector4& value)
{
    if (!boundMaterial())
    {
        throw Error("No material bound");
    }

    if (parameter.type() != MaterialValueType_Vector4)
    {
        throw Error("Invalid value for material parameter");
    }

    int location = parameter.location();
    if (location >= 0)
    {
        GLfloat temp[4] = { GLfloat(value[0]), GLfloat(value[1]), GLfloat(value[2]), GLfloat(value[3]) };
        GL_ASSERT(glUniform4fv(location, 1, temp));
    }
}

void OpenGLRenderer::bindMaterialParameter(const MaterialParameter& parameter, const Matrix4& value)
{
    if (!boundMaterial())
    {
        throw Error("No material bound");
    }

    if (parameter.type() != MaterialValueType_Matrix4)
    {
        throw Error("Invalid value for material parameter");
    }

    int location = parameter.location();
    if (location >= 0)
    {
        GLfloat temp[16] =
        {
            GLfloat(value[ 0]), GLfloat(value[ 1]), GLfloat(value[ 2]), GLfloat(value[ 3]),
            GLfloat(value[ 4]), GLfloat(value[ 5]), GLfloat(value[ 6]), GLfloat(value[ 7]),
            GLfloat(value[ 8]), GLfloat(value[ 9]), GLfloat(value[10]), GLfloat(value[11]),
            GLfloat(value[12]), GLfloat(value[13]), GLfloat(value[14]), GLfloat(value[15])
        };

        GL_ASSERT(glUniformMatrix4fv(location, 1, false, temp));
    }
}

void OpenGLRenderer::bindMaterialParameter(const MaterialParameter& parameter, Texture& texture)
{
    if (!boundMaterial())
    {
        throw Error("No material bound");
    }

    if (parameter.type() != MaterialValueType_Texture)
    {
        throw Error("Invalid value for material parameter");
    }

    int location = parameter.location();
    if (location >= 0)
    {
        GL_ASSERT(glUniform1i(location, parameter.textureIndex()));
        bindTexture(texture, parameter.textureIndex());
    }
}

void OpenGLRenderer::bindTexture(Texture& texture, unsigned index)
{
    if (index >= capabilities().maxTextureUnits)
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
        throw Error("The texture is not uploaded");
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

void OpenGLRenderer::bindMesh(Mesh& mesh)
{
    if (&mesh == boundMesh())
    {
        return;
    }
    setBoundMesh(&mesh);

    if (!mesh.isUploaded())
    {
        uploadMesh(mesh);
    }

    auto data = mesh.dataAs<MeshData>();
    GL_ASSERT(glBindVertexArray(data->vertexArrayId));
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

void OpenGLRenderer::draw()
{
    if (!boundMesh())
    {
        throw Error("Cannot draw without a mesh bound");
    }

    GL_ASSERT(
        glDrawElements(
            _primitiveTypeLookUp[(int)boundMesh()->primitiveType()],
            (GLsizei)boundMesh()->indexCount(),
            _indexTypeLookUp[(int)boundMesh()->indexType()],
            0
        )
    );
}

void OpenGLRenderer::clear()
{
    GL_ASSERT(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

#endif