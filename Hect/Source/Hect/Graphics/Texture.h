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

#include "Hect/Core/Export.h"
#include "Hect/Asset/AssetHandle.h"
#include "Hect/Graphics/Image.h"
#include "Hect/Graphics/RendererObject.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

///
/// Describes how a texture pixel is rendered when magnified or minified.
namespace TextureFilter
{
enum Enum
{
    ///
    /// The nearest pixel is selected.
    Nearest,

    /// The pixel is interpolated linearly.
    Linear
};
}

///
/// A 2-dimensional texture.
class HECT_API Texture :
    public RendererObject,
    public Encodable
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
    /// Constructs a default 2-dimensional texture.
    ///
    /// \param name The name.
    Texture(const std::string& name);

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
    Texture(const std::string& name, unsigned width, unsigned height, PixelType::Enum pixelType, PixelFormat::Enum pixelFormat, TextureFilter::Enum minFilter, TextureFilter::Enum magFilter, bool mipmapped, bool wrapped);

    ///
    /// Constructs a 2-dimensional texture given a source image.
    ///
    /// \note Once the texture is uploaded it will no longer hold a
    /// reference to the source image.
    ///
    /// \param name The name.
    /// \param image The source image.
    Texture(const std::string& name, const AssetHandle<Image>& image);

    ///
    /// Constructs a texture copied from another.
    ///
    /// \param texture The texture to copy.
    Texture(const Texture& texture);

    ///
    /// Constructs a texture moved from another.
    ///
    /// \param texture The texture to move.
    Texture(Texture&& texture);

    ///
    /// Destroys the texture on the GPU if it is uploaded.
    ~Texture();

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Sets the name.
    ///
    /// \param name The new name.
    void setName(const std::string& name);

    ///
    /// Sets the image (may affect width/height of texture).
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed before the image is set.
    ///
    /// \param image The new image.
    void setImage(const AssetHandle<Image>& image);

    ///
    /// Returns the minification filter.
    TextureFilter::Enum minFilter() const;

    ///
    /// Sets the minification filter.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed before the filter is set.
    ///
    /// \param filter The new minification filter.
    void setMinFilter(TextureFilter::Enum filter);

    ///
    /// Returns the magnification filter.
    TextureFilter::Enum magFilter() const;

    ///
    /// Sets the magnification filter.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed before the filter is set.
    ///
    /// \param filter The new magnification filter.
    void setMagFilter(TextureFilter::Enum filter);

    ///
    /// Returns whether the texture is mipmapped.
    bool isMipmapped() const;

    ///
    /// Sets whether the texture is mipmapped.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed before the mipmap value is set.
    ///
    /// \param mipmapped True if the texture is mipmapped; false otherwise.
    void setMipmapped(bool mipmapped);

    ///
    /// Returns whether the texture is wrapped.
    bool isWrapped() const;

    ///
    /// Sets whether the texture is wrapped.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed before the wrap value is set.
    ///
    /// \param wrapped True if the texture is wrapped; false otherwise.
    void setWrapped(bool wrapped);

    ///
    /// Returns the width.
    unsigned width() const;

    ///
    /// Returns the height.
    unsigned height() const;

    ///
    /// Returns the pixel type.
    PixelType::Enum pixelType() const;

    ///
    /// Returns the pixel format.
    PixelFormat::Enum pixelFormat() const;

    ///
    /// Returns the number of bytes in a pixel of this texture.
    int bytesPerPixel() const;

    ///
    /// Encodes the texture.
    ///
    /// \param encoder The encoder to use.
    void encode(ObjectEncoder& encoder) const;

    ///
    /// Decodes the texture.
    ///
    /// \param decoder The decoder to use.
    /// \param assetCache The asset cache to get referenced assets from.
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

    ///
    /// Replaces the texture with a copy of another.
    ///
    /// \note If the texture was uploaded to a renderer then it will be
    /// destroyed before assigned.
    ///
    /// \param texture The texture to copy.
    ///
    /// \returns A reference to the texture.
    Texture& operator=(const Texture& texture);

    ///
    /// Replaces the texture by moving another.
    ///
    /// \note If the texture was uploaded to a renderer then it will be
    /// destroyed before assigned.
    ///
    /// \param texture The texture to move.
    ///
    /// \returns A reference to the texture.
    Texture& operator=(Texture&& texture);

private:
    static TextureFilter::Enum _parseTextureFilter(const std::string& value);

    std::string _name;

    // A texture will only have an image if it hasn't been uploaded yet
    AssetHandle<Image> _image;

    unsigned _width;
    unsigned _height;

    PixelType::Enum _pixelType;
    PixelFormat::Enum _pixelFormat;

    TextureFilter::Enum _minFilter;
    TextureFilter::Enum _magFilter;

    bool _mipmapped;
    bool _wrapped;
};

}