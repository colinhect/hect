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
#pragma once

namespace hect
{

///
/// A geometry component.
class Geometry :
    public Component<Geometry>
{
public:

    ///
    /// Adds a surface.
    ///
    /// \param mesh The mesh.
    /// \param material The material.
    void addSurface(const AssetHandle<Mesh>& mesh, const AssetHandle<Material>& material);

    ///
    /// Returns the number of surfaces.
    size_t surfaceCount() const;

    ///
    /// Returns the meshes.
    AssetHandle<Mesh>::Array& meshes();

    ///
    /// Returns the meshes.
    const AssetHandle<Mesh>::Array& meshes() const;

    ///
    /// Returns the materials
    AssetHandle<Material>::Array& materials();

    ///
    /// Returns the materials
    const AssetHandle<Material>::Array& materials() const;

private:
    AssetHandle<Mesh>::Array _meshes;
    AssetHandle<Material>::Array _materials;
};

///
/// Serializer for Geometry.
class GeometrySerializer :
    public ComponentSerializer<Geometry>
{
public:

    ///
    /// See BaseComponentSerializer::save()
    void save(const Geometry& geometry, DataWriter& writer) const;

    ///
    /// See BaseComponentSerializer::load()
    void load(Geometry& geometry, DataReader& reader, AssetCache& assetCache) const;
};

}