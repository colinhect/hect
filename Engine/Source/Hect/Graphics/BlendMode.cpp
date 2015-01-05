///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
#include "BlendMode.h"

using namespace hect;

BlendMode::BlendMode()
{
}

BlendMode::BlendMode(BlendFunction function, BlendFactor sourceFactor, BlendFactor destinationFactor) :
    _function(function),
    _sourceFactor(sourceFactor),
    _destinationFactor(destinationFactor)
{
}

BlendFunction BlendMode::function() const
{
    return _function;
}

void BlendMode::setFunction(BlendFunction function)
{
    _function = function;
}

BlendFactor BlendMode::sourceFactor() const
{
    return _sourceFactor;
}

void BlendMode::setSourceFactor(BlendFactor factor)
{
    _sourceFactor = factor;
}

BlendFactor BlendMode::destinationFactor() const
{
    return _destinationFactor;
}

void BlendMode::setDestinationFactor(BlendFactor factor)
{
    _destinationFactor = factor;
}

bool BlendMode::operator==(const BlendMode& blendMode) const
{
    return _function == blendMode._function
           && _sourceFactor == blendMode._sourceFactor
           && _destinationFactor == blendMode._destinationFactor;
}

bool BlendMode::operator!=(const BlendMode& blendMode) const
{
    return !(*this == blendMode);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const BlendMode& blendMode)
{
    encoder << beginObject()
            << encodeEnum("function", blendMode._function)
            << encodeEnum("sourceFactor", blendMode._sourceFactor)
            << encodeEnum("destinationFactor", blendMode._destinationFactor)
            << endObject();

    return encoder;
}

Decoder& operator>>(Decoder& decoder, BlendMode& blendMode)
{
    decoder >> beginObject()
            >> decodeEnum("function", blendMode._function)
            >> decodeEnum("sourceFactor", blendMode._sourceFactor)
            >> decodeEnum("destinationFactor", blendMode._destinationFactor)
            >> endObject();

    return decoder;
}

}