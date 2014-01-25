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
#include "Mesh.h"

#include "Hect/Core/Error.h"
#include "Hect/Graphics/MeshWriter.h"
#include "Hect/Graphics/MeshEncoder.h"
#include "Hect/Graphics/VertexLayoutEncoder.h"
#include "Hect/Graphics/Renderer.h"

using namespace hect;

Mesh::Mesh() :
    _vertexLayout(VertexLayout::createDefault()),
    _primitiveType(PrimitiveType::Triangles),
    _indexType(IndexType::UnsignedShort),
    _vertexCount(0),
    _indexCount(0)
{
}

Mesh::Mesh(const std::string& name) :
    _name(name),
    _vertexLayout(VertexLayout::createDefault()),
    _primitiveType(PrimitiveType::Triangles),
    _indexType(IndexType::UnsignedShort),
    _vertexCount(0),
    _indexCount(0)
{
}

Mesh::Mesh(const std::string& name, const VertexLayout& vertexLayout, PrimitiveType primitiveType, IndexType indexType) :
    _name(name),
    _vertexLayout(vertexLayout),
    _primitiveType(primitiveType),
    _indexType(indexType),
    _vertexCount(0),
    _indexCount(0)
{
}

Mesh::Mesh(const Mesh& mesh) :
    _vertexLayout(mesh._vertexLayout),
    _primitiveType(mesh._primitiveType),
    _indexType(mesh._indexType),
    _vertexData(mesh._vertexData),
    _indexData(mesh._indexData),
    _vertexCount(mesh._vertexCount),
    _indexCount(mesh._indexCount),
    _boundingBox(mesh._boundingBox)
{
}

const std::string& Mesh::name() const
{
    return _name;
}

void Mesh::setVertexData(const VertexData& vertexData)
{
    if (_vertexData.size() > 0)
    {
        throw Error("Attempt to set the vertex data of a mesh with vertex data");
    }

    _vertexData = vertexData;
    _vertexCount = vertexData.size() / _vertexLayout.vertexSize();
}

const Mesh::VertexData& Mesh::vertexData() const
{
    return _vertexData;
}

size_t Mesh::vertexCount() const
{
    return _vertexCount;
}

void Mesh::setIndexData(const IndexData& indexData)
{
    if (_indexData.size() > 0)
    {
        throw Error("Attempt to set the index data of a mesh with index data");
    }

    _indexData = indexData;
    _indexCount = indexData.size() / indexSize();
}

const Mesh::IndexData& Mesh::indexData() const
{
    return _indexData;
}

size_t Mesh::indexCount() const
{
    return _indexCount;
}

unsigned Mesh::indexSize() const
{
    switch (_indexType)
    {
    case IndexType::UnsignedByte:
        return 1;
    case IndexType::UnsignedShort:
        return 2;
    case IndexType::UnsignedInt:
        return 4;
    }
    return 0;
}

const VertexLayout& Mesh::vertexLayout() const
{
    return _vertexLayout;
}

PrimitiveType Mesh::primitiveType() const
{
    return _primitiveType;
}

IndexType Mesh::indexType() const
{
    return _indexType;
}

AxisAlignedBox& Mesh::boundingBox()
{
    return _boundingBox;
}

const AxisAlignedBox& Mesh::boundingBox() const
{
    return _boundingBox;
}

void Mesh::clear()
{
    if (isUploaded())
    {
        renderer()->destroyMesh(*this);
    }

    _vertexData.clear();
    _vertexCount = 0;
    _indexData.clear();
    _indexCount = 0;
    _boundingBox = AxisAlignedBox();
}

void Mesh::save(ObjectEncoder& encoder) const
{
    MeshEncoder::save(*this, encoder);
}

void Mesh::load(ObjectDecoder& decoder, AssetCache& assetCache)
{
    MeshEncoder::load(*this, decoder, assetCache);
}

bool Mesh::operator==(const Mesh& mesh) const
{
    // Compare vertex layout
    if (_vertexLayout != mesh._vertexLayout)
    {
        return false;
    }

    // Compare primitive/index types
    if (_primitiveType != mesh._primitiveType || _indexType != mesh._indexType)
    {
        return false;
    }

    // Compare vertex/index counts
    if (_vertexCount != mesh._vertexCount || _indexCount != mesh._indexCount)
    {
        return false;
    }
    
    // Compare vertex data
    for (size_t i = 0; i < _vertexCount; ++i)
    {
        if (_vertexData[i] != mesh._vertexData[i])
        {
            return false;
        }
    }

    // Compare index data
    for (size_t i = 0; i < _indexCount; ++i)
    {
        if (_indexData[i] != mesh._indexData[i])
        {
            return false;
        }
    }

    return true;
}

bool Mesh::operator!=(const Mesh& mesh) const
{
    return !(*this == mesh);
}