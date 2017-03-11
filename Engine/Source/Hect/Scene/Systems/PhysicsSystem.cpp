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

btVector3 convertToBullet(Vector3 v)
{
    return btVector3(v.x, v.y, v.z);
}

btQuaternion convertToBullet(Quaternion q)
{
    return btQuaternion(q.x, q.y, q.z, -q.w);
}

btTransform convertToBullet(const TransformComponent& t)
{
    btTransform transform;
    transform.setOrigin(convertToBullet(t.globalPosition));
    transform.setRotation(convertToBullet(t.globalRotation));
    return transform;
}

btTriangleMesh* convertToBullet(const Mesh& m)
{
    MeshReader meshReader(m);

    std::vector<btVector3> vertices;
    while (meshReader.nextVertex())
    {
        Vector3 position = meshReader.readAttributeVector3(VertexAttributeSemantic::Position);
        vertices.push_back(convertToBullet(position));
    }

    std::vector<size_t> indices;
    while (meshReader.nextIndex())
    {
        indices.push_back(meshReader.readIndexUInt32());
    }

    btTriangleMesh* mesh = new btTriangleMesh();
    for (size_t i = 0; i < indices.size() - 2; i += 3)
    {
        mesh->addTriangle(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
    }

    return mesh;
}

Vector3 convertFromBullet(const btVector3& v)
{
    return Vector3(v.x(), v.y(), v.z());
}

Quaternion convertFromBullet(const btQuaternion& q)
{
    return Quaternion(q.x(), q.y(), q.z(), -q.w());
}

TransformComponent convertFromBullet(const btTransform& t)
{
    TransformComponent transform;
    transform.localPosition = convertFromBullet(t.getOrigin());
    transform.localRotation = convertFromBullet(t.getRotation());
    return transform;
}

}

PhysicsSystem::PhysicsSystem(Scene& scene, TransformSystem& transformSystem) :
    System(scene),
    gravity(Vector3(0, 0, -9.8)),
    _transformSystem(transformSystem),
    _configuration(new btDefaultCollisionConfiguration()),
    _dispatcher(new btCollisionDispatcher(_configuration.get())),
    _broadphase(new btDbvtBroadphase()),
    _solver(new btSequentialImpulseConstraintSolver()),
    _world(new btDiscreteDynamicsWorld(_dispatcher.get(), _broadphase.get(), _solver.get(), _configuration.get()))
{
}

PhysicsSystem::~PhysicsSystem()
{
    waitForSimulationTask();
}

void PhysicsSystem::applyForceToRigidBody(RigidBodyComponent& rigidBody, Vector3 force, Vector3 relativePosition)
{
    btRigidBody* bulletRigidBody = rigidBody._rigidBody.get();
    _forceApplications.emplace_back(bulletRigidBody, force, relativePosition);
}

void PhysicsSystem::commitRigidBody(RigidBodyComponent& rigidBody)
{
    const ComponentId id = rigidBody.id();
    _committedRigidBodyIds.push_back(id);
}

void PhysicsSystem::beginSimulationTask(TaskPool& taskPool, Seconds timeStep)
{
    _physicsSimulationTask = taskPool.enqueue([this, timeStep]
    {
        _world->stepSimulation(timeStep.value, 4);
    });
}

void PhysicsSystem::waitForSimulationTask()
{
    if (_physicsSimulationTask)
    {
        _physicsSimulationTask->wait();
    }
}

