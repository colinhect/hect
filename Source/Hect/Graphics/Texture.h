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

#include "Hect/Core/Sequence.h"
#include "Hect/IO/AssetHandle.h"
#include "Hect/IO/Encodable.h"
#include "Hect/Graphics/Image.h"
#include "Hect/Graphics/RendererObject.h"

namespace hect
{

///
/// Describes how a texture pixel is rendered when magnified or minified.
enum TextureFilter
{
    ///
    /// The nearest pixel is selected.
    TextureFilter_Nearest,

    /// The pixel is interpolated linearly.
    TextureFilter_Linear
};

///
/// A type of texture.
enum TextureType
{
    ///
    /// A 2-dimensional texture.
    TextureType_2D,

    ///
    /// A cube map texture.
    TextureType_CubeMap
};

///
/// A texture with one or more source images which can be sampled by the
/// renderer.
class Texture :
    public RendererObject,
    public Encodable
{
public:

    ///
    /// Constructs an empty 2-dimensional texture.
    Texture();

    ///
    /// Constructs a 2-dimensional texture given its dimensions and other
    /// properties.
    ///
    /// \param name The name.
    /// \param width The width.
    /// \param height The height.
    /// \param pixelType The pixel type.
    /// \param pixelFormat The pixel format.
    /// \param minFilter The minification filter.
    /// \param magFilter The magnification filter.
    /// \param mipmapped True if the texture is mipmapped; false otherwise.
    /// \param wrapped True if the texture is wrapped; false otherwise.
    Texture(const std::string& name, unsigned width, unsigned height, PixelType pixelType, PixelFormat pixelFormat, TextureFilter minFilter, TextureFilter magFilter, bool mipmapped, bool wrapped);

    ///
    /// Constructs a 2-dimensional texture given a source image.
    ///
    /// \param name The name.
    /// \param image The source image.
    Texture(const std::string& name, const AssetHandle<Image>& image);

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
    /// Returns the texture type.
    TextureType type();

    ///
    /// Sets the texture type.
    ///
    /// \param type The new texture type.
    void setType(TextureType type);

    ///
    /// Returns the source images.
    Sequence<AssetHandle<Image>> sourceImages();

    ///
    /// Adds a source image to the texture (may affect width/height of
    /// texture).
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed.
    ///
    /// \param image The source image to add.
    ///
    /// \throws Error If the maximum number of source images have been added
    /// based on the texture type or the image does not match the width/height
    /// of the texture.
    void addSourceImage(const AssetHandle<Image>& image);

    ///
    /// Clears all source images that were added to the texture.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed.
    void clearSourceImages();

    ///
    /// Returns the minification filter.
    TextureFilter minFilter() const;

    ///
    /// Sets the minification filter.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed before the filter is set.
    ///
    /// \param filter The new minification filter.
    void setMinFilter(TextureFilter filter);

    ///
    /// Returns the magnification filter.
    TextureFilter magFilter() const;

    ///
    /// Sets the magnification filter.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed before the filter is set.
    ///
    /// \param filter The new magnification filter.
    void setMagFilter(TextureFilter filter);

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
    PixelType pixelType() const;

    ///
    /// Returns the pixel format.
    PixelFormat pixelFormat() const;

    ///
    /// Returns the number of bytes in a pixel of this texture.
    int bytesPerPixel() const;

    void encode(Encoder& encoder) const;
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

private:
    std::string _name;
    TextureType _type;

    std::vector<AssetHandle<Image>> _sourceImages;

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