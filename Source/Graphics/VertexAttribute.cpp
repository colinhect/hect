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
#include "Hect.h"

using namespace hect;

VertexAttribute::VertexAttribute(VertexAttributeSemantic semantic, VertexAttributeType type, unsigned cardinality) :
    _semantic(semantic),
    _type(type),
    _cardinality(cardinality),
    _offset(0)
{
}

VertexAttributeSemantic VertexAttribute::semantic() const
{
    return _semantic;
}

VertexAttributeType VertexAttribute::type() const
{
    return _type;
}

unsigned VertexAttribute::cardinality() const
{
    return _cardinality;
}

unsigned VertexAttribute::size() const
{
    switch (_type)
    {
    case VertexAttributeType::Byte:
    case VertexAttributeType::UnsignedByte:
        return 1 * _cardinality;
    case VertexAttributeType::Short:
    case VertexAttributeType::UnsignedShort:
        return 2 * _cardinality;
    case VertexAttributeType::Int:
    case VertexAttributeType::UnsignedInt:
        return 4 * _cardinality;
    case VertexAttributeType::Half:
        return 2 * _cardinality;
    case VertexAttributeType::Float:
        return 4 * _cardinality;
    }

    return 0;
}

unsigned VertexAttribute::offset() const
{
    return _offset;
}