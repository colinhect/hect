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
#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Logic/System.h"
#include "Hect/Logic/Systems/TransformSystem.h"
#include "Hect/Logic/Components/RigidBodyComponent.h"
#include "Hect/Timing/TimeSpan.h"

// Forward declare Bullet classes
class btCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btConstraintSolver;
class btDynamicsWorld;
class btTriangleMesh;

namespace hect
{

///
/// Simulates physical interactions of physical bodies.
///
/// \system
class HECT_EXPORT PhysicsSystem :
    public System<PhysicsSystem, Components<RigidBodyComponent>>
{
public:
    PhysicsSystem(Engine& engine, Scene& scene);

    ///
    /// Applies a force to a rigid body.
    ///
    /// \param rigidBody The rigid body to apply the force to.
    /// \param force The force to apply.
    /// \param relativePosition The position relative to the rigid body to
    /// apply the force from.
    void applyForce(RigidBodyComponent& rigidBody, Vector3 force, Vector3 relativePosition);

    ///
    /// Commits any changes made to a RigidBodyComponent to take effect.
    ///
    /// \param rigidBody The rigid body to commit.
    void commit(RigidBodyComponent& rigidBody);

    void tick(double timeStep) override;
    void onComponentAdded(RigidBodyComponent::Iterator rigidBody) override;
    void onComponentRemoved(RigidBodyComponent::Iterator rigidBody) override;

    ///
    /// The gravitational force to apply to all rigid bodies.
    ///
    /// \property
    Vector3 gravity;

private:
    TransformSystem::Handle _transformSystem;

    btTriangleMesh* toBulletMesh(Mesh* mesh);

    std::shared_ptr<btCollisionConfiguration> _configuration;
    std::shared_ptr<btCollisionDispatcher> _dispatcher;
    std::shared_ptr<btBroadphaseInterface> _broadphase;
    std::shared_ptr<btConstraintSolver> _solver;
    std::shared_ptr<btDynamicsWorld> _world;

    std::map<Mesh*, std::shared_ptr<btTriangleMesh>> _bulletMeshes;
};

}