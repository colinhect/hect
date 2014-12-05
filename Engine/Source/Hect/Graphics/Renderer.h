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
#pragma once

#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/RenderState.h"
#include "Hect/Graphics/Image.h"

namespace hect
{

class RenderBuffer;
class FrameBuffer;
class Mesh;
class RenderTarget;
class Shader;
class ShaderParameter;
class ShaderValue;
class Texture;
class Window;

///
/// A higher-level abstraction over a hardware-accelerated graphics API such as
/// OpenGL or DirectX.
class Renderer :
    public Uncopyable
{
public:

    ///
    /// Contains the capabilities of the underlying hardware.
    struct Capabilities
    {
        ///
        /// The maximum number of textures that can be bound.
        unsigned maxTextureUnits { 0 };
    };

    ///
    /// Contains the various statistics of the renderer's state.
    struct Statistics
    {
        ///
        /// The number of bytes allocated on the GPU.
        size_t memoryUsage { 0 };
    };

    ///
    /// Data which provides a handle to the API-specific data.
    template <typename T>
    class Data
    {
    public:

        ///
        /// Constructs renderer data.
        ///
        /// \param renderer The renderer that the data is
        /// uploaded to.
        /// \param object The object that the data represents.
        Data(Renderer& renderer, T& object);

        virtual ~Data() { }

        ///
        /// Invalidates the GPU data.
        void invalidate();

    protected:
        Renderer* renderer;
        T* object;
    };

    ///
    /// A handle to API-specific data.
    ///
    /// \note Copying results in the data not being copied.  Moving results in the
    /// data being moved.  This is meant to allow uploaded renderer
    /// objects to be copied to not be attached to the same uploaded data.
    template <typename T>
    class DataHandle
    {
    public:

        ///
        /// Constructs a handle with no data.
        DataHandle();

        ///
        /// Constructs a handle with data.
        ///
        /// \note The handle assumes ownership of the lifetime of the data.
        ///
        /// \param data The data.
        DataHandle(Data<T>* data);

        ///
        /// Constructs a handle copied from another.
        ///
        /// \param handle The handle to copy.
        DataHandle(const DataHandle<T>& handle);

        ///
        /// Constructs a handle moved from another.
        ///
        /// \param handle The handle to move.
        DataHandle(DataHandle<T>&& handle);

        ///
        /// Sets the handle as a copy of another.
        ///
        /// \param handle The handle to copy.
        DataHandle& operator=(const DataHandle<T>& handle);

        ///
        /// Sets the handle as being moved from another.
        ///
        /// \param handle The handle to move.
        DataHandle& operator=(DataHandle<T>&& handle);

        ///
        /// Resets the handle to represent new data.
        ///
        /// \param data The new data the handle represents
        void reset(Data<T>* data);

        ///
        /// The data that the handle represents.
        std::unique_ptr<Data<T>> data;
    };

    ///
    /// An object which can be uploaded to a renderer.
    template <typename T>
    class Object
    {
        friend class NullRenderer;
        friend class OpenGLRenderer;
    public:

        ///
        /// Returns whether the object is uploaded to a renderer.
        bool isUploaded() const;

        ///
        /// Returns the renderer that the object is uploaded to.
        ///
        /// \throws Error If the object is not uploaded.
        Renderer& renderer();

        ///
        /// Returns the underlying data cast to the specified type.
        template <typename U>
        U* dataAs() const;

    private:
        void setAsUploaded(Renderer& renderer, Data<T>* data);
        void setAsDestroyed();

        Renderer* _renderer { nullptr };
        DataHandle<T> _handle;
    };

    virtual ~Renderer() { }

    ///
    /// Initialize the renderer.
    ///
    /// \param window The window that the rendere is used for.
    virtual void initialize(Window& window) = 0;

    ///
    /// Begin a new frame.
    virtual void beginFrame() = 0;

    ///
    /// End the current frame.
    virtual void endFrame() = 0;

    ///
    /// Binds a render state.
    ///
    /// \param state The render state to bind.
    virtual void bindState(const RenderState& state) = 0;

    ///
    /// Selects a render target to begin rendering to.
    ///
    /// \note All rendering occurring after this call will render to the
    /// bound target.
    ///
    /// \param renderTarget The render target to bind.
    virtual void bindTarget(RenderTarget& renderTarget) = 0;

    ///
    /// Selects a window as the active render target.
    ///
    /// \param window The window to bind.
    virtual void bindWindow(Window& window) = 0;

    ///
    /// Selects a frame buffer as the active render target.
    ///
    /// \param frameBuffer The frame buffer to bind.
    virtual void bindFrameBuffer(FrameBuffer& frameBuffer) = 0;

    ///
    /// Uploads a frame buffer.
    ///
    /// \param frameBuffer The frame buffer to upload.
    virtual void uploadFrameBuffer(FrameBuffer& frameBuffer) = 0;

    ///
    /// Destroys a frame buffer.
    ///
    /// \param frameBuffer The frame buffer to destroy.
    virtual void destroyFrameBuffer(FrameBuffer& frameBuffer) = 0;

    ///
    /// Uploads a render buffer.
    ///
    /// \param renderBuffer The render buffer to upload.
    virtual void uploadRenderBuffer(RenderBuffer& renderBuffer) = 0;

    ///
    /// Destroys a render buffer.
    ///
    /// \param renderBuffer The render buffer to destroy.
    virtual void destroyRenderBuffer(RenderBuffer& renderBuffer) = 0;

    ///
    /// Binds a shader.
    ///
    /// \param shader The shader to bind.
    virtual void bindShader(Shader& shader) = 0;

    ///
    /// Uploads a shader.
    ///
    /// \param shader The shader to upload.
    virtual void uploadShader(Shader& shader) = 0;

    ///
    /// Destroys a shader.
    ///
    /// \param shader The shader to destroy.
    virtual void destroyShader(Shader& shader) = 0;

    ///
    /// Binds a value to a shader parameter.
    ///
    /// \note The parameter is expected to belong to the bound shader.
    ///
    /// \param parameter The parameter.
    /// \param value The new value.
    ///
    /// \throws Error If no shader is bound or the specified value type differs
    /// from the parameter type.
    virtual void bindShaderParameter(const ShaderParameter& parameter, const ShaderValue& value) = 0;

    ///
    /// Binds an int value to a shader parameter.
    ///
    /// \note The parameter is expected to belong to the bound shader.
    ///
    /// \param parameter The parameter.
    /// \param value The new value.
    ///
    /// \throws Error If no shader is bound or the specified value type differs
    /// from the parameter type.
    virtual void bindShaderParameter(const ShaderParameter& parameter, int value) = 0;

    ///
    /// Binds a real value to a shader parameter.
    ///
    /// \note The parameter is expected to belong to the bound shader.
    ///
    /// \param parameter The parameter.
    /// \param value The new value.
    ///
    /// \throws Error If no shader is bound or the specified value type differs
    /// from the parameter type.
    virtual void bindShaderParameter(const ShaderParameter& parameter, Real value) = 0;

    ///
    /// Binds a 2-dimensional vector value to a shader parameter.
    ///
    /// \note The parameter is expected to belong to the bound shader.
    ///
    /// \param parameter The parameter.
    /// \param value The new value.
    ///
    /// \throws Error If no shader is bound or the specified value type differs
    /// from the parameter type.
    virtual void bindShaderParameter(const ShaderParameter& parameter, const Vector2& value) = 0;

    ///
    /// Binds a 3-dimensional vector value to a shader parameter.
    ///
    /// \note The parameter is expected to belong to the bound shader.
    ///
    /// \param parameter The parameter.
    /// \param value The new value.
    ///
    /// \throws Error If no shader is bound or the specified value type differs
    /// from the parameter type.
    virtual void bindShaderParameter(const ShaderParameter& parameter, const Vector3& value) = 0;

    ///
    /// Binds a 4-dimensional vector value to a shader parameter.
    ///
    /// \note The parameter is expected to belong to the bound shader.
    ///
    /// \param parameter The parameter.
    /// \param value The new value.
    ///
    /// \throws Error If no shader is bound or the specified value type differs
    /// from the parameter type.
    virtual void bindShaderParameter(const ShaderParameter& parameter, const Vector4& value) = 0;

    ///
    /// Binds a 4 by 4 matrix value to a shader parameter.
    ///
    /// \note The parameter is expected to belong to the bound shader.
    ///
    /// \param parameter The parameter.
    /// \param value The new value.
    ///
    /// \throws Error If no shader is bound or the specified value type differs
    /// from the parameter type.
    virtual void bindShaderParameter(const ShaderParameter& parameter, const Matrix4& value) = 0;

    ///
    /// Binds a texture to a shader parameter.
    ///
    /// \note The parameter is expected to belong to the bound shader.
    ///
    /// \param parameter The parameter.
    /// \param texture The texture.
    ///
    /// \throws Error If no shader is bound or the specified value type differs
    /// from the parameter type.
    virtual void bindShaderParameter(const ShaderParameter& parameter, Texture& texture) = 0;

    ///
    /// Binds a value to a shader parameter of the currently bound shader.
    ///
    /// \param parameterName The name of the parameter.
    /// \param value The new value.
    ///
    /// \throws Error If no shader is bound, no parameter of the given name,
    /// exists, or the specified value type differs from the parameter type.
    template <typename T>
    void bindShaderParameter(const std::string& parameterName, const T& value);

    ///
    /// Binds a value to a shader parameter of the currently bound shader.
    ///
    /// \param parameterName The name of the parameter.
    /// \param value The new value.
    ///
    /// \throws Error If no shader is bound, no parameter of the given name,
    /// exists, or the specified value type differs from the parameter type.
    template <typename T>
    void bindShaderParameter(const char* parameterName, const T& value);

    ///
    /// Binds a value to a shader parameter of the currently bound shader.
    ///
    /// \param parameterName The name of the parameter.
    /// \param value The new value.
    ///
    /// \throws Error If no shader is bound, no parameter of the given name,
    /// exists, or the specified value type differs from the parameter type.
    template <typename T>
    void bindShaderParameter(const std::string& parameterName, T& value);

    ///
    /// Binds a value to a shader parameter of the currently bound shader.
    ///
    /// \param parameterName The name of the parameter.
    /// \param value The new value.
    ///
    /// \throws Error If no shader is bound, no parameter of the given name,
    /// exists, or the specified value type differs from the parameter type.
    template <typename T>
    void bindShaderParameter(const char* parameterName, T& value);

    ///
    /// Binds a texture.
    ///
    /// \param texture The texture to bind.
    /// \param index The texture index to bind to.
    ///
    /// \throws Error If the binding index lies outside of the hardware
    /// capabilities.
    virtual void bindTexture(Texture& texture, unsigned index) = 0;

    ///
    /// Uploads a texture.
    ///
    /// \param texture The texture to upload.
    virtual void uploadTexture(Texture& texture) = 0;

    ///
    /// Destroys a texture.
    ///
    /// \param texture The texture to destroy.
    virtual void destroyTexture(Texture& texture) = 0;

    ///
    /// Downloads the 2-dimensional image of the given uploaded texture.
    ///
    /// \param texture The texture the download the image of.
    ///
    /// \returns The downloaded image.
    ///
    /// \throws Error If the texture is not uploaded.
    virtual Image downloadTextureImage(const Texture& texture) = 0;

    ///
    /// Binds a mesh.
    ///
    /// \param mesh The mesh to bind.
    virtual void bindMesh(Mesh& mesh) = 0;

    ///
    /// Uploads a mesh.
    ///
    /// \param mesh The mesh to upload.
    virtual void uploadMesh(Mesh& mesh) = 0;

    ///
    /// Destroys a mesh.
    ///
    /// \param mesh The mesh to destroy.
    virtual void destroyMesh(Mesh& mesh) = 0;

    ///
    /// Draws to the render target in the current state.
    virtual void draw() = 0;

    ///
    /// Clears the render target.
    virtual void clear() = 0;

    ///
    /// Returns the capabilities of the underlying hardware.
    Capabilities& capabilities();

    ///
    /// Returns the capabilities of the underlying hardware.
    const Capabilities& capabilities() const;

    ///
    /// Returns the statistics of the renderer's state.
    Statistics& statistics();

    ///
    /// Returns the statistics of the renderer's state.
    const Statistics& statistics() const;

protected:
    RenderTarget* boundTarget();
    void setBoundTarget(RenderTarget* target);
    Shader* boundShader();
    void setBoundShader(Shader* shader);
    Mesh* boundMesh();
    void setBoundMesh(Mesh* mesh);

private:
    const ShaderParameter& resolveShaderParameter(const std::string& parameterName);
    const ShaderParameter& resolveShaderParameter(const char* parameterName);

    Capabilities _capabilities;
    Statistics _statistics;

    RenderTarget* _boundTarget { nullptr };
    Shader* _boundShader { nullptr };
    Mesh* _boundMesh { nullptr };
};

}

#include "Renderer.inl"
