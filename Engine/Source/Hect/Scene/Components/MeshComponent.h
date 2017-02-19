///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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

#include "Hect/Core/Export.h"
#include "Hect/Graphics/Material.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/IO/Encodable.h"
#include "Hect/Scene/Scene.h"

namespace hect
{

///
/// A Mesh surface with a specific Material in a MeshComponent.
///
/// \encodable
class HECT_EXPORT MeshSurface :
    public Encodable
{
public:
    MeshSurface();

    ///
    /// Constructs a surface.
    ///
    /// \param mesh The mesh.
    /// \param material The material (optional).
    MeshSurface(const Mesh::Handle& mesh, const Material::Handle& material = Material::Handle());

    ///
    /// The mesh.
    ///
    /// \property
    Mesh::Handle mesh;

    ///
    /// The material.
    ///
    /// \property
    Material::Handle material;

    ///
    /// Whether the surface is visible.
    ///
    /// \property
    bool visible { true };
};

///
/// A collection of MeshSurface%s which are rendered.
///
/// \component
class HECT_EXPORT MeshComponent :
    public Component<MeshComponent>
{
public:

    ///
    /// Adds a mesh surface.
    ///
    /// \param mesh The mesh.
    /// \param material The material (optional).
    void addSurface(const Mesh::Handle& mesh, const Material::Handle& material = Material::Handle());

    ///
    /// Adds a mesh surface.
    ///
    /// \param surface The surface to add.
    void addSurface(const MeshSurface& surface);

    ///
    /// The surfaces.
    ///
    /// \property{vector}
    std::vector<MeshSurface> surfaces;

    ///
    /// Whether the surfaces are visible.
    ///
    /// \property
    bool visible { true };
};

}
