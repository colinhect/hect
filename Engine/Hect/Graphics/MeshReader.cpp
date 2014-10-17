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
#include "MeshReader.h"

using namespace hect;

MeshReader::MeshReader(const Mesh& mesh) :
    _mesh(&mesh),
    _vertexStream(mesh.vertexData()),
    _indexStream(mesh.indexData())
{
}

bool MeshReader::nextVertex()
{
    if (_vertexCount != 0)
    {
        _vertexPosition += _mesh->vertexLayout().vertexSize();
    }

    ++_vertexCount;
    return _mesh->vertexCount() >= _vertexCount;
}

Real MeshReader::readAttributeReal(VertexAttributeSemantic semantic) const
{
    checkVertexBoundary();

    float value = 0;

    const VertexLayout& vertexLayout = _mesh->vertexLayout();
    if (vertexLayout.hasAttributeWithSemantic(semantic))
    {
        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);
        value = readComponentValue(attribute, 0);
    }

    return value;
}

Vector2 MeshReader::readAttributeVector2(VertexAttributeSemantic semantic) const
{
    checkVertexBoundary();

    Vector2 value;

    const VertexLayout& vertexLayout = _mesh->vertexLayout();
    if (vertexLayout.hasAttributeWithSemantic(semantic))
    {
        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);

        unsigned cardinality = attribute.cardinality();
        if (cardinality > 0)
        {
            value.x = readComponentValue(attribute, 0);
        }

        if (cardinality > 1)
        {
            value.y = readComponentValue(attribute, 1);
        }
    }

    return value;
}

Vector3 MeshReader::readAttributeVector3(VertexAttributeSemantic semantic) const
{
    checkVertexBoundary();

    Vector3 value;

    const VertexLayout& vertexLayout = _mesh->vertexLayout();
    if (vertexLayout.hasAttributeWithSemantic(semantic))
    {
        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);

        unsigned cardinality = attribute.cardinality();

        if (cardinality > 0)
        {
            value.x = readComponentValue(attribute, 0);
        }

        if (cardinality > 1)
        {
            value.y = readComponentValue(attribute, 1);
        }

        if (cardinality > 2)
        {
            value.z = readComponentValue(attribute, 2);
        }
    }

    return value;
}

Vector4 MeshReader::readAttributeVector4(VertexAttributeSemantic semantic) const
{
    checkVertexBoundary();

    Vector4 value;

    const VertexLayout& vertexLayout = _mesh->vertexLayout();
    if (vertexLayout.hasAttributeWithSemantic(semantic))
    {
        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);

        unsigned cardinality = attribute.cardinality();

        if (cardinality > 0)
        {
            value.x = readComponentValue(attribute, 0);
        }

        if (cardinality > 1)
        {
            value.y = readComponentValue(attribute, 1);
        }

        if (cardinality > 2)
        {
            value.z = readComponentValue(attribute, 2);
        }

        if (cardinality > 3)
        {
            value.w = readComponentValue(attribute, 3);
        }
    }

    return value;
}

bool MeshReader::nextIndex()
{
    if (_indexCount != 0)
    {
        _indexPosition += _mesh->indexSize();
    }

    ++_indexCount;
    return _mesh->indexCount() >= _indexCount;
}

uint8_t MeshReader::readIndexUInt8() const
{
    return (uint8_t)readIndexUInt32();
}

uint16_t MeshReader::readIndexUInt16() const
{
    return (uint16_t)readIndexUInt32();
}

uint32_t MeshReader::readIndexUInt32() const
{
    checkIndexBoundary();
    _indexStream.seek(_indexPosition);

    // Read the index data based on the type
    uint32_t index = 0;
    switch (_mesh->indexType())
    {
    case IndexType_UInt8:
    {
        uint8_t value;
        _indexStream >> value;
        index = value;
    }
    break;
    case IndexType_UInt16:
    {
        uint16_t value;
        _indexStream >> value;
        index = value;
    }
    break;
    case IndexType_UInt32:
        _indexStream >> index;
        break;
    }

    return index;
}

void MeshReader::checkVertexBoundary() const
{
    if (_vertexCount == 0)
    {
        throw Error("Cannot read attribute before moving to the first vertex");
    }
    else if (_vertexCount > _mesh->vertexCount())
    {
        throw Error("Cannot read past the last vertex");
    }
}

void MeshReader::checkIndexBoundary() const
{
    if (_indexCount == 0)
    {
        throw Error("Cannot read index before moving to the first index");
    }
    else if (_indexCount > _mesh->indexCount())
    {
        throw Error("Cannot read past the last index");
    }
}

float MeshReader::readComponentValue(const VertexAttribute& attribute, unsigned index) const
{
    _vertexStream.seek(_vertexPosition);

    size_t offset = _vertexPosition + attribute.offset();

    // Read the vertex data based on the type
    float value = 0;
    switch (attribute.type())
    {
    case VertexAttributeType_Int8:
    {
        _vertexStream.seek(offset + index * sizeof(int8_t));
        int8_t readValue;
        _vertexStream >> readValue;
        value = static_cast<float>(readValue);
    }
    break;
    case VertexAttributeType_UInt8:
    {
        _vertexStream.seek(offset + index * sizeof(uint8_t));
        uint8_t readValue;
        _vertexStream >> readValue;
        value = static_cast<float>(readValue);
    }
    break;
    case VertexAttributeType_Int16:
    {
        _vertexStream.seek(offset + index * sizeof(int16_t));
        int16_t readValue;
        _vertexStream >> readValue;
        value = static_cast<float>(readValue);
    }
    break;
    case VertexAttributeType_UInt16:
    {
        _vertexStream.seek(offset + index * sizeof(uint16_t));
        uint16_t readValue;
        _vertexStream >> readValue;
        value = static_cast<float>(readValue);
    }
    break;
    case VertexAttributeType_Int32:
    {
        _vertexStream.seek(offset + index * sizeof(int32_t));
        int32_t readValue;
        _vertexStream >> readValue;
        value = static_cast<float>(readValue);
    }
    break;
    case VertexAttributeType_UInt32:
    {
        _vertexStream.seek(offset + index * sizeof(uint32_t));
        uint32_t readValue;
        _vertexStream >> readValue;
        value = static_cast<float>(readValue);
    }
    break;
    case VertexAttributeType_Float32:
        _vertexStream.seek(offset + index * sizeof(float));
        _vertexStream >> value;
        break;
    }

    return value;
}