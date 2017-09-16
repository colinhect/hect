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
#include "BlendMode.h"

using namespace hect;

BlendMode::BlendMode()
{
}

BlendMode::BlendMode(BlendFunction function, BlendFactor source_factor, BlendFactor destination_factor) :
    _function(function),
    _source_factor(source_factor),
    _destination_factor(destination_factor)
{
}

BlendFunction BlendMode::function() const
{
    return _function;
}

void BlendMode::set_function(BlendFunction function)
{
    _function = function;
}

BlendFactor BlendMode::source_factor() const
{
    return _source_factor;
}

void BlendMode::set_source_factor(BlendFactor factor)
{
    _source_factor = factor;
}

BlendFactor BlendMode::destination_factor() const
{
    return _destination_factor;
}

void BlendMode::set_destination_factor(BlendFactor factor)
{
    _destination_factor = factor;
}

bool BlendMode::operator==(const BlendMode& blend_mode) const
{
    return _function == blend_mode._function
           && _source_factor == blend_mode._source_factor
           && _destination_factor == blend_mode._destination_factor;
}

bool BlendMode::operator!=(const BlendMode& blend_mode) const
{
    return !(*this == blend_mode);
}

void BlendMode::encode(Encoder& encoder) const
{
    encoder << encode_enum("function", _function)
            << encode_enum("source_factor", _source_factor)
            << encode_enum("destination_factor", _destination_factor);
}

void BlendMode::decode(Decoder& decoder)
{
    decoder >> decode_enum("function", _function)
            >> decode_enum("source_factor", _source_factor)
            >> decode_enum("destination_factor", _destination_factor);
}
