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

#include <vector>

#include "Hect/Concurrency/Task.h"
#include "Hect/Core/Export.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Scene/System.h"
#include "Hect/Scene/Systems/TransformSystem.h"
#include "Hect/Scene/Components/RigidBodyComponent.h"

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
    ~PhysicsSystem();

    ///
    /// Applies a force to a rigid body.
    ///
    /// \param rigidBody The rigid body to apply the force to.
    /// \param force The force to apply.
    /// \param relativePosition The position relative to the rigid body to
    /// apply the force from.
    void applyForceToRigidBody(RigidBodyComponent& rigidBody, Vector3 force, Vector3 relativePosition);

    ///
    /// Commits any changes made to a RigidBodyComponent to take effect.
    ///
    /// \param rigidBody The rigid body to commit.
    void commitRigidBody(RigidBodyComponent& rigidBody);

    ///
    /// Begins the physics simulation task.
    ///
    /// \param timeStep The duration of time in seconds to simulate.
    void beginSimulationTask(double timeStep);

    ///
    /// Waits for the physics simulation task to complete if it has begin.
    void waitForSimulationTask();

    ///
    /// Syncs the transforms of all entities with physical body components with
    /// the physics simulation.
    void syncWithSimulation();

    ///
    /// The gravitational force to apply to all rigid bodies.
    ///
    /// \property
    Vector3 gravity;

private:
    // A deferred application of force to a Bullet rigid body
    class ForceApplication
    {
    public:
        ForceApplication(btRigidBody* rigidBody, Vector3 force, Vector3 relativePosition);

        btRigidBody* rigidBody;
        Vector3 force;
        Vector3 relativePosition;
    };

    // System overrides
    void onComponentAdded(RigidBodyComponent::Iterator rigidBody) override;
    void onComponentRemoved(RigidBodyComponent::Iterator rigidBody) override;

    std::vector<ComponentId> _committedRigidBodyIds;
    std::vector<btRigidBody*> _addedRigidBodies;
    std::vector<btRigidBody*> _removedRigidBodies;
    std::vector<ForceApplication> _forceApplications;

    Task::Handle _physicsSimulationTask;

    // Convert a mesh to a Bullet mesh object
    btTriangleMesh* toBulletMesh(Mesh* mesh);

    // Bullet related
    std::shared_ptr<btCollisionConfiguration> _configuration;
    std::shared_ptr<btCollisionDispatcher> _dispatcher;
    std::shared_ptr<btBroadphaseInterface> _broadphase;
    std::shared_ptr<btConstraintSolver> _solver;
    std::shared_ptr<btDynamicsWorld> _world;
    std::map<Mesh*, std::shared_ptr<btTriangleMesh>> _bulletMeshes;
};

}