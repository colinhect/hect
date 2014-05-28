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
#include "Hect/Graphics/Image.h"
#include "Hect/IO/ReadStream.h"
#include "Hect/IO/WriteStream.h"

namespace hect
{

///
/// Provides functionality for encoding/decoding images from/to PNG data.
class HECT_API ImagePngEncoder
{
public:

    ///
    /// Decodes an image from a stream of PNG data.
    ///
    /// \note The resulting image will be 32-bit RGBA.
    ///
    /// \param image The image to decode to (existing data is lost).
    /// \param stream The stream containing the PNG data.
    static void decode(Image& image, ReadStream& stream);

    ///
    /// Encodes an image as PNG data to a stream.
    ///
    /// \note The image must be 32-bit RGBA format.
    ///
    /// \param image The image to encode.
    /// \param stream The stream to write the PNG data to.
    static void encode(const Image& image, WriteStream& stream);
};

}