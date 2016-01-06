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
#include "Hect/Core/Sequence.h"
#include "Hect/IO/Asset.h"
#include "Hect/Graphics/Image.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/TextureFilter.h"

namespace hect
{

///
/// A 3-dimensional texture.
class HECT_EXPORT Texture3 :
    public Asset<Texture3>,
    public Renderer::Object<Texture3>
{
public:

    ///
    /// Constructs an empty 3-dimensional texture.
    Texture3();

    ///
    /// Constructs a 3-dimensional texture.
    ///
    /// \param name The name.
    /// \param width The width.
    /// \param height The height.
    /// \param depth The height.
    /// \param pixelFormat The pixel format.
    /// \param minFilter The minification filter.
    /// \param magFilter The magnification filter.
    /// \param mipmapped True if the texture is mipmapped; false otherwise.
    /// \param wrapped True if the texture is wrapped; false otherwise.
    Texture3(const std::string& name, unsigned width, unsigned height, unsigned depth,
             const PixelFormat& pixelFormat = PixelFormat::Rgb8,
             TextureFilter minFilter = TextureFilter::Linear,
             TextureFilter magFilter = TextureFilter::Linear,
             bool mipmapped = false,
             bool wrapped = false);

    ///
    /// Returns the image of the texture for the specified depth.
    ///
    /// \param depth The depth.
    ///
    /// \throws InvalidOperation If the specified depth is greater than the
    /// depth of the texture.
    Image& image(unsigned depth);

    ///
    /// Sets the source image to the texture for the specified depth.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed.
    ///
    /// \param depth The depth.
    /// \param image The image.
    ///
    /// \throws InvalidOperation If the image is not compatible with the
    /// texture.
    void setImage(unsigned depth, const Image::Handle& image);

    ///
    /// Invalidate the local images of the texture, forcing the images to be
    /// downloaded from the renderer the next time the images are accessed.
    ///
    /// \throws InvalidOperation If the texture is not uploaded.
    void invalidateLocalImages();

    ///
    /// Reads a color value of a pixel.
    ///
    /// \param x The x coordinate.
    /// \param y The y coordinate.
    /// \param z The z coordinate.
    Color readPixel(unsigned x, unsigned y, unsigned z);

    ///
    /// Reads a color value of a pixel.
    ///
    /// \param coords The UVW coordinates.
    Color readPixel(const Vector3& coords);

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
    /// Returns the depth.
    unsigned depth() const;

    ///
    /// Returns the pixel format.
    const PixelFormat& pixelFormat() const;

    ///
    /// Returns whether the texture is equivalent to another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param texture The other texture.
    bool operator==(const Texture3& texture) const;

    ///
    /// Returns whether the texture is different from another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param texture The other texture.
    bool operator!=(const Texture3& texture) const;

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    std::vector<Image::Handle> _images;

    unsigned _width { 0 };
    unsigned _height { 0 };
    unsigned _depth { 0 };

    PixelFormat _pixelFormat;

    TextureFilter _minFilter { TextureFilter::Linear };
    TextureFilter _magFilter { TextureFilter::Linear };

    bool _mipmapped { true };
    bool _wrapped { false };
};

}