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
#include "Geometry.h"

using namespace hect;

void Geometry::addSurface(const AssetHandle<Mesh>& mesh, const AssetHandle<Material>& material)
{
    _meshes.push_back(mesh);
    _materials.push_back(material);
}

size_t Geometry::surfaceCount() const
{
    return _meshes.size();
}

AssetHandle<Mesh>::Array& Geometry::meshes()
{
    return _meshes;
}

const AssetHandle<Mesh>::Array& Geometry::meshes() const
{
    return _meshes;
}

AssetHandle<Material>::Array& Geometry::materials()
{
    return _materials;
}

const AssetHandle<Material>::Array& Geometry::materials() const
{
    return _materials;
}

void Geometry::save(ObjectWriter& writer) const
{
    size_t surfaceCount = _meshes.size();

    ArrayWriter surfaces = writer.writeArray("surfaces");
    for (size_t i = 0; i < surfaceCount; ++i)
    {
        ObjectWriter surface = surfaces.writeObject();
        surface.writeString("mesh", _meshes[i].path().toString());
        surface.writeString("material", _materials[i].path().toString());
    }
}

void Geometry::load(ObjectReader& reader, AssetCache& assetCache)
{
    if (reader.hasMember("surfaces"))
    {
        ArrayReader surfaces = reader.readArray("surfaces");
        while (surfaces.hasMoreElements())
        {
            ObjectReader surface = surfaces.readObject();
            if (surface.hasMember("mesh") && surface.hasMember("material"))
            {
                std::string meshPath = surface.readString("mesh");
                std::string materialPath = surface.readString("material");

                AssetHandle<Mesh> mesh = assetCache.getHandle<Mesh>(meshPath);
                AssetHandle<Material> material = assetCache.getHandle<Material>(materialPath);

                addSurface(mesh, material);
            }
        }
    }
}