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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/Color.h"
#include "Hect/Graphics/CullMode.h"
#include "Hect/Graphics/Image.h"
#include "Hect/Math/Vector2.h"
#include "Hect/Math/Rectangle.h"

namespace hect
{

class FrameBuffer;
class Uniform;
class UniformValue;
class Mesh;
class RenderTarget;
class Shader;
class Texture2;
class Texture3;
class TextureCube;
class Window;

///
/// A higher-level abstraction over a hardware-accelerated graphics API such as
/// OpenGL or DirectX.
class HECT_EXPORT Renderer :
    public Uncopyable
{
    friend class Engine;
    friend class FrameBuffer;
    friend class Window;
public:

    ///
    /// Contains the capabilities of the underlying hardware.
    struct HECT_EXPORT Capabilities
    {
        ///
        /// The maximum number of textures that can be bound.
        unsigned max_texture_units { 0 };
    };

    ///
    /// Contains the various statistics of the renderer's state.
    struct HECT_EXPORT Statistics
    {
        ///
        /// The number of bytes allocated on the GPU.
        size_t memory_usage { 0 };
    };

    ///
    /// Implementation-specific data providing a handle for a Renderer::Object.
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
    /// A handle to implementation-specific data.
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
        friend class Renderer;
    public:

        ///
        /// Returns whether the object is uploaded to a renderer.
        bool is_uploaded() const;

        ///
        /// Returns the renderer that the object is uploaded to.
        ///
        /// \throws InvalidOperation If the object is not uploaded.
        Renderer& renderer();

        ///
        /// Returns the underlying data cast to the specified type.
        template <typename U>
        U* data_as() const;

        ///
        /// Sets the object as being uploaded to the renderer.
        ///
        /// \param renderer The renderer.
        /// \param data The renderer-specific data of the object.
        void set_as_uploaded(Renderer& renderer, Data<T>* data);

        ///
        /// Sets the object as being destroyed from the renderer.
        void set_as_destroyed();

    private:
        Renderer* _renderer { nullptr };
        DataHandle<T> _handle;
    };

    ///
    /// Provides a context for rendering to a target.
    class HECT_EXPORT Frame :
        public Uncopyable
    {
        friend class Renderer;
    public:
        Frame(Frame&& frame);
        ~Frame();

        ///
        /// Sets the active cull mode to use for subsequent render calls within
        /// the frame.
        ///
        /// \param cull_mode The cull mode.
        void set_cull_mode(CullMode cull_mode);

        ///
        /// Sets the active shader affecting following render calls within the
        /// frame.
        ///
        /// \note The shader will be uploaded if it was not already uploaded.
        ///
        /// \param shader The shader to set.
        void set_shader(Shader& shader);

        ///
        /// Sets the value for a uniform of the active shader.
        ///
        /// \warning The uniform is expected to belong to the active shader.
        ///
        /// \param uniform The uniform to set the value for.
        /// \param value The value.
        ///
        /// \throws InvalidOperation If the specified value type differs from
        /// the uniform type.
        void set_uniform(const Uniform& uniform, const UniformValue& value);

        ///
        /// \copydoc hect::Renderer::Frame::set_uniform()
        void set_uniform(const Uniform& uniform, int value);

        ///
        /// \copydoc hect::Renderer::Frame::set_uniform()
        void set_uniform(const Uniform& uniform, double value);

        ///
        /// \copydoc hect::Renderer::Frame::set_uniform()
        void set_uniform(const Uniform& uniform, Vector2 value);

        ///
        /// \copydoc hect::Renderer::Frame::set_uniform()
        void set_uniform(const Uniform& uniform, Vector3 value);

        ///
        /// \copydoc hect::Renderer::Frame::set_uniform()
        void set_uniform(const Uniform& uniform, Vector4 value);

        ///
        /// \copydoc hect::Renderer::Frame::set_uniform()
        void set_uniform(const Uniform& uniform, const Matrix4& value);

        ///
        /// \copydoc hect::Renderer::Frame::set_uniform()
        void set_uniform(const Uniform& uniform, Color value);

        ///
        /// \copydoc hect::Renderer::Frame::set_uniform()
        void set_uniform(const Uniform& uniform, Texture2& value);

        ///
        /// \copydoc hect::Renderer::Frame::set_uniform()
        void set_uniform(const Uniform& uniform, Texture3& value);

        ///
        /// \copydoc hect::Renderer::Frame::set_uniform()
        void set_uniform(const Uniform& uniform, TextureCube& value);

        ///
        /// Render a mesh using the active state of the frame.
        ///
        /// \note The mesh will be uploaded if it was not already uploaded.
        ///
        /// \param mesh The mesh to set.
        void render_mesh(Mesh& mesh);

        ///
        /// Render a viewport using the active state of the frame.
        void render_viewport();

        ///
        /// Clears the render target.
        ///
        /// \param color The color to clear the render target to.
        /// \param depth Whether the depth channel is cleared.
        void clear(Color color = Color::Zero, bool depth = true);

    private:
        Frame(Renderer& renderer, RenderTarget& target);

        Renderer& _renderer;
    };

