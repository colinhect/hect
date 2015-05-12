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

#ifdef HECT_RENDERER_OPENGL

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
#include "Hect/Graphics/CubicTexture.h"
#include "Hect/Runtime/Window.h"

using namespace hect;

#ifdef HECT_DEBUG_BUILD
#define GL_ASSERT(expression) expression; checkGLError()
#else
#define GL_ASSERT(expression) expression
#endif

namespace
{

static Mesh _viewportMesh;

Mesh createViewportMesh()
{
    Mesh viewportMesh("Viewport");

    VertexLayout vertexLayout;
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3));
    vertexLayout.addAttribute(VertexAttribute(VertexAttributeSemantic::TextureCoords0, VertexAttributeType::Float32, 3));

    viewportMesh.setVertexLayout(vertexLayout);

    MeshWriter meshWriter(viewportMesh);
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(-1, -1, 0));
    meshWriter.writeAttributeData(VertexAttributeSemantic::TextureCoords0, Vector2(0, 0));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(1, -1, 0));
    meshWriter.writeAttributeData(VertexAttributeSemantic::TextureCoords0, Vector2(1, 0));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(1, 1, 0));
    meshWriter.writeAttributeData(VertexAttributeSemantic::TextureCoords0, Vector2(1, 1));
    meshWriter.addVertex();
    meshWriter.writeAttributeData(VertexAttributeSemantic::Position, Vector3(-1, 1, 0));
    meshWriter.writeAttributeData(VertexAttributeSemantic::TextureCoords0, Vector2(0, 1));
    meshWriter.addIndex(0);
    meshWriter.addIndex(1);
    meshWriter.addIndex(2);
    meshWriter.addIndex(2);
    meshWriter.addIndex(3);
    meshWriter.addIndex(0);

    return viewportMesh;
}

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
class Texture2Data :
	public Renderer::Data<Texture2>
{
public:
	Texture2Data(Renderer& renderer, Texture2& object, GLuint textureId) :
		Renderer::Data<Texture2>(renderer, object),
		textureId(textureId)
	{
	}

	~Texture2Data()
	{
		// Destroy the texture if it is uploaded
		if (object && object->isUploaded())
		{
			renderer->destroyTexture(*object);
		}
	}

	GLuint textureId;
};

// OpenGL-specific data for a cubic texture
class CubicTextureData :
	public Renderer::Data<CubicTexture>
{
public:
	CubicTextureData(Renderer& renderer, CubicTexture& object, GLuint textureId) :
		Renderer::Data<CubicTexture>(renderer, object),
		textureId(textureId)
	{
	}

	~CubicTextureData()
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
void checkGLError()
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

GLenum _pixelFormatLookUp[5] =
{
	GLenum(-1), // 0
    GL_R, // 1
    GL_RG, // 2
    GL_RGB, // 3
    GL_RGBA // 4
};

GLenum _internalImageFormatLookUp[2][5][3] =
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

GLenum _shaderSourceTypeLookUp[3] =
{
    GL_VERTEX_SHADER, // Vertex
    GL_FRAGMENT_SHADER, // Fragment
    GL_GEOMETRY_SHADER // Geometry
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
};

}

