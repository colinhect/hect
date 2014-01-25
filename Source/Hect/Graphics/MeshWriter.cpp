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
#include "MeshWriter.h"

#include "Hect/Core/Error.h"

using namespace hect;

MeshWriter::MeshWriter(Mesh& mesh) :
    _mesh(&mesh),
    _vertexPosition(0),
    _vertexStream(mesh._vertexData),
    _indexStream(mesh._indexData)
{
}

size_t MeshWriter::addVertex()
{
    _vertexPosition = _vertexStream.position();

    // Push back zeroed data for the added vertex
    for (unsigned i = 0; i < _mesh->vertexLayout().vertexSize(); ++i)
    {
        _vertexStream.writeUnsignedByte(0);
    }

    return _mesh->_vertexCount++;
}

void MeshWriter::writeAttributeData(VertexAttributeSemantic semantic, Real value)
{
    const VertexLayout& vertexLayout = _mesh->vertexLayout();
    if (vertexLayout.hasAttributeWithSemantic(semantic))
    {
        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);
        _setComponentValue(attribute, 0, (float)value);
    }
}

void MeshWriter::writeAttributeData(VertexAttributeSemantic semantic, const Vector2& value)
{
    const VertexLayout& vertexLayout = _mesh->vertexLayout();
    if (vertexLayout.hasAttributeWithSemantic(semantic))
    {
        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);

        unsigned cardinality = attribute.cardinality();

        if (cardinality > 0)
        {
            _setComponentValue(attribute, 0, (float)value.x);
        }

        if (cardinality > 1)
        {
            _setComponentValue(attribute, 1, (float)value.y);
        }
    }
}

void MeshWriter::writeAttributeData(VertexAttributeSemantic semantic, const Vector3& value)
{
    // If this data is a position then expand the bounding box to include it
    if (semantic == VertexAttributeSemantic::Position)
    {
        _mesh->boundingBox().expandToInclude(value);
    }

    const VertexLayout& vertexLayout = _mesh->vertexLayout();
    if (vertexLayout.hasAttributeWithSemantic(semantic))
    {
        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);

        unsigned cardinality = attribute.cardinality();

        if (cardinality > 0)
        {
            _setComponentValue(attribute, 0, (float)value.x);
        }

        if (cardinality > 1)
        {
            _setComponentValue(attribute, 1, (float)value.y);
        }

        if (cardinality > 2)
        {
            _setComponentValue(attribute, 2, (float)value.z);
        }
    }
}

void MeshWriter::writeAttributeData(VertexAttributeSemantic semantic, const Vector4& value)
{
    const VertexLayout& vertexLayout = _mesh->vertexLayout();
    if (vertexLayout.hasAttributeWithSemantic(semantic))
    {
        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);

        unsigned cardinality = attribute.cardinality();

        if (cardinality > 0)
        {
            _setComponentValue(attribute, 0, (float)value.x);
        }

        if (cardinality > 1)
        {
            _setComponentValue(attribute, 1, (float)value.y);
        }

        if (cardinality > 2)
        {
            _setComponentValue(attribute, 2, (float)value.z);
        }

        if (cardinality > 3)
        {
            _setComponentValue(attribute, 3, (float)value.w);
        }
    }
}

void MeshWriter::addIndex(uint64_t value)
{
    // Write the index data based on the type
    switch (_mesh->indexType())
    {
    case IndexType::UnsignedByte:
        _indexStream.writeUnsignedByte((uint8_t)value);
        break;
    case IndexType::UnsignedShort:
        _indexStream.writeUnsignedShort((uint16_t)value);
        break;
    case IndexType::UnsignedInt:
        _indexStream.writeUnsignedInt((uint32_t)value);
        break;
    }

    ++_mesh->_indexCount;
}

void MeshWriter::_setComponentValue(const VertexAttribute& attribute, unsigned index, float value)
{
    size_t position = _vertexStream.position();
    size_t offset = _vertexPosition + attribute.offset();

    // Write the vertex data based on the type
    switch (attribute.type())
    {
    case VertexAttributeType::Byte:
        _vertexStream.seek(offset + index * sizeof(int8_t));
        _vertexStream.writeByte((int8_t)value);
        break;
    case VertexAttributeType::UnsignedByte:
        _vertexStream.seek(offset + index * sizeof(uint8_t));
        _vertexStream.writeUnsignedByte((uint8_t)value);
        break;
    case VertexAttributeType::Short:
        _vertexStream.seek(offset + index * sizeof(int16_t));
        _vertexStream.writeShort((int16_t)value);
        break;
    case VertexAttributeType::UnsignedShort:
        _vertexStream.seek(offset + index * sizeof(uint16_t));
        _vertexStream.writeUnsignedShort((uint16_t)value);
        break;
    case VertexAttributeType::Int:
        _vertexStream.seek(offset + index * sizeof(int32_t));
        _vertexStream.writeInt((int32_t)value);
        break;
    case VertexAttributeType::UnsignedInt:
        _vertexStream.seek(offset + index * sizeof(uint32_t));
        _vertexStream.writeUnsignedInt((uint32_t)value);
        break;
    case VertexAttributeType::Half:
        throw Error("16-bit floats are not yet implemented");
        break;
    case VertexAttributeType::Float:
        _vertexStream.seek(offset + index * sizeof(float));
        _vertexStream.writeFloat(value);
        break;
    }

    _vertexStream.seek(position);
}