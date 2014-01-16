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

MeshReader::MeshReader(const Mesh& mesh) :
    _mesh(&mesh),
    _vertexCount(0),
    _vertexDataIndex(0),
    _indexCount(0),
    _indexDataIndex(0)
{
}

bool MeshReader::nextVertex()
{
    if (_vertexCount != 0)
    {
        _vertexDataIndex += _mesh->vertexLayout().vertexSize();
    }

    ++_vertexCount;
    return _mesh->vertexCount() >= _vertexCount;
}

float MeshReader::readAttributeFloat(VertexAttributeSemantic semantic) const
{
    _checkVertexBoundary();

    float value = 0;

    const VertexAttribute* attribute = _mesh->vertexLayout().attributeWithSemantic(semantic);
    if (attribute)
    {
        value = _readComponentValue(attribute, 0);
    }

    return value;
}

Vector2<float> MeshReader::readAttributeVector2(VertexAttributeSemantic semantic) const
{
    _checkVertexBoundary();

    Vector2<float> value;

    const VertexAttribute* attribute = _mesh->vertexLayout().attributeWithSemantic(semantic);
    if (attribute)
    {
        VertexAttributeType type = attribute->type();
        unsigned cardinality = attribute->cardinality();

        // If the type matches and the cardinality is large enough then read
        // all components at the same time
        if (type == VertexAttributeType::Float && cardinality >= 2)
        {
            value = _readAttributeData<Vector2<float>>(*attribute);
        }

        // Otherwise set each component individually
        else
        {
            if (cardinality > 0)
            {
                value.x = _readComponentValue(attribute, 0);
            }

            if (cardinality > 1)
            {
                value.y = _readComponentValue(attribute, 1);
            }
        }
    }

    return value;
}

Vector3<float> MeshReader::readAttributeVector3(VertexAttributeSemantic semantic) const
{
    _checkVertexBoundary();

    Vector3<float> value;

    const VertexAttribute* attribute = _mesh->vertexLayout().attributeWithSemantic(semantic);
    if (attribute)
    {
        VertexAttributeType type = attribute->type();
        unsigned cardinality = attribute->cardinality();

        // If the type matches and the cardinality is large enough then read
        // all components at the same time
        if (type == VertexAttributeType::Float && cardinality >= 3)
        {
            value = _readAttributeData<Vector3<float>>(*attribute);
        }

        // Otherwise set each component individually
        else
        {
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
    }

    return value;
}

Vector4<float> MeshReader::readAttributeVector4(VertexAttributeSemantic semantic) const
{
    _checkVertexBoundary();

    Vector4<float> value;

    const VertexAttribute* attribute = _mesh->vertexLayout().attributeWithSemantic(semantic);
    if (attribute)
    {
        VertexAttributeType type = attribute->type();
        unsigned cardinality = attribute->cardinality();

        // If the type matches and the cardinality is large enough then read
        // all components at the same time
        if (type == VertexAttributeType::Float && cardinality >= 4)
        {
            value = _readAttributeData<Vector4<float>>(*attribute);
        }

        // Otherwise set each component individually
        else
        {
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
    }

    return value;
}

bool MeshReader::nextIndex()
{
    if (_indexCount != 0)
    {
        _indexDataIndex += _mesh->indexSize();
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

    IndexType indexType = _mesh->indexType();

    // Get the location of the index
    const void* indexData = &_mesh->indexData()[_indexDataIndex];

    // Read the index data based on the type
    uint32_t index = 0;
    switch (indexType)
    {
    case IndexType::UnsignedByte:
        index = *(uint8_t*)indexData;
        break;
    case IndexType::UnsignedShort:
        index = *(uint16_t*)indexData;
        break;
    case IndexType::UnsignedInt:
        index = *(uint32_t*)indexData;
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

float MeshReader::_readComponentValue(const VertexAttribute* attribute, unsigned index) const
{
    size_t offset = _vertexDataIndex + attribute->offset();

    // Read the vertex data based on the type
    float value = 0;
    switch (attribute->type())
    {
    case VertexAttributeType::Byte:
        value = (float)*(int8_t*)&_mesh->vertexData()[offset + index * 1];
        break;
    case VertexAttributeType::UnsignedByte:
        value = (float)*(uint8_t*)&_mesh->vertexData()[offset + index * 1];
        break;
    case VertexAttributeType::Short:
        value = (float)*(int16_t*)&_mesh->vertexData()[offset + index * 2];
        break;
    case VertexAttributeType::UnsignedShort:
        value = (float)*(uint16_t*)&_mesh->vertexData()[offset + index * 2];
        break;
    case VertexAttributeType::Int:
        value = (float)*(int32_t*)&_mesh->vertexData()[offset + index * 4];
        break;
    case VertexAttributeType::UnsignedInt:
        value = (float)*(uint32_t*)&_mesh->vertexData()[offset + index * 4];
        break;
    case VertexAttributeType::Half:
        throw Error("16-bit floats are not yet implemented");
        break;
    case VertexAttributeType::Float:
        value = *(float*)&_mesh->vertexData()[offset + index * 4];
        break;
    }

    return value;
}