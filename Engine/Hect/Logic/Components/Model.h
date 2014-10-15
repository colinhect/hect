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

#include "Hect/Logic/ComponentRegistry.h"
#include "Hect/Graphics/Material.h"
#include "Hect/Graphics/Mesh.h"

namespace hect
{

///
/// A single surface with a specific material in a model component.
class ModelSurface
{
public:
    ModelSurface();

    ///
    /// Constructs the surface given the mesh and the material.
    ModelSurface(const AssetHandle<Mesh>& mesh, const AssetHandle<Material>& material);

    ///
    /// The mesh.
    AssetHandle<Mesh> mesh;

    ///
    /// The material.
    AssetHandle<Material> material;
};

///
/// A collection of surfaces which are rendered.
///
/// \component
class Model :
    public Component<Model>
{
public:

    ///
    /// The surfaces.
    std::vector<ModelSurface> surfaces;

    void encode(Encoder& encoder) const;
    void decode(Decoder& decoder);
};

}