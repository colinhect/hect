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
/// A 2-dimensional texture.
class HECT_EXPORT Texture2 :
    public Asset<Texture2>,
    public Renderer::Object<Texture2>
{
public:

    ///
    /// Constructs an empty 2-dimensional texture.
    Texture2();

    ///
    /// Constructs a 2-dimensional texture.
    ///
    /// \param name The name.
    /// \param width The width.
    /// \param height The height.
    Texture2(Name name, unsigned width, unsigned height);

    ///
    /// Constructs a 2-dimensional texture.
    ///
    /// \param name The name.
    /// \param image The source image.
    Texture2(Name name, const AssetHandle<Image>& image);

    ///
    /// Returns the image of the texture.
    Image& image();

    ///
    /// Sets the source image to the texture.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed.
    ///
    /// \param image The source image.
    ///
    /// \throws InvalidOperation If the image is not compatible with the
    /// texture.
    void set_image(const AssetHandle<Image>& image);

    ///
    /// Invalidate the local image of the texture, forcing the image to be
    /// downloaded from the renderer the next time the image is accessed.
    ///
    /// \throws InvalidOperation If the texture is not uploaded.
    void invalidate_local_image();

    ///
    /// Reads a color value of a pixel.
    ///
    /// \param x The x coordinate.
    /// \param y The y coordinate.
    Color read_pixel(unsigned x, unsigned y);

    ///
    /// Reads a color value of a pixel.
    ///
    /// \param coords The UV coordinates.
    Color read_pixel(Vector2 coords);

    ///
    /// Returns the minification filter.
    TextureFilter min_filter() const;

    ///
    /// Sets the minification filter.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed before the filter is set.
    ///
    /// \param filter The new minification filter.
    void set_min_filter(TextureFilter filter);

    ///
    /// Returns the magnification filter.
    TextureFilter mag_filter() const;

    ///
    /// Sets the magnification filter.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed before the filter is set.
    ///
    /// \param filter The new magnification filter.
    void set_mag_filter(TextureFilter filter);

    ///
    /// Returns whether the texture is mipmapped.
    bool is_mipmapped() const;

    ///
    /// Sets whether the texture is mipmapped.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed before the mipmap value is set.
    ///
    /// \param mipmapped True if the texture is mipmapped; false otherwise.
    void set_mipmapped(bool mipmapped);

    ///
    /// Returns whether the texture is wrapped.
    bool is_wrapped() const;

    ///
    /// Sets whether the texture is wrapped.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed before the wrap value is set.
    ///
    /// \param wrapped True if the texture is wrapped; false otherwise.
    void set_wrapped(bool wrapped);

    ///
    /// Returns the width.
    unsigned width() const;

    ///
    /// Returns the height.
    unsigned height() const;

    ///
    /// Returns the pixel format.
    PixelFormat pixel_format() const;

    ///
    /// Sets the pixel format.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed before the pixel format is set.
    ///
    /// \param pixel_format The new pixel format.
    void set_pixel_format(PixelFormat pixel_format);

    ///
    /// Returns whether the texture is equivalent to another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param texture The other texture.
    bool operator==(const Texture2& texture) const;

    ///
    /// Returns whether the texture is different from another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param texture The other texture.
    bool operator!=(const Texture2& texture) const;

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    void destroy_if_uploaded();

    AssetHandle<Image> _image;

    unsigned _width { 0 };
    unsigned _height { 0 };

    PixelFormat _pixel_format { PixelFormat::Rgb8 };

    TextureFilter _min_filter { TextureFilter::Linear };
    TextureFilter _mag_filter { TextureFilter::Linear };

    bool _mipmapped { true };
    bool _wrapped { false };
};

}
