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
    PhysicsSystem(Scene& scene, TransformSystem& transform_system);
    ~PhysicsSystem();

    ///
    /// Applies a force to a rigid body.
    ///
    /// \param rigid_body The rigid body to apply the force to.
    /// \param force The force to apply.
    /// \param relative_position The position relative to the rigid body to
    /// apply the force from.
    void apply_force_to_rigid_body(RigidBodyComponent& rigid_body, Vector3 force, Vector3 relative_position);

    ///
    /// Commits any changes made to a RigidBodyComponent to take effect.
    ///
    /// \param rigid_body The rigid body to commit.
    void commit_rigid_body(RigidBodyComponent& rigid_body);

    ///
    /// Begins the physics simulation task.
    ///
    /// \param task_pool The task pool.
    /// \param time_step The duration of time to simulate.
    void begin_simulation_task(TaskPool& task_pool, Seconds time_step);

    ///
    /// Waits for the physics simulation task to complete if it has begin.
    void wait_for_simulation_task();

    ///
    /// Syncs the transforms of all entities with physical body components with
    /// the physics simulation.
    void sync_with_simulation();

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
        ForceApplication(btRigidBody* rigid_body, Vector3 force, Vector3 relative_position);

        btRigidBody* rigid_body;
        Vector3 force;
        Vector3 relative_position;
    };

    // System overrides
    void on_component_added(RigidBodyComponent& rigid_body) override;
    void on_component_removed(RigidBodyComponent& rigid_body) override;

    TransformSystem& _transform_system;

    std::vector<ComponentId> _committed_rigid_body_ids;
    std::vector<btRigidBody*> _added_rigid_bodies;
    std::vector<btRigidBody*> _removed_rigid_bodies;
    std::vector<ForceApplication> _force_applications;

    Task::Handle _physics_simulation_task;

    // Convert a mesh to a Bullet mesh object
    btTriangleMesh* to_bullet_mesh(Mesh* mesh);

    // Bullet related
    std::shared_ptr<btCollisionConfiguration> _configuration;
    std::shared_ptr<btCollisionDispatcher> _dispatcher;
    std::shared_ptr<btBroadphaseInterface> _broadphase;
    std::shared_ptr<btConstraintSolver> _solver;
    std::shared_ptr<btDynamicsWorld> _world;
    std::map<Mesh*, std::shared_ptr<btTriangleMesh>> _bullet_meshes;
};

}
