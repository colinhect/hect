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

#include "Hect/Core/Error.h"

using namespace hect;

MeshReader::MeshReader(const Mesh& mesh) :
    _mesh(&mesh),
    _vertexCount(0),
    _vertexPosition(0),
    _vertexStream(mesh.vertexData()),
    _indexCount(0),
    _indexPosition(0),
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
    _checkVertexBoundary();

    float value = 0;

    const VertexLayout& vertexLayout = _mesh->vertexLayout();
    if (vertexLayout.hasAttributeWithSemantic(semantic))
    {
        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);
        value = _readComponentValue(attribute, 0);
    }

    return value;
}

Vector2 MeshReader::readAttributeVector2(VertexAttributeSemantic semantic) const
{
    _checkVertexBoundary();

    Vector2 value;

    const VertexLayout& vertexLayout = _mesh->vertexLayout();
    if (vertexLayout.hasAttributeWithSemantic(semantic))
    {
        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);

        unsigned cardinality = attribute.cardinality();
        if (cardinality > 0)
        {
            value.x = _readComponentValue(attribute, 0);
        }

        if (cardinality > 1)
        {
            value.y = _readComponentValue(attribute, 1);
        }
    }

    return value;
}

Vector3 MeshReader::readAttributeVector3(VertexAttributeSemantic semantic) const
{
    _checkVertexBoundary();

    Vector3 value;

    const VertexLayout& vertexLayout = _mesh->vertexLayout();
    if (vertexLayout.hasAttributeWithSemantic(semantic))
    {
        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);

        unsigned cardinality = attribute.cardinality();

        if (cardinality > 0)
        {
            value.x = _readComponentValue(attribute, 0);
        }

        if (cardinality > 1)
        {
            value.y = _readComponentValue(attribute, 1);
        }

        if (cardinality > 2)
        {
            value.z = _readComponentValue(attribute, 2);
        }
    }

    return value;
}

Vector4 MeshReader::readAttributeVector4(VertexAttributeSemantic semantic) const
{
    _checkVertexBoundary();

    Vector4 value;

    const VertexLayout& vertexLayout = _mesh->vertexLayout();
    if (vertexLayout.hasAttributeWithSemantic(semantic))
    {
        const VertexAttribute& attribute = vertexLayout.attributeWithSemantic(semantic);

        unsigned cardinality = attribute.cardinality();

        if (cardinality > 0)
        {
            value.x = _readComponentValue(attribute, 0);
        }

        if (cardinality > 1)
        {
            value.y = _readComponentValue(attribute, 1);
        }

        if (cardinality > 2)
        {
            value.z = _readComponentValue(attribute, 2);
        }

        if (cardinality > 3)
        {
            value.w = _readComponentValue(attribute, 3);
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

uint8_t MeshReader::readIndexByte() const
{
    return (uint8_t)readIndexInt();
}

uint16_t MeshReader::readIndexShort() const
{
    return (uint16_t)readIndexInt();
}

uint32_t MeshReader::readIndexInt() const
{
    _checkIndexBoundary();
    _indexStream.seek(_indexPosition);

    // Read the index data based on the type
    uint32_t index = 0;
    switch (_mesh->indexType())
    {
    case IndexType::UnsignedByte:
        index = _indexStream.readUnsignedByte();
        break;
    case IndexType::UnsignedShort:
        index = _indexStream.readUnsignedShort();
        break;
    case IndexType::UnsignedInt:
        index = _indexStream.readUnsignedInt();
        break;
    }

    return index;
}

void MeshReader::_checkVertexBoundary() const
{
    if (_vertexCount == 0)
    {
        throw Error("Attempt to read attribute before moving to the first vertex");
    }
    else if (_vertexCount > _mesh->vertexCount())
    {
        throw Error("Attempt to read past the last vertex");
    }
}

void MeshReader::_checkIndexBoundary() const
{
    if (_indexCount == 0)
    {
        throw Error("Attempt to read index before moving to the first index");
    }
    else if (_indexCount > _mesh->indexCount())
    {
        throw Error("Attempt to read past the last index");
    }
}

float MeshReader::_readComponentValue(const VertexAttribute& attribute, unsigned index) const
{
    _vertexStream.seek(_vertexPosition);

    size_t offset = _vertexPosition + attribute.offset();

    // Read the vertex data based on the type
    float value = 0;
    switch (attribute.type())
    {
    case VertexAttributeType::Byte:
        _vertexStream.seek(offset + index * sizeof(int8_t));
        value = (float)_vertexStream.readByte();
        break;
    case VertexAttributeType::UnsignedByte:
        _vertexStream.seek(offset + index * sizeof(uint8_t));
        value = (float)_vertexStream.readUnsignedByte();
        break;
    case VertexAttributeType::Short:
        _vertexStream.seek(offset + index * sizeof(int16_t));
        value = (float)_vertexStream.readShort();
        break;
    case VertexAttributeType::UnsignedShort:
        _vertexStream.seek(offset + index * sizeof(uint16_t));
        value = (float)_vertexStream.readUnsignedShort();
        break;
    case VertexAttributeType::Int:
        _vertexStream.seek(offset + index * sizeof(int32_t));
        value = (float)_vertexStream.readInt();
        break;
    case VertexAttributeType::UnsignedInt:
        _vertexStream.seek(offset + index * sizeof(uint32_t));
        value = (float)_vertexStream.readUnsignedInt();
        break;
    case VertexAttributeType::Half:
        throw Error("16-bit floats are not yet implemented");
        break;
    case VertexAttributeType::Float:
        _vertexStream.seek(offset + index * sizeof(float));
        value = _vertexStream.readFloat();
        break;
    }

    return value;
}