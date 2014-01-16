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

// Forward declare Bullet classes
class btRigidBody;
class btMotionState;
class btCollisionShape;

namespace hect
{

///
/// A simulated physical body.
class RigidBody :
    public Component<RigidBody>
{
    friend class PhysicsSystem;
public:
    RigidBody();

    ///
    /// Returns the mass.
    double mass() const;

    ///
    /// Sets the mass.
    ///
    /// \param mass The new mass.
    void setMass(double mass);

    ///
    /// Returns the linear velocity.
    const Vector3<>& linearVelocity() const;

    ///
    /// Sets the linear velocity.
    ///
    /// \param linearVelocity The new linear velocity.
    void setLinearVelocity(const Vector3<>& linearVelocity);

    ///
    /// Returns the angular velocity.
    const Vector3<>& angularVelocity() const;

    ///
    /// Sets the angular velocity.
    ///
    /// \param angularVelocity The new angular velocity.
    void setAngularVelocity(const Vector3<>& angularVelocity);

    ///
    /// Returns the mesh.
    AssetHandle<Mesh>& mesh();

    ///
    /// Returns the mesh.
    const AssetHandle<Mesh>& mesh() const;

    ///
    /// Sets the mesh.
    ///
    /// \param mesh The new mesh.
    void setMesh(const AssetHandle<Mesh>& mesh);

private:
    double _mass;
    mutable Vector3<> _linearVelocity;
    mutable Vector3<> _angularVelocity;
    AssetHandle<Mesh> _mesh;
    
    std::shared_ptr<btRigidBody> _rigidBody;
    std::shared_ptr<btMotionState> _motionState;
    std::shared_ptr<btCollisionShape> _collisionShape;
};

///
/// Serializer for RigidBody.
class RigidBodySerializer :
    public ComponentSerializer<RigidBody>
{
public:

    ///
    /// See BaseComponentSerializer::save()
    void save(const RigidBody& rigidBody, DataWriter& writer) const;

    ///
    /// See BaseComponentSerializer::load()
    void load(RigidBody& rigidBody, DataReader& reader, AssetCache& assetCache) const;
};

}