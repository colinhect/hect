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
#include "Hect/Graphics/Renderer.h"
#include "Hect/IO/Encoders/MeshEncoder.h"
#include "Hect/IO/Encoders/VertexLayoutEncoder.h"

using namespace hect;

Mesh::Mesh() :
    _vertexLayout(VertexLayout::createDefault()),
    _primitiveType(PrimitiveType_Triangles),
    _indexType(IndexType_UnsignedShort),
    _vertexCount(0),
    _indexCount(0)
{
}

Mesh::Mesh(const std::string& name) :
    _name(name),
    _vertexLayout(VertexLayout::createDefault()),
    _primitiveType(PrimitiveType_Triangles),
    _indexType(IndexType_UnsignedShort),
    _vertexCount(0),
    _indexCount(0)
{
}

Mesh::Mesh(const Mesh& mesh) :
    RendererObject(mesh),
    _name(mesh._name),
    _vertexLayout(mesh._vertexLayout),
    _primitiveType(mesh._primitiveType),
    _indexType(mesh._indexType),
    _vertexData(mesh._vertexData),
    _vertexCount(mesh._vertexCount),
    _indexData(mesh._indexData),
    _indexCount(mesh._indexCount),
    _axisAlignedBox(mesh._axisAlignedBox)
{
}

Mesh::Mesh(Mesh&& mesh) :
    RendererObject(mesh),
    _name(std::move(mesh._name)),
    _vertexLayout(std::move(mesh._vertexLayout)),
    _primitiveType(mesh._primitiveType),
    _indexType(mesh._indexType),
    _vertexData(std::move(mesh._vertexData)),
    _vertexCount(mesh._vertexCount),
    _indexData(std::move(mesh._indexData)),
    _indexCount(mesh._indexCount),
    _axisAlignedBox(mesh._axisAlignedBox)
{
}

Mesh::~Mesh()
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }
}

const std::string& Mesh::name() const
{
    return _name;
}

void Mesh::setName(const std::string& name)
{
    _name = name;
}

const VertexLayout& Mesh::vertexLayout() const
{
    return _vertexLayout;
}

void Mesh::setVertexLayout(const VertexLayout& vertexLayout)
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    if (_vertexData.size() != 0)
    {
        throw Error("Cannot change the vertex layout of a mesh with vertex data");
    }

    _vertexLayout = vertexLayout;
}

PrimitiveType Mesh::primitiveType() const
{
    return _primitiveType;
}

void Mesh::setPrimitiveType(PrimitiveType primitiveType)
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    _primitiveType = primitiveType;
}

IndexType Mesh::indexType() const
{
    return _indexType;
}

void Mesh::setIndexType(IndexType indexType)
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    if (_indexData.size() != 0)
    {
        throw Error("Cannot change the index type of a mesh with index data");
    }

    _indexType = indexType;
}

const Mesh::VertexData& Mesh::vertexData() const
{
    return _vertexData;
}

void Mesh::setVertexData(const VertexData& vertexData)
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    _vertexData = vertexData;
    _vertexCount = vertexData.size() / _vertexLayout.vertexSize();
}

size_t Mesh::vertexCount() const
{
    return _vertexCount;
}

const Mesh::IndexData& Mesh::indexData() const
{
    return _indexData;
}

void Mesh::setIndexData(const IndexData& indexData)
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    _indexData = indexData;
    _indexCount = indexData.size() / indexSize();
}

size_t Mesh::indexCount() const
{
    return _indexCount;
}

unsigned Mesh::indexSize() const
{
    switch (_indexType)
    {
    case IndexType_UnsignedByte:
        return 1;
    case IndexType_UnsignedShort:
        return 2;
    case IndexType_UnsignedInt:
        return 4;
    }
    return 0;
}

AxisAlignedBox& Mesh::axisAlignedBox()
{
    return _axisAlignedBox;
}

const AxisAlignedBox& Mesh::axisAlignedBox() const
{
    return _axisAlignedBox;
}

void Mesh::encode(ObjectEncoder& encoder) const
{
    MeshEncoder::encode(*this, encoder);
}

void Mesh::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    MeshEncoder::decode(*this, decoder, assetCache);
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

Mesh& Mesh::operator=(const Mesh& mesh)
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    RendererObject::operator=(mesh);

    _name = mesh.name();
    _vertexLayout = mesh.vertexLayout();
    _primitiveType = mesh.primitiveType();
    _indexType = mesh.indexType();
    _vertexData = mesh.vertexData();
    _vertexCount = mesh.vertexCount();
    _indexData = mesh.indexData();
    _indexCount = mesh.indexCount();
    _axisAlignedBox = mesh.axisAlignedBox();

    return *this;
}

Mesh& Mesh::operator=(Mesh&& mesh)
{
    if (isUploaded())
    {
        renderer().destroyMesh(*this);
    }

    RendererObject::operator=(mesh);

    _name = std::move(mesh.name());
    _vertexLayout = std::move(mesh.vertexLayout());
    _primitiveType = mesh.primitiveType();
    _indexType = mesh.indexType();
    _vertexData = std::move(mesh.vertexData());
    _vertexCount = mesh.vertexCount();
    _indexData = std::move(mesh.indexData());
    _indexCount = mesh.indexCount();
    _axisAlignedBox = mesh.axisAlignedBox();

    return *this;
}