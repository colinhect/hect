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

#include "Hect/Graphics/BlendFactor.h"
#include "Hect/Graphics/BlendFunction.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

class BlendMode
{
public:
    BlendFunction function() const;
    void setFunction(BlendFunction function);

    BlendFactor sourceFactor() const;
    void setSourceFactor(BlendFactor factor);

    BlendFactor destinationFactor() const;
    void setDestinationFactor(BlendFactor factor);

    ///
    /// Returns whether the blend mode is equivalent to another.
    ///
    /// \param shader The other blend mode.
    bool operator==(const BlendMode& blendMode) const;

    ///
    /// Returns whether the blend mode is different from another.
    ///
    /// \param shader The other blend mode.
    bool operator!=(const BlendMode& blendMode) const;

    friend HECT_EXPORT Encoder& operator<<(Encoder& encoder, const BlendMode& blendMode);
    friend HECT_EXPORT Decoder& operator>>(Decoder& decoder, BlendMode& blendMode);

private:
    BlendFunction _function { BlendFunction_Add };
    BlendFactor _sourceFactor { BlendFactor_One };
    BlendFactor _destinationFactor { BlendFactor_Zero };
};

}