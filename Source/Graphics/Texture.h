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

namespace hect
{

///
/// Describes how a texture pixel is rendered when magnified or minified.
enum class TextureFilter
{
    ///
    /// The nearest pixel is selected.
    Nearest,

    /// The pixel is interpolated linearly.
    Linear
};

///
/// A 2-dimensional texture.
class Texture :
    public RendererObject
{
    friend class Renderer;
public:

    ///
    /// An array of textures.
    typedef std::vector<Texture> Array;

    ///
    /// Constructs a default 2-dimensional texture.
    Texture();

    ///
    /// Constructs a 2-dimensional texture given its properties.
    ///
    /// \param name The name.
    /// \param width The width.
    /// \param height The height.
    /// \param pixelType The pixel type.
    /// \param pixelFormat The pixel format.
    /// \param minFilter The minification filter.
    /// \param magFilter The Magnification filter.
    /// \param mipmapped True if the texture is mipmapped; false otherwise.
    /// \param wrapped True if the texture is wrapped; false otherwise.
    Texture(const std::string& name, unsigned width, unsigned height, PixelType pixelType, PixelFormat pixelFormat, TextureFilter minFilter, TextureFilter magFilter, bool mipmapped, bool wrapped);

    ///
    /// Constructs a 2-dimensional texture given a source image.
    ///
    /// \remarks Once the texture is uploaded it will no longer hold a
    /// reference to the source image.
    ///
    /// \param name The name.
    /// \param image The source image.
    Texture(const std::string& name, Image::Ref image);

    ///
    /// Constructs a copy of another texture.
    ///
    /// \param texture The texture to copy.
    Texture(const Texture& texture);

    ///
    /// Destroys the texture on the GPU if it is uploaded.
    ~Texture();

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Returns the minification filter.
    TextureFilter minFilter() const;

    ///
    /// Sets the minification filter.
    ///
    /// \param filter The new minification filter.
    ///
    /// \throws Error If the texture is uploaded.
    void setMinFilter(TextureFilter filter);

    ///
    /// Returns the magnification filter.
    TextureFilter magFilter() const;

    ///
    /// Sets the magnification filter.
    ///
    /// \param filter The new magnification filter.
    ///
    /// \throws Error If the texture is uploaded.
    void setMagFilter(TextureFilter filter);

    ///
    /// Returns whether the texture is mipmapped.
    bool isMipmapped() const;

    ///
    /// Sets whether the texture is mipmapped.
    ///
    /// \param mipmapped True if the texture is mipmapped; false otherwise.
    ///
    /// \throws Error If the texture is uploaded.
    void setMipmapped(bool mipmapped);

    ///
    /// Returns whether the texture is wrapped.
    bool isWrapped() const;

    ///
    /// Sets whether the texture is wrapped.
    ///
    /// \param wrapped True if the texture is wrapped; false otherwise.
    ///
    /// \throws Error If the texture is uploaded.
    void setWrapped(bool wrapped);

    ///
    /// Returns the width.
    unsigned width() const;

    ///
    /// Returns the height.
    unsigned height() const;

    ///
    /// Returns the pixel type.
    PixelType pixelType() const;

    ///
    /// Returns the pixel format.
    PixelFormat pixelFormat() const;

    ///
    /// Returns the number of bytes in a pixel of this texture.
    int bytesPerPixel() const;

private:
    std::string _name;

    // A texture will only have an image if it hasn't been uploaded yet
    Image::Ref _image;

    unsigned _width;
    unsigned _height;

    PixelType _pixelType;
    PixelFormat _pixelFormat;

    TextureFilter _minFilter;
    TextureFilter _magFilter;

    bool _mipmapped;
    bool _wrapped;
};

}