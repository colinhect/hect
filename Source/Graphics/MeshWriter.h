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
/// Provides an interface for writing to a mesh without knowledge of the
/// vertex layout.
class MeshWriter
{
public:

    ///
    /// Constructs a mesh writer given a mesh to build on.
    ///
    /// \param mesh The mesh to write to.
    MeshWriter(Mesh& mesh);

    ///
    /// Adds a new vertex to the mesh.
    ///
    /// \returns The index of the added vertex.
    size_t addVertex();

    ///
    /// Writes attribute data of the most recently added vertex for a given
    /// semantic.
    ///
    /// \param semantic The semantic of the attribute to set the data for.
    /// \param value The value.
    void writeAttributeData(VertexAttributeSemantic semantic, float value);

    ///
    /// Writes attribute data of the most recently added vertex for a given
    /// semantic.
    ///
    /// \param semantic The semantic of the attribute to set the data for.
    /// \param value The value.
    void writeAttributeData(VertexAttributeSemantic semantic, const Vector2<float>& value);

    ///
    /// Writes attribute data of the most recently added vertex for a given
    /// semantic.
    ///
    /// \param semantic The semantic of the attribute to set the data for.
    /// \param value The value.
    void writeAttributeData(VertexAttributeSemantic semantic, const Vector3<float>& value);

    ///
    /// Writes attribute data of the most recently added vertex for a given
    /// semantic.
    ///
    /// \param semantic The semantic of the attribute to set the data for.
    /// \param value The value.
    void writeAttributeData(VertexAttributeSemantic semantic, const Vector4<float>& value);

    ///
    /// Adds an index to the mesh.
    void addIndex(uint64_t value);

private:
    void _setComponentValue(const VertexAttribute* attribute, unsigned index, float value);

    template <typename T>
    void _writeAttributeData(const VertexAttribute& attribute, const T& value);

    Mesh* _mesh;
    size_t _vertexDataIndex;
};

}

#include "MeshWriter.inl"
