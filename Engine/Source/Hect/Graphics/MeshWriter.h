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
#include "Hect/Graphics/Mesh.h"
#include "Hect/IO/MemoryWriteStream.h"
#include "Hect/Math/Vector2.h"
#include "Hect/Math/Vector3.h"
#include "Hect/Math/Vector4.h"

namespace hect
{

///
/// Provides an interface for writing to a mesh without knowledge of the
/// vertex layout.
class HECT_EXPORT MeshWriter
{
public:

    ///
    /// Constructs a mesh writer.
    ///
    /// \param mesh The mesh to write to.
    MeshWriter(Mesh& mesh);

    ///
    /// Adds a new vertex to the mesh.
    ///
    /// \returns The index of the added vertex.
    size_t add_vertex();

    ///
    /// Writes attribute data of the most recently added vertex for a given
    /// semantic.
    ///
    /// \param semantic The semantic of the attribute to set the data for.
    /// \param value The value.
    void write_attribute_data(VertexAttributeSemantic semantic, double value);

    ///
    /// \copydoc hect::MeshWriter::write_attribute_data()
    void write_attribute_data(VertexAttributeSemantic semantic, Vector2 value);

    ///
    /// \copydoc hect::MeshWriter::write_attribute_data()
    void write_attribute_data(VertexAttributeSemantic semantic, Vector3 value);

    ///
    /// \copydoc hect::MeshWriter::write_attribute_data()
    void write_attribute_data(VertexAttributeSemantic semantic, Vector4 value);

    ///
    /// \copydoc hect::MeshWriter::write_attribute_data()
    void write_attribute_data(VertexAttributeSemantic semantic, Color value);

    ///
    /// Adds an index to the mesh.
    void add_index(uint64_t value);

private:
    void set_component_value(const VertexAttribute& attribute, unsigned index, float value);

    Mesh& _mesh;
    size_t _vertex_position { 0 };
    MemoryWriteStream _vertex_stream;
    MemoryWriteStream _index_stream;
};

}