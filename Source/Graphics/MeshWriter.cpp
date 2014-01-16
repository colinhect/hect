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

MeshWriter::MeshWriter(Mesh& mesh) :
    _mesh(&mesh),
    _vertexDataIndex(0)
{
}

size_t MeshWriter::addVertex()
{
    _vertexDataIndex = _mesh->_vertexData.size();

    // Push back zeroed data for the added vertex
    for (unsigned i = 0; i < _mesh->vertexLayout().vertexSize(); ++i)
    {
        _mesh->_vertexData.push_back(0);
    }

    return _mesh->_vertexCount++;
}

void MeshWriter::writeAttributeData(VertexAttributeSemantic semantic, float value)
{
    const VertexAttribute* attribute = _mesh->vertexLayout().attributeWithSemantic(semantic);
    if (attribute)
    {
        _setComponentValue(attribute, 0, value);
    }
}

void MeshWriter::writeAttributeData(VertexAttributeSemantic semantic, const Vector2<float>& value)
{
    const VertexAttribute* attribute = _mesh->vertexLayout().attributeWithSemantic(semantic);
    if (attribute)
    {
        VertexAttributeType type = attribute->type();
        unsigned cardinality = attribute->cardinality();

        // If the type matches and the cardinality is large enough then set it
        // all components at the same time
        if (type == VertexAttributeType::Float && cardinality >= 2)
        {
            _writeAttributeData(*attribute, value);
        }

        // Otherwise set each component individually
        else
        {
            if (cardinality > 0)
            {
                _setComponentValue(attribute, 0, value.x);
            }

            if (cardinality > 1)
            {
                _setComponentValue(attribute, 1, value.y);
            }
        }
    }
}

void MeshWriter::writeAttributeData(VertexAttributeSemantic semantic, const Vector3<float>& value)
{
    // If this data is a position then expand the bounding box to include it
    if (semantic == VertexAttributeSemantic::Position)
    {
        _mesh->boundingBox().expandToInclude(value);
    }

    const VertexAttribute* attribute = _mesh->vertexLayout().attributeWithSemantic(semantic);
    if (attribute)
    {
        VertexAttributeType type = attribute->type();
        unsigned cardinality = attribute->cardinality();

        // If the type matches and the cardinality is large enough then set it
        // all components at the same time
        if (type == VertexAttributeType::Float && cardinality >= 3)
        {
            _writeAttributeData(*attribute, value);
        }

        // Otherwise set each component individually
        else
        {
            if (cardinality > 0)
            {
                _setComponentValue(attribute, 0, value.x);
            }

            if (cardinality > 1)
            {
                _setComponentValue(attribute, 1, value.y);
            }

            if (cardinality > 2)
            {
                _setComponentValue(attribute, 2, value.z);
            }
        }
    }
}

void MeshWriter::writeAttributeData(VertexAttributeSemantic semantic, const Vector4<float>& value)
{
    const VertexAttribute* attribute = _mesh->vertexLayout().attributeWithSemantic(semantic);
    if (attribute)
    {
        VertexAttributeType type = attribute->type();
        unsigned cardinality = attribute->cardinality();

        // If the type matches and the cardinality is large enough then set it
        // all components at the same time
        if (type == VertexAttributeType::Float && cardinality >= 4)
        {
            _writeAttributeData(*attribute, value);
        }

        // Otherwise set each component individually
        else
        {
            if (cardinality > 0)
            {
                _setComponentValue(attribute, 0, value.x);
            }

            if (cardinality > 1)
            {
                _setComponentValue(attribute, 1, value.y);
            }

            if (cardinality > 2)
            {
                _setComponentValue(attribute, 2, value.z);
            }

            if (cardinality > 3)
            {
                _setComponentValue(attribute, 3, value.w);
            }
        }
    }
}

void MeshWriter::addIndex(uint64_t value)
{
    IndexType indexType = _mesh->indexType();
    size_t indexSize = _mesh->indexSize();

    // Push back zeroed data for the added index
    size_t indexDataIndex = _mesh->_indexData.size();
    for (unsigned i = 0; i < indexSize; ++i)
    {
        _mesh->_indexData.push_back(0);
    }

    // Get the location of the index
    void* index = &_mesh->_indexData[indexDataIndex];

    // Set the index data based on the type
    switch (indexType)
    {
    case IndexType::UnsignedByte:
        *(uint8_t*)index = (uint8_t)value;
        break;
    case IndexType::UnsignedShort:
        *(uint16_t*)index = (uint16_t)value;
        break;
    case IndexType::UnsignedInt:
        *(uint32_t*)index = (uint32_t)value;
        break;
    }

    ++_mesh->_indexCount;
}

void MeshWriter::_setComponentValue(const VertexAttribute* attribute, unsigned index, float value)
{
    size_t offset = _vertexDataIndex + attribute->offset();

    // Set the vertex data based on the type
    switch (attribute->type())
    {
    case VertexAttributeType::Byte:
        *(int8_t*)&_mesh->_vertexData[offset + index * 1] = (int8_t)value;
        break;
    case VertexAttributeType::UnsignedByte:
        *(uint8_t*)&_mesh->_vertexData[offset + index * 1] = (uint8_t)value;
        break;
    case VertexAttributeType::Short:
        *(int16_t*)&_mesh->_vertexData[offset + index * 2] = (int16_t)value;
        break;
    case VertexAttributeType::UnsignedShort:
        *(uint16_t*)&_mesh->_vertexData[offset + index * 2] = (uint16_t)value;
        break;
    case VertexAttributeType::Int:
        *(int32_t*)&_mesh->_vertexData[offset + index * 4] = (int32_t)value;
        break;
    case VertexAttributeType::UnsignedInt:
        *(uint32_t*)&_mesh->_vertexData[offset + index * 4] = (uint32_t)value;
        break;
    case VertexAttributeType::Half:
        throw Error("16-bit floats are not yet implemented");
        break;
    case VertexAttributeType::Float:
        *(float*)&_mesh->_vertexData[offset + index * 4] = value;
        break;
    }
}