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
#include "Hect/IO/MemoryReadStream.h"
#include "Hect/Math/Vector2.h"
#include "Hect/Math/Vector3.h"
#include "Hect/Math/Vector4.h"

namespace hect
{

///
/// Provides an interface for reading from a mesh without knowledge of the
/// vertex layout.
class HECT_EXPORT MeshReader
{
public:

    ///
    /// Constructs a mesh.
    ///
    /// \param mesh The mesh to read from.
    MeshReader(const Mesh& mesh);

    ///
    /// Moves to the next vertex in the mesh.
    ///
    /// \returns Whether there is a vertex left to read.
    bool nextVertex();

    ///
    /// Reads an the attribute with the given semantic as a double.
    ///
    /// \throws InvalidOperation If the first vertex has not been moved to or
    /// attempting to read past the last vertex.
    double readAttributeDouble(VertexAttributeSemantic semantic) const;

    ///
    /// Reads an the attribute with the given semantic as a 2-dimensional
    /// vector.
    ///
    /// \throws InvalidOperation If the first vertex has not been moved to or
    /// attempting to read past the last vertex.
    Vector2 readAttributeVector2(VertexAttributeSemantic semantic) const;

    ///
    /// Reads an the attribute with the given semantic as a 3-dimensional
    /// vector.
    ///
    /// \throws InvalidOperation If the first vertex has not been moved to or
    /// attempting to read past the last vertex.
    Vector3 readAttributeVector3(VertexAttributeSemantic semantic) const;

    ///
    /// Reads an the attribute with the given semantic as a 4-dimensional
    /// vector.
    ///
    /// \throws InvalidOperation If the first vertex has not been moved to or
    /// attempting to read past the last vertex.
    Vector4 readAttributeVector4(VertexAttributeSemantic semantic) const;

    ///
    /// Reads an the attribute with the given semantic as a color.
    ///
    /// \throws InvalidOperation If the first vertex has not been moved to or
    /// attempting to read past the last vertex.
    Color readAttributeColor(VertexAttributeSemantic semantic) const;

    ///
    /// Moves to the next index in the mesh.
    ///
    /// \returns Whether there is an index left to read.
    bool nextIndex();

    ///
    /// Reads the current index as an unsigned byte.
    ///
    /// \throws InvalidOperation If the first index has not been moved to or
    /// attempting to read past the last index.
    uint8_t readIndexUInt8() const;

    ///
    /// Reads the current index as an unsigned short.
    ///
    /// \throws InvalidOperation If the first index has not been moved to or
    /// attempting to read past the last index.
    uint16_t readIndexUInt16() const;

    ///
    /// Reads the current index as an unsigned int.
    ///
    /// \throws InvalidOperation If the first index has not been moved to or
    /// attempting to read past the last index.
    uint32_t readIndexUInt32() const;

private:
    void checkVertexBoundary() const;
    void checkIndexBoundary() const;

    float readComponentValue(const VertexAttribute& attribute, unsigned index) const;

    const Mesh& _mesh;

    size_t _vertexCount { 0 };
    size_t _vertexPosition { 0 };
    mutable MemoryReadStream _vertexStream;

    size_t _indexCount { 0 };
    size_t _indexPosition { 0 };
    mutable MemoryReadStream _indexStream;
};

}