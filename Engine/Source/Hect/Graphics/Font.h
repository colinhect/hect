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
#include "Hect/IO/Asset.h"
#include "Hect/IO/ByteVector.h"

namespace hect
{

///
/// A font.
class HECT_EXPORT Font :
    public Asset<Font>
{
    friend class VectorRenderer;
public:

    ///
    /// Constructs a blank font.
    Font();

    ///
    /// Constructs a blank font.
    ///
    /// \param name The name of the font.
    Font(const std::string& name);

    ///
    /// Returns whether the font is equivalent to another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param font The other font.
    bool operator==(const Font& font) const;

    ///
    /// Returns whether the font is different from another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param font The other font.
    bool operator!=(const Font& font) const;

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    ByteVector _data;
};

}
