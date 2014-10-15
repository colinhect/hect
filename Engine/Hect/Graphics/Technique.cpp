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
#include "Technique.h"

using namespace hect;

Technique::PassSequence Technique::passes()
{
    return _passes;
}

const Technique::PassSequence Technique::passes() const
{
    return _passes;
}

void Technique::addPass(const Pass& pass)
{
    _passes.push_back(pass);
}

void Technique::clearPasses()
{
    _passes.clear();
}

bool Technique::operator==(const Technique& technique) const
{
    if (_passes.size() != technique._passes.size())
    {
        return false;
    }

    for (size_t i = 0; i < _passes.size(); ++i)
    {
        if (_passes[i] != technique._passes[i])
        {
            return false;
        }
    }

    return true;
}

bool Technique::operator!=(const Technique& technique) const
{
    return !(*this == technique);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Technique& technique)
{
    return encoder << beginObject()
           << encodeVector("passes", technique._passes)
           << endObject();
}

Decoder& operator>>(Decoder& decoder, Technique& technique)
{
    return decoder >> beginObject()
           >> decodeVector("passes", technique._passes)
           >> endObject();
}

}