    ~Renderer();

    ///
    /// Begins a new context for rendering graphics to a target.
    ///
    /// \param target The target to render the frame to.
    ///
    /// \throws InvalidOperation If another frame is already active.
    Frame begin_frame(RenderTarget& target);

    ///
    /// Uploads a frame buffer.
    ///
    /// \note If the frame buffer is already uploaded then no action is taken.
    ///
    /// \param frame_buffer The frame buffer to upload.
    void upload_frame_buffer(FrameBuffer& frame_buffer);

    ///
    /// Destroys a frame buffer.
    ///
    /// \param frame_buffer The frame buffer to destroy.
    void destroy_frame_buffer(FrameBuffer& frame_buffer);

    ///
    /// Uploads a shader.
    ///
    /// \note If the shader is already uploaded then no action is taken.
    ///
    /// \param shader The shader to upload.
    ///
    /// \throws InvalidOperation If the shader is invalid.
    void upload_shader(Shader& shader);

    ///
    /// Destroys a shader.
    ///
    /// \param shader The shader to destroy.
    void destroy_shader(Shader& shader);

    ///
    /// Uploads a 2-dimensional texture.
    ///
    /// \note If the texture is already uploaded then no action is taken.
    ///
    /// \param texture The texture to upload.
    void upload_texture(Texture2& texture);

    ///
    /// Uploads a 3-dimensional texture.
    ///
    /// \note If the texture is already uploaded then no action is taken.
    ///
    /// \param texture The texture to upload.
    void upload_texture(Texture3& texture);

    ///
    /// Uploads a cubic texture.
    ///
    /// \note If the texture is already uploaded then no action is taken.
    ///
    /// \param texture The texture to upload.
    void upload_texture(TextureCube& texture);

    ///
    /// Destroys a 2-dimensional texture.
    ///
    /// \param texture The texture to destroy.
    /// \param download_image Whether to download the texture's image before it
    /// it is destroyed.
    void destroy_texture(Texture2& texture, bool download_image = false);

    ///
    /// Destroys a 3-dimensional texture.
    ///
    /// \param texture The texture to destroy.
    /// \param download_image Whether to download the texture's image before it
    /// it is destroyed.
    void destroy_texture(Texture3& texture, bool download_image = false);

    ///
    /// Destroys a cubic texture.
    ///
    /// \param texture The texture to destroy.
    /// \param download_image Whether to download the texture's image before it
    /// it is destroyed.
    void destroy_texture(TextureCube& texture, bool download_image = false);

    ///
    /// Downloads the 2-dimensional image of the given uploaded texture.
    ///
    /// \param texture The texture the download the image of.
    ///
    /// \throws InvalidOperation If the texture is not uploaded.
    void download_texture_image(Texture2& texture);

    ///
    /// Downloads the 2-dimensional images of the given uploaded texture.
    ///
    /// \param texture The texture the download the image of.
    ///
    /// \throws InvalidOperation If the texture is not uploaded.
    void download_texture_images(Texture3& texture);

    ///
    /// Uploads a mesh.
    ///
    /// \note If the mesh is already uploaded then no action is taken.
    ///
    /// \param mesh The mesh to upload.
    void upload_mesh(Mesh& mesh);

    ///
    /// Destroys a mesh.
    ///
    /// \param mesh The mesh to destroy.
    void destroy_mesh(Mesh& mesh);

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

private:
    Renderer();

    void initialize();
    void shutdown();

    void on_begin_frame(RenderTarget& target);
    void on_end_frame();

    void set_target(RenderTarget& target);
    void set_target(Window& window);
    void set_target(FrameBuffer& frame_buffer);

    void set_cull_mode(CullMode cull_mode);

    void set_shader(Shader& shader);

    void set_uniform(const Uniform& uniform, int value);
    void set_uniform(const Uniform& uniform, double value);
    void set_uniform(const Uniform& uniform, Vector2 value);
    void set_uniform(const Uniform& uniform, Vector3 value);
    void set_uniform(const Uniform& uniform, Vector4 value);
    void set_uniform(const Uniform& uniform, const Matrix4& value);
    void set_uniform(const Uniform& uniform, Color value);
    void set_uniform(const Uniform& uniform, Texture2& value);
    void set_uniform(const Uniform& uniform, Texture3& value);
    void set_uniform(const Uniform& uniform, TextureCube& value);

    void render_mesh(Mesh& mesh);

    void render_viewport();

    void clear(Color color, bool depth);

    Capabilities _capabilities;
    Statistics _statistics;
    bool _in_frame { false };
};

}

#include "Renderer.inl"
