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
#include "Bullet.h"

namespace hect
{

btVector3 convertToBullet(const Vector3<>& v)
{
    return btVector3(v.x, v.y, v.z);
}

btQuaternion convertToBullet(const Quaternion<>& q)
{
    return btQuaternion(q.x, q.y, q.z, -q.w);
}

btTransform convertToBullet(const Transform& t)
{
    btTransform transform;
    transform.setOrigin(convertToBullet(t.position()));
    transform.setRotation(convertToBullet(t.rotation()));
    return transform;
}

btTriangleMesh* convertToBullet(const Mesh& m)
{
    MeshReader meshReader(m);

    std::vector<btVector3> vertices;
    while (meshReader.nextVertex())
    {
        Vector3<float> position = meshReader.readAttributeVector3(VertexAttributeSemantic::Position);
        vertices.push_back(convertToBullet(position));
    }

    std::vector<size_t> indices;
    while (meshReader.nextIndex())
    {
        indices.push_back(meshReader.readIndexInt());
    }

    btTriangleMesh* mesh = new btTriangleMesh();
    for (size_t i = 0; i < indices.size() - 2; i += 3)
    {
        mesh->addTriangle(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
    }

    return mesh;
}

Vector3<> convertFromBullet(const btVector3& v)
{
    return Vector3<>(v.x(), v.y(), v.z());
}

Quaternion<> convertFromBullet(const btQuaternion& q)
{
    return Quaternion<>(q.x(), q.y(), q.z(), -q.w());
}

Transform convertFromBullet(const btTransform& t)
{
    Transform transform;
    transform.setPosition(convertFromBullet(t.getOrigin()));
    transform.setRotation(convertFromBullet(t.getRotation()));
    return transform;
}

}