void Renderer::Frame::setCullMode(CullMode cullMode)
{
    switch (cullMode)
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

void Renderer::Frame::setShader(Shader& shader)
{
    // Upload the shader if needed
    if (!shader.isUploaded())
    {
        _renderer.uploadShader(shader);
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
        GL_ASSERT(glDepthMask(GL_TRUE));
    }
    else
    {
        GL_ASSERT(glDisable(GL_DEPTH_TEST));
        GL_ASSERT(glDepthMask(GL_FALSE));
    }

    // Set the values for each unbound uniform
    for (const Uniform& uniform : shader.uniforms())
    {
        if (uniform.binding() == UniformBinding::None)
        {
            setUniform(uniform, uniform.value());
        }
    }
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
	case UniformType::CubicTexture:
	{
		CubicTexture::Handle texture = value.asCubicTexture();
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

    int location = uniform.location();
    if (location >= 0)
    {
        GL_ASSERT(glUniform1i(location, value));
    }
}

void Renderer::Frame::setUniform(const Uniform& uniform, double value)
{
    if (uniform.type() != UniformType::Float)
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

void Renderer::Frame::setUniform(const Uniform& uniform, const Vector2& value)
{
    if (uniform.type() != UniformType::Vector2)
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

void Renderer::Frame::setUniform(const Uniform& uniform, const Vector3& value)
{
    if (uniform.type() != UniformType::Vector3)
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

void Renderer::Frame::setUniform(const Uniform& uniform, const Vector4& value)
{
    if (uniform.type() != UniformType::Vector4)
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

void Renderer::Frame::setUniform(const Uniform& uniform, const Matrix4& value)
{
    if (uniform.type() != UniformType::Matrix4)
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

void Renderer::Frame::setUniform(const Uniform& uniform, const Color& value)
{
    if (uniform.type() != UniformType::Color)
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

void Renderer::Frame::setUniform(const Uniform& uniform, Texture2& value)
{
	if (uniform.type() != UniformType::Texture2)
	{
		throw InvalidOperation("Invalid value for uniform");
	}

	int location = uniform.location();
	if (location >= 0)
	{
		GL_ASSERT(glUniform1i(location, static_cast<GLint>(uniform.textureIndex())));
		TextureIndex index = uniform.textureIndex();

		if (index >= _renderer.capabilities().maxTextureUnits)
		{
			throw InvalidOperation("Cannot bind a texture unit beyond hardware capabilities");
		}

		if (!value.isUploaded())
		{
			_renderer.uploadTexture(value);
		}

		auto data = value.dataAs<Texture2Data>();

		GL_ASSERT(glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(index)));
		GL_ASSERT(glBindTexture(GL_TEXTURE_2D, data->textureId));
	}
}

void Renderer::Frame::setUniform(const Uniform& uniform, CubicTexture& value)
{
	if (uniform.type() != UniformType::CubicTexture)
	{
		throw InvalidOperation("Invalid value for uniform");
	}

	int location = uniform.location();
	if (location >= 0)
	{
		GL_ASSERT(glUniform1i(location, static_cast<GLint>(uniform.textureIndex())));
		TextureIndex index = uniform.textureIndex();

		if (index >= _renderer.capabilities().maxTextureUnits)
		{
			throw InvalidOperation("Cannot bind a texture unit beyond hardware capabilities");
		}

		if (!value.isUploaded())
		{
			_renderer.uploadTexture(value);
		}

		auto data = value.dataAs<CubicTextureData>();

		GL_ASSERT(glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(index)));
		GL_ASSERT(glBindTexture(GL_TEXTURE_CUBE_MAP, data->textureId));
	}
}

void Renderer::Frame::renderMesh(Mesh& mesh)
{
    if (!mesh.isUploaded())
    {
        _renderer.uploadMesh(mesh);
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

void Renderer::Frame::renderViewport()
{
    renderMesh(_viewportMesh);
}

void Renderer::Frame::clear(bool depth)
{
    GL_ASSERT(glClear(depth ? (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) : GL_COLOR_BUFFER_BIT));
}

Renderer::Frame::Frame(Renderer& renderer, RenderTarget& target) :
    _renderer(renderer)
{
    GL_ASSERT(glUseProgram(0));
    GL_ASSERT(glBindVertexArray(0));

    for (unsigned i = 0; i < _renderer.capabilities().maxTextureUnits; ++i)
    {
        GL_ASSERT(glActiveTexture(GL_TEXTURE0 + i));
        GL_ASSERT(glBindTexture(GL_TEXTURE_2D, 0));
    }

    GL_ASSERT(glDisable(GL_BLEND));
    GL_ASSERT(glEnable(GL_DEPTH_TEST));
    GL_ASSERT(glDepthMask(GL_TRUE));

    _renderer.setTarget(target);
}

Renderer::~Renderer()
{
    destroyMesh(_viewportMesh);
}

void Renderer::uploadFrameBuffer(FrameBuffer& frameBuffer)
{
    if (frameBuffer.isUploaded())
    {
        return;
    }

    GLuint frameBufferId = 0;

    GL_ASSERT(glGenFramebuffers(1, &frameBufferId));
    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId));

    GLenum mrt[16];

    // Attach all textures
    int textureIndex = 0;
    for (FrameBuffer::Attachment& attachment : frameBuffer.attachments())
    {
        Texture2& texture = attachment.texture();

        // Upload the texture if needed
        if (!texture.isUploaded())
        {
			if (attachment.slot() == FrameBufferSlot::Depth)
			{
				GLenum type = GL_TEXTURE_2D;

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

				glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

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

				const PixelFormat& pixelFormat = texture.pixelFormat();

				GLint internalFormat = -1;
				if (pixelFormat.type() == PixelType::Float16)
				{
					internalFormat = GL_DEPTH_COMPONENT16;
				}
				else if(pixelFormat.type() == PixelType::Float32)
				{
					internalFormat = GL_DEPTH_COMPONENT32;
				}

				GL_ASSERT(
					glTexImage2D(
						target,
						0,
						internalFormat,
						texture.width(),
						texture.height(),
						0,
						GL_DEPTH_COMPONENT,
						_pixelTypeLookUp[(int)pixelFormat.type()],
						0
						)
					);

				texture.clearSourceImage();

				if (texture.isMipmapped())
				{
					GL_ASSERT(glGenerateMipmap(type));
				}

				GL_ASSERT(glBindTexture(type, 0));

				texture.setAsUploaded(*this, new Texture2Data(*this, texture, textureId));
				statistics().memoryUsage += texture.width() * texture.height() * texture.pixelFormat().size();

				HECT_TRACE(format("Uploaded texture '%s'", texture.name().c_str()));
			}
			else
			{
				uploadTexture(texture);
			}
        }

        auto targetData = texture.dataAs<Texture2Data>();
        GL_ASSERT(glFramebufferTexture2D(GL_FRAMEBUFFER, _frameBufferSlotLookUp[(int)attachment.slot()], GL_TEXTURE_2D, targetData->textureId, 0));

		if (attachment.slot() != FrameBufferSlot::Depth)
		{
			mrt[textureIndex++] = _frameBufferSlotLookUp[(int)attachment.slot()];
		}

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw InvalidOperation("Invalid frame buffer");
        }
    }

    GL_ASSERT(glDrawBuffers(textureIndex, mrt));

    frameBuffer.setAsUploaded(*this, new FrameBufferData(*this, frameBuffer, frameBufferId));

    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Renderer::destroyFrameBuffer(FrameBuffer& frameBuffer)
{
    if (!frameBuffer.isUploaded())
    {
        return;
    }

    auto data = frameBuffer.dataAs<FrameBufferData>();

    GL_ASSERT(glDeleteFramebuffers(1, &data->frameBufferId));

    frameBuffer.setAsDestroyed();
}

void Renderer::uploadShader(Shader& shader)
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

void Renderer::destroyShader(Shader& shader)
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

void Renderer::uploadTexture(Texture2& texture)
{
    if (texture.isUploaded())
    {
        return;
    }

    GLenum type = GL_TEXTURE_2D;

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
	
    Image& image = *texture.sourceImage();
    const PixelFormat& pixelFormat = image.pixelFormat();

    GL_ASSERT(
        glTexImage2D(
			GL_TEXTURE_2D,
            0,
            _internalImageFormatLookUp[(int)image.colorSpace()][(int)pixelFormat.cardinality()][(int)pixelFormat.type()],
            image.width(),
            image.height(),
            0,
            _pixelFormatLookUp[(int)pixelFormat.cardinality()],
            _pixelTypeLookUp[(int)pixelFormat.type()],
            image.hasPixelData() ? &image.pixelData()[0] : 0
        )
    );

    texture.clearSourceImage();

    if (texture.isMipmapped())
    {
        GL_ASSERT(glGenerateMipmap(type));
    }

    GL_ASSERT(glBindTexture(type, 0));

    texture.setAsUploaded(*this, new Texture2Data(*this, texture, textureId));
    statistics().memoryUsage += texture.width() * texture.height() * texture.pixelFormat().size();

    HECT_TRACE(format("Uploaded texture '%s'", texture.name().c_str()));
}

void Renderer::uploadTexture(CubicTexture& texture)
{
	if (texture.isUploaded())
	{
		return;
	}

	GLenum type = GL_TEXTURE_CUBE_MAP;

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

	GL_ASSERT(glTexParameterf(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_ASSERT(glTexParameterf(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;

	for (Image::Handle& imageHandle : texture.sourceImages())
	{
		Image& image = *imageHandle;
		const PixelFormat& pixelFormat = image.pixelFormat();

		GL_ASSERT(
			glTexImage2D(
				target,
				0,
				_internalImageFormatLookUp[(int)image.colorSpace()][(int)pixelFormat.cardinality()][(int)pixelFormat.type()],
				image.width(),
				image.height(),
				0,
				_pixelFormatLookUp[(int)pixelFormat.cardinality()],
				_pixelTypeLookUp[(int)pixelFormat.type()],
				image.hasPixelData() ? &image.pixelData()[0] : 0
				)
			);
		
		++target;
	}
	texture.clearSourceImages();

	if (texture.isMipmapped())
	{
		GL_ASSERT(glGenerateMipmap(type));
	}

	GL_ASSERT(glBindTexture(type, 0));

	texture.setAsUploaded(*this, new CubicTextureData(*this, texture, textureId));
	statistics().memoryUsage += texture.width() * texture.height() * texture.pixelFormat().size();

	HECT_TRACE(format("Uploaded texture '%s'", texture.name().c_str()));
}

void Renderer::destroyTexture(Texture2& texture)
{
	if (!texture.isUploaded())
	{
		return;
	}

	auto data = texture.dataAs<Texture2Data>();
	GL_ASSERT(glDeleteTextures(1, &data->textureId));

	statistics().memoryUsage -= texture.width() * texture.height() * texture.pixelFormat().size();
	texture.setAsDestroyed();

	HECT_TRACE(format("Destroyed texture '%s'", texture.name().c_str()));
}

void Renderer::destroyTexture(CubicTexture& texture)
{
	if (!texture.isUploaded())
	{
		return;
	}

	auto data = texture.dataAs<CubicTextureData>();
	GL_ASSERT(glDeleteTextures(1, &data->textureId));

	statistics().memoryUsage -= texture.width() * texture.height() * texture.pixelFormat().size();
	texture.setAsDestroyed();

	HECT_TRACE(format("Destroyed texture '%s'", texture.name().c_str()));
}

Image Renderer::downloadTextureImage(const Texture2& texture)
{
    if (!texture.isUploaded())
    {
        throw InvalidOperation("The texture is not uploaded");
    }

    auto data = texture.dataAs<Texture2Data>();

    GL_ASSERT(glBindTexture(GL_TEXTURE_2D, data->textureId));

    Image image;
    image.setWidth(texture.width());
    image.setHeight(texture.height());
    image.setPixelFormat(texture.pixelFormat());

    ByteVector pixelData(image.pixelFormat().size() * image.width() * image.height(), 0);

    GL_ASSERT(
        glGetTexImage(
            GL_TEXTURE_2D,
            0,
            _pixelFormatLookUp[(int)texture.pixelFormat().cardinality()],
            _pixelTypeLookUp[(int)texture.pixelFormat().type()],
            &pixelData[0]
        )
    );

    image.setPixelData(std::move(pixelData));

    GL_ASSERT(glBindTexture(GL_TEXTURE_2D, 0));

    return image;
}

void Renderer::uploadMesh(Mesh& mesh)
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
            !mesh.vertexData().empty() ? &mesh.vertexData()[0] : nullptr,
            GL_STATIC_DRAW
        )
    );

    // Describe the vertex layout
    GLuint attributeIndex = 0;
    const VertexLayout& vertexLayout = mesh.vertexLayout();
    for (const VertexAttribute& attribute : vertexLayout.attributes())
    {
        GL_ASSERT(glEnableVertexAttribArray(attributeIndex));

		size_t offset = attribute.offset();

        if (attribute.type() == VertexAttributeType::Float32)
        {

            GL_ASSERT(
                glVertexAttribPointer(
                    attributeIndex,
                    attribute.cardinality(),
                    _vertexAttributeTypeLookUp[(int)attribute.type()],
                    GL_FALSE,
                    vertexLayout.vertexSize(),
                    reinterpret_cast<GLvoid*>(offset)
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
					reinterpret_cast<GLvoid*>(offset)
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
            !mesh.indexData().empty() ? &mesh.indexData()[0] : nullptr,
            GL_STATIC_DRAW
        )
    );

    GL_ASSERT(glBindVertexArray(0));

    mesh.setAsUploaded(*this, new MeshData(*this, mesh, vertexArrayId, vertexBufferId, indexBufferId));

    HECT_TRACE(format("Uploaded mesh '%s'", mesh.name().c_str()));
}

void Renderer::destroyMesh(Mesh& mesh)
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

void Renderer::setTarget(RenderTarget& renderTarget)
{
    renderTarget.bind(*this);
}

void Renderer::setTarget(Window& window)
{
    GL_ASSERT(glViewport(0, 0, window.width(), window.height()));
    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Renderer::setTarget(FrameBuffer& frameBuffer)
{
    if (!frameBuffer.isUploaded())
    {
        uploadFrameBuffer(frameBuffer);
    }

    auto data = frameBuffer.dataAs<FrameBufferData>();

    GL_ASSERT(glViewport(0, 0, frameBuffer.width(), frameBuffer.height()));
    GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, data->frameBufferId));
}

Renderer::Renderer()
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

    // Create the mesh used to in Renderer::Frame::renderViewport()
    _viewportMesh = createViewportMesh();
}

#endif