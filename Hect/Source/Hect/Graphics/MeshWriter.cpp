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
        _vertexStream << static_cast<uint8_t>(0);
    }

    return _mesh->_vertexCount++;
}

void MeshWriter::writeAttributeData(VertexAttributeSemantic semantic, Real value)
{
    const VertexLayout& vertexLayout = _mesh->vertexLayout();
    if (vertexLayout.hasAttributeWithSemantic(semantic))
    {
        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);
        setComponentValue(attribute, 0, static_cast<float>(value));
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
            setComponentValue(attribute, 0, static_cast<float>(value.x));
        }

        if (cardinality > 1)
        {
            setComponentValue(attribute, 1, static_cast<float>(value.y));
        }
    }
}

void MeshWriter::writeAttributeData(VertexAttributeSemantic semantic, const Vector3& value)
{
    // If this data is a position then expand the bounding box to include it
    if (semantic == VertexAttributeSemantic_Position)
    {
        _mesh->axisAlignedBox().expandToInclude(value);
    }

    const VertexLayout& vertexLayout = _mesh->vertexLayout();
    if (vertexLayout.hasAttributeWithSemantic(semantic))
    {
        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);

        unsigned cardinality = attribute.cardinality();

        if (cardinality > 0)
        {
            setComponentValue(attribute, 0, static_cast<float>(value.x));
        }

        if (cardinality > 1)
        {
            setComponentValue(attribute, 1, static_cast<float>(value.y));
        }

        if (cardinality > 2)
        {
            setComponentValue(attribute, 2, static_cast<float>(value.z));
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
            setComponentValue(attribute, 0, static_cast<float>(value.x));
        }

        if (cardinality > 1)
        {
            setComponentValue(attribute, 1, static_cast<float>(value.y));
        }

        if (cardinality > 2)
        {
            setComponentValue(attribute, 2, static_cast<float>(value.z));
        }

        if (cardinality > 3)
        {
            setComponentValue(attribute, 3, static_cast<float>(value.w));
        }
    }
}

void MeshWriter::addIndex(uint64_t value)
{
    // Write the index data based on the type
    switch (_mesh->indexType())
    {
    case IndexType_UInt8:
        _indexStream << static_cast<uint8_t>(value);
        break;
    case IndexType_UInt16:
        _indexStream << static_cast<uint16_t>(value);
        break;
    case IndexType_UInt32:
        _indexStream << static_cast<uint32_t>(value);
        break;
    }

    ++_mesh->_indexCount;
}

void MeshWriter::setComponentValue(const VertexAttribute& attribute, unsigned index, float value)
{
    size_t position = _vertexStream.position();
    size_t offset = _vertexPosition + attribute.offset();

    // Write the vertex data based on the type
    switch (attribute.type())
    {
    case VertexAttributeType_Int8:
        _vertexStream.seek(offset + index * sizeof(int8_t));
        _vertexStream << static_cast<int8_t>(value);
        break;
    case VertexAttributeType_UInt8:
        _vertexStream.seek(offset + index * sizeof(uint8_t));
        _vertexStream << static_cast<uint8_t>(value);
        break;
    case VertexAttributeType_Int16:
        _vertexStream.seek(offset + index * sizeof(int16_t));
        _vertexStream << static_cast<int16_t>(value);
        break;
    case VertexAttributeType_UInt16:
        _vertexStream.seek(offset + index * sizeof(uint16_t));
        _vertexStream << static_cast<uint16_t>(value);
        break;
    case VertexAttributeType_Int32:
        _vertexStream.seek(offset + index * sizeof(int32_t));
        _vertexStream << static_cast<int32_t>(value);
        break;
    case VertexAttributeType_UInt32:
        _vertexStream.seek(offset + index * sizeof(uint32_t));
        _vertexStream << static_cast<uint32_t>(value);
        break;
    case VertexAttributeType_Float32:
        _vertexStream.seek(offset + index * sizeof(float));
        _vertexStream << value;
        break;
    }

    _vertexStream.seek(position);
}