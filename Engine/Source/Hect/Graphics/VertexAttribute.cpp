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
#include "VertexAttribute.h"

using namespace hect;

VertexAttribute::VertexAttribute()
{
}

VertexAttribute::VertexAttribute(VertexAttributeSemantic semantic, VertexAttributeType type, unsigned cardinality) :
    _semantic(semantic),
    _type(type),
    _cardinality(cardinality)
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
    case VertexAttributeType::Int8:
    case VertexAttributeType::UInt8:
        return 1 * _cardinality;
    case VertexAttributeType::Int16:
    case VertexAttributeType::UInt16:
        return 2 * _cardinality;
    case VertexAttributeType::Int32:
    case VertexAttributeType::UInt32:
        return 4 * _cardinality;
    case VertexAttributeType::Float32:
        return 4 * _cardinality;
    default:
        break;
    }

    return 0;
}

unsigned VertexAttribute::offset() const
{
    return _offset;
}

bool VertexAttribute::operator==(const VertexAttribute& vertex_attribute) const
{
    return _semantic == vertex_attribute._semantic
           && _type == vertex_attribute._type
           && _cardinality == vertex_attribute._cardinality
           && _offset == vertex_attribute._offset;
}

bool VertexAttribute::operator!=(const VertexAttribute& vertex_attribute) const
{
    return !(*this == vertex_attribute);
}

void VertexAttribute::encode(Encoder& encoder) const
{
    encoder << encode_enum("semantic", _semantic)
            << encode_enum("type", _type)
            << encode_value("cardinality", _cardinality);
}

void VertexAttribute::decode(Decoder& decoder)
{
    decoder >> decode_enum("semantic", _semantic)
            >> decode_enum("type", _type)
            >> decode_value("cardinality", _cardinality);
}
