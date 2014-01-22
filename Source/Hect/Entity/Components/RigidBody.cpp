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
#include "RigidBody.h"

using namespace hect;

#include "Hect/Internal/Bullet.h"

RigidBody::RigidBody() :
    _mass(0)
{
}

Real RigidBody::mass() const
{
    return _mass;
}

void RigidBody::setMass(Real mass)
{
    if (_rigidBody)
    {
        throw Error("Cannot set mass after the rigid body is created");
    }
    _mass = mass;
}

const Vector3& RigidBody::linearVelocity() const
{
    // Update the current value from the Bullet rigid body
    if (_rigidBody)
    {
        _linearVelocity = convertFromBullet(_rigidBody->getLinearVelocity());
    }

    return _linearVelocity;
}

void RigidBody::setLinearVelocity(const Vector3& linearVelocity)
{
    // Set the new value in the bullet rigid body as well
    if (_rigidBody)
    {
        _rigidBody->setLinearVelocity(convertToBullet(linearVelocity));
    }

    _linearVelocity = linearVelocity;
}

const Vector3& RigidBody::angularVelocity() const
{
    // Update the current value from the Bullet rigid body
    if (_rigidBody)
    {
        _angularVelocity = convertFromBullet(_rigidBody->getAngularVelocity());
    }

    return _angularVelocity;
}

void RigidBody::setAngularVelocity(const Vector3& angularVelocity)
{
    // Set the new value in the bullet rigid body as well
    if (_rigidBody)
    {
        _rigidBody->setAngularFactor(convertToBullet(angularVelocity));
    }

    _angularVelocity = angularVelocity;
}

AssetHandle<Mesh>& RigidBody::mesh()
{
    return _mesh;
}

const AssetHandle<Mesh>& RigidBody::mesh() const
{
    return _mesh;
}

void RigidBody::setMesh(const AssetHandle<Mesh>& mesh)
{
    if (_rigidBody)
    {
        throw Error("Cannot set mesh after the rigid body is created");
    }
    _mesh = mesh;
}

void RigidBody::save(ObjectWriter& writer) const
{
    writer.writeReal("mass", _mass);
    writer.writeVector3("linearVelocity", _linearVelocity);
    writer.writeVector3("angularVelocity", _angularVelocity);
    writer.writeString("mesh", _mesh.path().toString());
}

void RigidBody::load(ObjectReader& reader, AssetCache& assetCache)
{
    if (reader.hasMember("mass"))
    {
        setMass(reader.readReal("mass"));
    }

    if (reader.hasMember("linearVelocity"))
    {
        setLinearVelocity(reader.readVector3("linearVelocity"));
    }

    if (reader.hasMember("angularVelocity"))
    {
        setAngularVelocity(reader.readVector3("angularVelocity"));
    }

    if (reader.hasMember("mesh"))
    {
        std::string meshPath = reader.readString("mesh");
        AssetHandle<Mesh> mesh = assetCache.getHandle<Mesh>(meshPath);

        setMesh(mesh);
    }
}