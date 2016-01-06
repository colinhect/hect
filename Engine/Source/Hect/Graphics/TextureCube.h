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
#include "Hect/Graphics/CubeSide.h"
#include "Hect/Graphics/Image.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/TextureFilter.h"

#include <array>

namespace hect
{

///
/// A cubic texture.
class HECT_EXPORT TextureCube :
    public Asset<TextureCube>,
    public Renderer::Object<TextureCube>
{
public:

    ///
    /// Constructs an empty cubic texture.
    TextureCube();

    ///
    /// Constructs an cubic texture.
    ///
    /// \param name The name.
    /// \param width The width.
    /// \param height The height.
    /// \param pixelFormat The pixel format.
    /// \param minFilter The minification filter.
    /// \param magFilter The magnification filter.
    /// \param mipmapped True if the texture is mipmapped; false otherwise.
    TextureCube(const std::string& name, unsigned width, unsigned height,
                const PixelFormat& pixelFormat = PixelFormat::Rgb8,
                TextureFilter minFilter = TextureFilter::Linear,
                TextureFilter magFilter = TextureFilter::Linear,
                bool mipmapped = true);

    ///
    /// Returns the image of the specified side.
    ///
    /// \param side The side of the cube to get the image of.
    Image& image(CubeSide side);

    ///
    /// Sets the image of the specified side of the texture.
    ///
    /// \note If the texture is uploaded to a renderer then it will be
    /// destroyed.
    ///
    /// \param side The side of the cube to set the image for.
    /// \param image The image.
    ///
    /// \throws InvalidOperation If the image is not compatible with the
    /// texture.
    void setImage(CubeSide side, const Image::Handle& image);

    ///
    /// Invalidate the local images of the texture, forcing the images to be
    /// downloaded from the renderer the next time the images are accessed.
    ///
    /// \throws InvalidOperation If the texture is not uploaded.
    void invalidateLocalImages();

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
    /// Returns the width.
    unsigned width() const;

    ///
    /// Returns the height.
    unsigned height() const;

    ///
    /// Returns the pixel format.
    const PixelFormat& pixelFormat() const;

    ///
    /// Returns whether the texture is equivalent to another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param texture The other texture.
    bool operator==(const TextureCube& texture) const;

    ///
    /// Returns whether the texture is different from another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param texture The other texture.
    bool operator!=(const TextureCube& texture) const;

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    std::array<Image::Handle, 6> _images;

    unsigned _width { 0 };
    unsigned _height { 0 };

    PixelFormat _pixelFormat;

    TextureFilter _minFilter { TextureFilter::Linear };
    TextureFilter _magFilter { TextureFilter::Linear };

    bool _mipmapped { true };
};

}