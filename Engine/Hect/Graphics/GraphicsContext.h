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

class FrameBuffer;
class Mesh;
class RenderTarget;
class Shader;
class Texture;
class Uniform;
class UniformValue;
class Window;

///
/// A higher-level abstraction over a hardware-accelerated graphics API such as
/// OpenGL or DirectX.
class GraphicsContext :
    public Uncopyable
{
public:

    ///
    /// Contains the capabilities of the underlying hardware.
    struct Capabilities
    {
        ///
        /// The maximum number of textures that can be bound.
        unsigned maxTextureUnits;
    };

    ///
    /// Data which provides a handle to the API-specific data.
    template <typename T>
    class Data
    {
    public:

        ///
        /// Constructs the data given the graphics context and the object that
        /// the data represents.
        ///
        /// \param graphicsContext The graphics context that the data is
        /// uploaded to.
        /// \param object The object that the data represents.
        Data(GraphicsContext& graphicsContext, T& object);

        virtual ~Data();

        ///
        /// Invalidates the GPU data.
        void invalidate();

    protected:
        GraphicsContext* graphicsContext;
        T* object;
    };

    ///
    /// A handle to API-specific data.
    ///
    /// \note Copying results in the data not being copied.  Moving results in the
    /// data being moved.  This is meant to allow uploaded graphics context
    /// objects to be copied to not be attached to the same uploaded data.
    template <typename T>
    class DataHandle
    {
    public:

        ///
        /// Constructs a handle with no data.
        DataHandle();

        ///
        /// Constructs a handle given the data.
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
    /// An object which can be uploaded to a graphics context.
    template <typename T>
    class Object
    {
        friend class GraphicsContext;
    public:
        virtual ~Object();

        ///
        /// Returns whether the object is uploaded to a graphics context.
        bool isUploaded() const;

        ///
        /// Returns the graphics context that the object is uploaded to.
        ///
        /// \throws Error If the object is not uploaded.
        GraphicsContext& graphicsContext();

    private:

        template <typename U>
        U* dataAs() const;

        void setAsUploaded(GraphicsContext& graphicsContext, Data<T>* data);
        void setAsDestroyed();

        GraphicsContext* _graphicsContext { nullptr };
        DataHandle<T> _handle;
    };

    ///
    /// Constructs a graphics context given the window its context belongs to.
    ///
    /// \param window The window which has the context.
    GraphicsContext(Window& window);

    ///
    /// Begin a new frame.
    void beginFrame();

    ///
    /// End the current frame.
    void endFrame();

    ///
    /// Binds a render state.
    ///
    /// \param state The render state to bind.
    void bindState(const RenderState& state);

    ///
    /// Selects a render target to begin rendering to.
    ///
    /// \note All rendering occurring after this call will render to the
    /// bound target.
    ///
    /// \param renderTarget The render target to bind.
    void bindTarget(RenderTarget& renderTarget);

    ///
    /// Selects a window as the active render target.
    ///
    /// \param window The window to bind.
    void bindWindow(Window& window);

    ///
    /// Selects a frame buffer as the active render target.
    ///
    /// \param frameBuffer The frame buffer to bind.
    void bindFrameBuffer(FrameBuffer& frameBuffer);

    ///
    /// Uploads a frame buffer.
    ///
    /// \param frameBuffer The frame buffer to upload.
    void uploadFrameBuffer(FrameBuffer& frameBuffer);

    ///
    /// Destroys a frame buffer.
    ///
    /// \param frameBuffer The frame buffer to destroy.
    void destroyFrameBuffer(FrameBuffer& frameBuffer);

    ///
    /// Binds a shader.
    ///
    /// \param shader The shader to bind.
    void bindShader(Shader& shader);

    ///
    /// Uploads a shader.
    ///
    /// \param shader The shader to upload.
    void uploadShader(Shader& shader);

    ///
    /// Destroys a shader.
    ///
    /// \param shader The shader to destroy.
    void destroyShader(Shader& shader);

    ///
    /// Sets the value of a uniform.
    ///
    /// \param uniform The uniform.
    /// \param value The new value.
    void setUniform(const Uniform& uniform, const UniformValue& value);

    ///
    /// Binds a texture.
    ///
    /// \param texture The texture to bind.
    /// \param index The texture index to bind to.
    ///
    /// \throws Error If the binding index lies outside of the hardware
    /// capabilities.
    void bindTexture(Texture& texture, unsigned index);

    ///
    /// Uploads a texture.
    ///
    /// \param texture The texture to upload.
    void uploadTexture(Texture& texture);

    ///
    /// Destroys a texture.
    ///
    /// \param texture The texture to destroy.
    void destroyTexture(Texture& texture);

    ///
    /// Downloads the 2-dimensional image of the given uploaded texture.
    ///
    /// \param texture The texture the download the image of.
    ///
    /// \returns The downloaded image.
    ///
    /// \throws Error If the texture is not uploaded.
    Image downloadTextureImage(const Texture& texture);

    ///
    /// Binds a mesh.
    ///
    /// \param mesh The mesh to bind.
    void bindMesh(Mesh& mesh);

    ///
    /// Uploads a mesh.
    ///
    /// \param mesh The mesh to upload.
    void uploadMesh(Mesh& mesh);

    ///
    /// Destroys a mesh.
    ///
    /// \param mesh The mesh to destroy.
    void destroyMesh(Mesh& mesh);

    ///
    /// Draws to the render target in the current state.
    void draw();

    ///
    /// Clears the render target.
    void clear();

    ///
    /// Returns the capabilities of the underlying hardware.
    const Capabilities& capabilities() const;

private:
    void initialize();

    Capabilities _capabilities;

    RenderTarget* _boundTarget { nullptr };
    Shader* _boundShader { nullptr };
    Mesh* _boundMesh { nullptr };
    std::vector<Texture*> _boundTextures;
};

}

#include "GraphicsContext.inl"