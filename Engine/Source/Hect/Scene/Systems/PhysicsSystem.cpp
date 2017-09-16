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
#include "PhysicsSystem.h"

#include "Hect/Graphics/MeshReader.h"
#include "Hect/Runtime/Engine.h"
#include "Hect/Scene/Components/RigidBodyComponent.h"
#include "Hect/Scene/Components/TransformComponent.h"

#ifdef HECT_WINDOWS_BUILD
#pragma warning(push, 0)
#endif

#define BT_USE_DOUBLE_PRECISION

#include <btBulletDynamicsCommon.h>

#ifdef HECT_WINDOWS_BUILD
#pragma warning(pop)
#endif

using namespace hect;

namespace
{

btVector3 convert_to_bullet(Vector3 v)
{
    return btVector3(v.x, v.y, v.z);
}

btQuaternion convert_to_bullet(Quaternion q)
{
    return btQuaternion(q.x, q.y, q.z, -q.w);
}

btTransform convert_to_bullet(const TransformComponent& t)
{
    btTransform transform;
    transform.setOrigin(convert_to_bullet(t.global_position));
    transform.setRotation(convert_to_bullet(t.global_rotation));
    return transform;
}

btTriangleMesh* convert_to_bullet(const Mesh& m)
{
    MeshReader mesh_reader(m);

    std::vector<btVector3> vertices;
    while (mesh_reader.next_vertex())
    {
        Vector3 position = mesh_reader.read_attribute_vector3(VertexAttributeSemantic::Position);
        vertices.push_back(convert_to_bullet(position));
    }

    std::vector<size_t> indices;
    while (mesh_reader.next_index())
    {
        indices.push_back(mesh_reader.read_index_u_int32());
    }

    btTriangleMesh* mesh = new btTriangleMesh();
    for (size_t i = 0; i < indices.size() - 2; i += 3)
    {
        mesh->addTriangle(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
    }

    return mesh;
}

Vector3 convert_from_bullet(const btVector3& v)
{
    return Vector3(v.x(), v.y(), v.z());
}

Quaternion convert_from_bullet(const btQuaternion& q)
{
    return Quaternion(q.x(), q.y(), q.z(), -q.w());
}

TransformComponent convert_from_bullet(const btTransform& t)
{
    TransformComponent transform;
    transform.local_position = convert_from_bullet(t.getOrigin());
    transform.local_rotation = convert_from_bullet(t.getRotation());
    return transform;
}

}

PhysicsSystem::PhysicsSystem(Scene& scene, TransformSystem& transform_system) :
    System(scene),
    gravity(Vector3(0, 0, -9.8)),
    _transform_system(transform_system),
    _configuration(new btDefaultCollisionConfiguration()),
    _dispatcher(new btCollisionDispatcher(_configuration.get())),
    _broadphase(new btDbvtBroadphase()),
    _solver(new btSequentialImpulseConstraintSolver()),
    _world(new btDiscreteDynamicsWorld(_dispatcher.get(), _broadphase.get(), _solver.get(), _configuration.get()))
{
}

PhysicsSystem::~PhysicsSystem()
{
    wait_for_simulation_task();
}

void PhysicsSystem::apply_force_to_rigid_body(RigidBodyComponent& rigid_body, Vector3 force, Vector3 relative_position)
{
    btRigidBody* bullet_rigid_body = rigid_body._rigid_body.get();
    _force_applications.emplace_back(bullet_rigid_body, force, relative_position);
}

void PhysicsSystem::commit_rigid_body(RigidBodyComponent& rigid_body)
{
    const ComponentId id = rigid_body.id();
    _committed_rigid_body_ids.push_back(id);
}

void PhysicsSystem::begin_simulation_task(TaskPool& task_pool, Seconds time_step)
{
    _physics_simulation_task = task_pool.enqueue([this, time_step]
    {
        _world->stepSimulation(time_step.value, 4);
    });
}

void PhysicsSystem::wait_for_simulation_task()
{
    if (_physics_simulation_task)
    {
        _physics_simulation_task->wait();
    }
}

void PhysicsSystem::sync_with_simulation()
{
    ComponentPool<RigidBodyComponent>& rigid_body_components = scene().components<RigidBodyComponent>();
    for (ComponentId id : _committed_rigid_body_ids)
    {
        RigidBodyComponent& rigid_body = rigid_body_components.with_id(id);
        rigid_body._rigid_body->setLinearVelocity(convert_to_bullet(rigid_body.linear_velocity));
        rigid_body._rigid_body->setAngularVelocity(convert_to_bullet(rigid_body.angular_velocity));
    }
    _committed_rigid_body_ids.clear();

    // Add new rigid bodies to the Bullet world
    for (btRigidBody* rigid_body : _added_rigid_bodies)
    {
        _world->addRigidBody(rigid_body);
    }
    _added_rigid_bodies.clear();

    // Apply forces to rigid bodies
    for (ForceApplication& force_application : _force_applications)
    {
        btRigidBody* rigid_body = force_application.rigid_body;
        if (rigid_body)
        {
            const btVector3 force = convert_to_bullet(force_application.force);
            const btVector3 relative_position = convert_to_bullet(force_application.relative_position);
            rigid_body->applyForce(force, relative_position);
        }
    }
    _force_applications.clear();

    // Remove old rigid bodies from the Bullet world
    for (btRigidBody* rigid_body : _removed_rigid_bodies)
    {
        _world->removeRigidBody(rigid_body);
    }
    _removed_rigid_bodies.clear();

    // Update gravity if needed
    Vector3 bullet_gravity = convert_from_bullet(_world->getGravity());
    if (gravity != bullet_gravity)
    {
        _world->setGravity(convert_to_bullet(gravity));
    }

    // For each rigid body component
    for (RigidBodyComponent& rigid_body : scene().components<RigidBodyComponent>())
    {
        Entity& entity = rigid_body.entity();
        if (!entity.parent() && entity.has_component<TransformComponent>())
        {
            auto& transform = entity.component<TransformComponent>();

            // Update the transform to what Bullet says it should be
            btTransform bullet_transform;
            static_cast<btDefaultMotionState*>(rigid_body._rigid_body->getMotionState())->getWorldTransform(bullet_transform);

            TransformComponent new_transform = convert_from_bullet(bullet_transform);
            transform.local_position = new_transform.local_position;
            transform.local_scale = new_transform.local_scale;
            transform.local_rotation = new_transform.local_rotation;
            _transform_system.commit_transform(transform);

            // Update rigid body properties to what Bullet says it should be
            rigid_body.linear_velocity = convert_from_bullet(rigid_body._rigid_body->getLinearVelocity());
            rigid_body.angular_velocity = convert_from_bullet(rigid_body._rigid_body->getAngularVelocity());
        }
    }
}

void PhysicsSystem::on_component_added(RigidBodyComponent& rigid_body)
{
    Entity& entity = rigid_body.entity();

    auto& transform = entity.component<TransformComponent>();

    Mesh& mesh = *rigid_body.mesh;
    rigid_body._collision_shape.reset(new btConvexTriangleMeshShape(to_bullet_mesh(&mesh)));

    btScalar mass = rigid_body.mass;
    btVector3 local_inertia(0, 0, 0);
    if (mass != 0.0)
    {
        rigid_body._collision_shape->calculateLocalInertia(mass, local_inertia);
    }

    btVector3 linear_velocity = convert_to_bullet(rigid_body.linear_velocity);
    btVector3 angular_velocity = convert_to_bullet(rigid_body.angular_velocity);

    _transform_system.update_transform(transform);

    rigid_body._motion_state.reset(new btDefaultMotionState(convert_to_bullet(transform)));
    btRigidBody::btRigidBodyConstructionInfo info(mass, rigid_body._motion_state.get(), rigid_body._collision_shape.get(), local_inertia);
    rigid_body._rigid_body.reset(new btRigidBody(info));
    rigid_body._rigid_body->setSleepingThresholds(0, 0);
    rigid_body._rigid_body->setLinearVelocity(linear_velocity);
    rigid_body._rigid_body->setAngularVelocity(angular_velocity);
    rigid_body._rigid_body->setAngularFactor(0.5);

    _added_rigid_bodies.push_back(rigid_body._rigid_body.get());
}

void PhysicsSystem::on_component_removed(RigidBodyComponent& rigid_body)
{
    // Remove the transform from the committed transform vector
    const ComponentId id = rigid_body.id();
    _committed_rigid_body_ids.erase(std::remove(_committed_rigid_body_ids.begin(), _committed_rigid_body_ids.end(), id), _committed_rigid_body_ids.end());

    _removed_rigid_bodies.push_back(rigid_body._rigid_body.get());
}

btTriangleMesh* PhysicsSystem::to_bullet_mesh(Mesh* mesh)
{
    auto it = _bullet_meshes.find(mesh);
    if (it != _bullet_meshes.end())
    {
        // The Bullet mesh was already created
        return (*it).second.get();
    }
    else
    {
        // Create a Bullet mesh from the mesh and keep it to be looked up later
        btTriangleMesh* bullet_mesh = convert_to_bullet(*mesh);
        _bullet_meshes[mesh] = std::unique_ptr<btTriangleMesh>(bullet_mesh);
        return bullet_mesh;
    }
}

PhysicsSystem::ForceApplication::ForceApplication(btRigidBody* rigid_body, Vector3 force, Vector3 relative_position) :
    rigid_body(rigid_body),
    force(force),
    relative_position(relative_position)
{
}
