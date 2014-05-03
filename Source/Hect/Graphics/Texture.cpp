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
#include "Texture.h"

#include "Hect/Core/Format.h"
#include "Hect/Core/Error.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/IO/Encoders/TextureEncoder.h"

using namespace hect;

Texture::Texture() :
    _minFilter(TextureFilter::Linear),
    _magFilter(TextureFilter::Linear),
    _mipmapped(true),
    _wrapped(false)
{
    setImage(new Image());
}

Texture::Texture(const std::string& name) :
    _name(name),
    _minFilter(TextureFilter::Linear),
    _magFilter(TextureFilter::Linear),
    _mipmapped(true),
    _wrapped(false)
{
    setImage(new Image());
}

Texture::Texture(const std::string& name, unsigned width, unsigned height, PixelType::Enum pixelType, PixelFormat::Enum pixelFormat, TextureFilter::Enum minFilter, TextureFilter::Enum magFilter, bool mipmapped, bool wrapped) :
    _name(name),
    _image(new Image(width, height, pixelType, pixelFormat)),
    _width(width),
    _height(height),
    _pixelType(pixelType),
    _pixelFormat(pixelFormat),
    _minFilter(minFilter),
    _magFilter(magFilter),
    _mipmapped(mipmapped),
    _wrapped(wrapped)
{
}

Texture::Texture(const std::string& name, const AssetHandle<Image>& image) :
    _name(name),
    _minFilter(TextureFilter::Linear),
    _magFilter(TextureFilter::Linear),
    _mipmapped(true),
    _wrapped(false)
{
    setImage(image);
}

Texture::Texture(const Texture& texture) :
    RendererObject(texture),
    _name(texture._name),
    _width(texture.width()),
    _height(texture.height()),
    _pixelType(texture.pixelType()),
    _pixelFormat(texture.pixelFormat()),
    _minFilter(texture.minFilter()),
    _magFilter(texture.magFilter()),
    _mipmapped(texture.isMipmapped()),
    _wrapped(texture.isWrapped())
{
    if (texture.isUploaded())
    {
        // Download the image for the source texture and copy it as the source
        // image for this texture
        setImage(new Image(texture.renderer().downloadTextureImage(texture)));
    }
}

Texture::Texture(Texture&& texture) :
    RendererObject(texture),
    _name(std::move(texture._name)),
    _image(texture._image),
    _width(texture.width()),
    _height(texture.height()),
    _pixelType(texture.pixelType()),
    _pixelFormat(texture.pixelFormat()),
    _minFilter(texture.minFilter()),
    _magFilter(texture.magFilter()),
    _mipmapped(texture.isMipmapped()),
    _wrapped(texture.isWrapped())
{
}

Texture::~Texture()
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }
}

const std::string& Texture::name() const
{
    return _name;
}

void Texture::setName(const std::string& name)
{
    _name = name;
}

void Texture::setImage(const AssetHandle<Image>& image)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    _image = image;
    _width = image->width();
    _height = image->height();
    _pixelType = image->pixelType();
    _pixelFormat = image->pixelFormat();
}

TextureFilter::Enum Texture::minFilter() const
{
    return _minFilter;
}

void Texture::setMinFilter(TextureFilter::Enum filter)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    _minFilter = filter;
}

TextureFilter::Enum Texture::magFilter() const
{
    return _magFilter;
}

void Texture::setMagFilter(TextureFilter::Enum filter)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    _magFilter = filter;
}

bool Texture::isMipmapped() const
{
    return _mipmapped;
}

void Texture::setMipmapped(bool mipmapped)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    _mipmapped = mipmapped;
}

bool Texture::isWrapped() const
{
    return _wrapped;
}

void Texture::setWrapped(bool wrapped)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    _wrapped = wrapped;
}

unsigned Texture::width() const
{
    return _width;
}

unsigned Texture::height() const
{
    return _height;
}

PixelFormat::Enum Texture::pixelFormat() const
{
    return _pixelFormat;
}

PixelType::Enum Texture::pixelType() const
{
    return _pixelType;
}

int Texture::bytesPerPixel() const
{
    int componentCount = 0;

    switch (_pixelFormat)
    {
    case PixelFormat::Rgb:
        componentCount = 3;
        break;
    case PixelFormat::Rgba:
        componentCount = 4;
        break;
    }

    switch (_pixelType)
    {
    case PixelType::Half:
        return componentCount * 2;
        break;
    case PixelType::Float:
        return componentCount * 4;
        break;
    case PixelType::Byte:
        return componentCount * 1;
        break;
    }

    return 0;
}

void Texture::encode(ObjectEncoder& encoder) const
{
    TextureEncoder::encode(*this, encoder);
}

void Texture::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    TextureEncoder::decode(*this, decoder, assetCache);
}

Texture& Texture::operator=(const Texture& texture)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    RendererObject::operator=(texture);

    _name = texture.name();

    if (texture.isUploaded())
    {
        // Download the image for the source texture and copy it as the source
        // image for this texture
        _image = new Image(texture.renderer().downloadTextureImage(texture));
    }
    else
    {
        _image = texture._image;
    }

    _width = texture.width();
    _height = texture.height();
    _pixelType = texture.pixelType();
    _pixelFormat = texture.pixelFormat();
    _minFilter = texture.minFilter();
    _magFilter = texture.magFilter();
    _mipmapped = texture.isMipmapped();
    _wrapped = texture.isWrapped();

    return *this;
}

Texture& Texture::operator=(Texture&& texture)
{
    if (isUploaded())
    {
        renderer().destroyTexture(*this);
    }

    RendererObject::operator=(texture);

    _name = std::move(texture.name());
    _image = texture._image;
    _width = texture.width();
    _height = texture.height();
    _pixelType = texture.pixelType();
    _pixelFormat = texture.pixelFormat();
    _minFilter = texture.minFilter();
    _magFilter = texture.magFilter();
    _mipmapped = texture.isMipmapped();
    _wrapped = texture.isWrapped();

    return *this;
}