void PhysicsSystem::syncWithSimulation()
{
    ComponentPool<RigidBodyComponent>& rigidBodyComponents = scene().components<RigidBodyComponent>();
    for (ComponentId id : _committedRigidBodyIds)
    {
        RigidBodyComponent& rigidBody = rigidBodyComponents.withId(id);
        rigidBody._rigidBody->setLinearVelocity(convertToBullet(rigidBody.linearVelocity));
        rigidBody._rigidBody->setAngularVelocity(convertToBullet(rigidBody.angularVelocity));
    }
    _committedRigidBodyIds.clear();

    // Add new rigid bodies to the Bullet world
    for (btRigidBody* rigidBody : _addedRigidBodies)
    {
        _world->addRigidBody(rigidBody);
    }
    _addedRigidBodies.clear();

    // Apply forces to rigid bodies
    for (ForceApplication& forceApplication : _forceApplications)
    {
        btRigidBody* rigidBody = forceApplication.rigidBody;
        if (rigidBody)
        {
            const btVector3 force = convertToBullet(forceApplication.force);
            const btVector3 relativePosition = convertToBullet(forceApplication.relativePosition);
            rigidBody->applyForce(force, relativePosition);
        }
    }
    _forceApplications.clear();

    // Remove old rigid bodies from the Bullet world
    for (btRigidBody* rigidBody : _removedRigidBodies)
    {
        _world->removeRigidBody(rigidBody);
    }
    _removedRigidBodies.clear();

    // Update gravity if needed
    Vector3 bulletGravity = convertFromBullet(_world->getGravity());
    if (gravity != bulletGravity)
    {
        _world->setGravity(convertToBullet(gravity));
    }

    // For each rigid body component
    for (RigidBodyComponent& rigidBody : scene().components<RigidBodyComponent>())
    {
        Entity& entity = rigidBody.entity();
        TransformComponent::Iterator transform = entity.component<TransformComponent>();
        if (!entity.parent() && transform)
        {
            // Update the transform to what Bullet says it should be
            btTransform bulletTransform;
            ((btDefaultMotionState*)rigidBody._rigidBody->getMotionState())->getWorldTransform(bulletTransform);

            TransformComponent newTransform = convertFromBullet(bulletTransform);
            transform->localPosition = newTransform.localPosition;
            transform->localScale = newTransform.localScale;
            transform->localRotation = newTransform.localRotation;
            _transformSystem.commitTransform(*transform);

            // Update rigid body properties to what Bullet says it should be
            rigidBody.linearVelocity = convertFromBullet(rigidBody._rigidBody->getLinearVelocity());
            rigidBody.angularVelocity = convertFromBullet(rigidBody._rigidBody->getAngularVelocity());
        }
    }
}

void PhysicsSystem::onComponentAdded(RigidBodyComponent::Iterator rigidBody)
{
    Entity& entity = rigidBody->entity();
    TransformComponent::Iterator transform = entity.component<TransformComponent>();
    if (transform)
    {
        Mesh& mesh = *rigidBody->mesh;
        rigidBody->_collisionShape.reset(new btConvexTriangleMeshShape(toBulletMesh(&mesh)));

        btScalar mass = rigidBody->mass;
        btVector3 localInertia(0, 0, 0);
        if (mass != 0.0)
        {
            rigidBody->_collisionShape->calculateLocalInertia(mass, localInertia);
        }

        btVector3 linearVelocity = convertToBullet(rigidBody->linearVelocity);
        btVector3 angularVelocity = convertToBullet(rigidBody->angularVelocity);

        _transformSystem.updateTransform(*transform);

        rigidBody->_motionState.reset(new btDefaultMotionState(convertToBullet(*transform)));
        btRigidBody::btRigidBodyConstructionInfo info(mass, rigidBody->_motionState.get(), rigidBody->_collisionShape.get(), localInertia);
        rigidBody->_rigidBody.reset(new btRigidBody(info));
        rigidBody->_rigidBody->setSleepingThresholds(0, 0);
        rigidBody->_rigidBody->setLinearVelocity(linearVelocity);
        rigidBody->_rigidBody->setAngularVelocity(angularVelocity);
        rigidBody->_rigidBody->setAngularFactor(0.5);

        _addedRigidBodies.push_back(rigidBody->_rigidBody.get());
    }
}

void PhysicsSystem::onComponentRemoved(RigidBodyComponent::Iterator rigidBody)
{
    // Remove the transform from the committed transform vector
    const ComponentId id = rigidBody->id();
    _committedRigidBodyIds.erase(std::remove(_committedRigidBodyIds.begin(), _committedRigidBodyIds.end(), id), _committedRigidBodyIds.end());

    _removedRigidBodies.push_back(rigidBody->_rigidBody.get());
}

btTriangleMesh* PhysicsSystem::toBulletMesh(Mesh* mesh)
{
    auto it = _bulletMeshes.find(mesh);
    if (it != _bulletMeshes.end())
    {
        // The Bullet mesh was already created
        return (*it).second.get();
    }
    else
    {
        // Create a Bullet mesh from the mesh and keep it to be looked up later
        btTriangleMesh* bulletMesh = convertToBullet(*mesh);
        _bulletMeshes[mesh] = std::unique_ptr<btTriangleMesh>(bulletMesh);
        return bulletMesh;
    }
}

PhysicsSystem::ForceApplication::ForceApplication(btRigidBody* rigidBody, Vector3 force, Vector3 relativePosition) :
    rigidBody(rigidBody),
    force(force),
    relativePosition(relativePosition)
{
}
