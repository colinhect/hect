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
#include "Hect/Graphics/PixelType.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

///
/// Describes the format of a pixel.
class HECT_EXPORT PixelFormat :
    public Encodable
{
public:

    ///
    /// 8-bit single-channel pixel format.
    static const PixelFormat R8;

    ///
    /// 16-bit floating-point single-channel pixel format.
    static const PixelFormat R16;

    ///
    /// 32-bit floating-point single-channel pixel format.
    static const PixelFormat R32;

    ///
    /// 8-bit single-channel pixel format.
    static const PixelFormat Rg8;

    ///
    /// 16-bit floating-point dual-channel pixel format.
    static const PixelFormat Rg16;

    ///
    /// 32-bit floating-point dual-channel pixel format.
    static const PixelFormat Rg32;

    ///
    /// 8-bit red/green/blue pixel format.
    static const PixelFormat Rgb8;

    ///
    /// 16-bit floating-point red/green/blue pixel format.
    static const PixelFormat Rgb16;

    ///
    /// 32-bit floating-point red/green/blue pixel format.
    static const PixelFormat Rgb32;

    ///
    /// 8-bit red/green/blue/alpha pixel format.
    static const PixelFormat Rgba8;

    ///
    /// 16-bit floating-point red/green/blue/alpha pixel format.
    static const PixelFormat Rgba16;

    ///
    /// 32-bit floating-point red/green/blue/alpha pixel format.
    static const PixelFormat Rgba32;

    ///
    /// Constructs a default pixel format (PixelFormat::Rgba8).
    PixelFormat();

    ///
    /// Constructs a pixel format.
    ///
    /// \param type The type.
    /// \param cardinality The number of components in a pixel.
    ///
    /// \throws InvalidOperation If the cardinality is 0 or exceeds 4.
    PixelFormat(PixelType type, unsigned cardinality);

    ///
    /// Returns the type.
    PixelType type() const;

    ///
    /// Returns the number of components in a pixel.
    unsigned cardinality() const;

    ///
    /// Returns the total size in bytes in a pixel.
    unsigned size() const;

    ///
    /// Returns whether the format is equivalent to another.
    ///
    /// \param pixel_format The other format.
    bool operator==(const PixelFormat& pixel_format) const;

    ///
    /// Returns whether the format is different from another.
    ///
    /// \param pixel_format The other format.
    bool operator!=(const PixelFormat& pixel_format) const;

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    PixelType _type { PixelType::Byte };
    unsigned _cardinality { 4 };
};

}
