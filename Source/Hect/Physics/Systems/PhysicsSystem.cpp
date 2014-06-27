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
#include "PhysicsSystem.h"

#include "Hect/Logic/Scene.h"
#include "Hect/Graphics/Components/Transform.h"
#include "Hect/Physics/Components/RigidBody.h"

using namespace hect;

#include "Hect/Internal/Bullet.h"

PhysicsSystem::PhysicsSystem(Scene& scene, TaskPool& taskPool) :
    System(scene),
    _taskPool(&taskPool),
    _configuration(new btDefaultCollisionConfiguration()),
    _dispatcher(new btCollisionDispatcher(_configuration.get())),
    _broadphase(new btDbvtBroadphase()),
    _solver(new btSequentialImpulseConstraintSolver()),
    _world(new btDiscreteDynamicsWorld(_dispatcher.get(), _broadphase.get(), _solver.get(), _configuration.get()))
{
    setGravity(Vector3::zero());

    // Register to component events for rigid bodies
    ComponentPool<RigidBody>& rigidBodyPool = scene.components<RigidBody>();
    rigidBodyPool.dispatcher().addListener(*this);
}

PhysicsSystem::~PhysicsSystem()
{
    _updateTask.wait();

    // Unregister from component events for rigid bodies
    ComponentPool<RigidBody>& rigidBodyPool = scene().components<RigidBody>();
    rigidBodyPool.dispatcher().removeListener(*this);
}

void PhysicsSystem::asynchronousUpdate(Real timeStep, unsigned maxSubStepCount)
{
    // Wait until the last update (if any) completes
    _updateTask.wait();

    // Update the dynamics world
    _updateTask = _taskPool->enqueue([this, timeStep, maxSubStepCount]()
    {
        _world->stepSimulation(timeStep, maxSubStepCount);
    });
}

void PhysicsSystem::updateTransforms()
{
    // Wait until the last update (if any) completes
    _updateTask.wait();

    // For each rigid body component
    for (RigidBody& rigidBody : scene().components<RigidBody>())
    {
        Entity& entity = rigidBody.entity();
        if (!entity.parent() && entity.component<Transform>())
        {
            // Update the transform to what Bullet says it should be
            btTransform& bulletTransform = rigidBody._rigidBody->getWorldTransform();
            entity.replaceComponent(convertFromBullet(bulletTransform));
        }
    }
}

const Vector3& PhysicsSystem::gravity() const
{
    return _gravity;
}

void PhysicsSystem::setGravity(const Vector3& gravity)
{
    _gravity = gravity;
    _world->setGravity(convertToBullet(gravity));
}

void PhysicsSystem::receiveEvent(const ComponentEvent<RigidBody>& event)
{
    Entity& entity = event.entity();

    auto transform = entity.component<Transform>();
    if (transform)
    {
        auto rigidBody = entity.component<RigidBody>();
        if (rigidBody)
        {
            if (event.type() == ComponentEventType::Add)
            {
                Mesh& mesh = *rigidBody->mesh();

                rigidBody->_collisionShape.reset(new btConvexTriangleMeshShape(_toBulletMesh(&mesh)));

                btScalar mass = rigidBody->mass();
                btVector3 localInertia(0, 0, 0);
                if (mass != 0.0)
                {
                    rigidBody->_collisionShape->calculateLocalInertia(mass, localInertia);
                }

                btVector3 linearVelocity = convertToBullet(rigidBody->linearVelocity());
                btVector3 angularVelocity = convertToBullet(rigidBody->angularVelocity());

                rigidBody->_motionState.reset(new btDefaultMotionState(convertToBullet(*transform)));
                btRigidBody::btRigidBodyConstructionInfo info(mass, rigidBody->_motionState.get(), rigidBody->_collisionShape.get(), localInertia);
                rigidBody->_rigidBody.reset(new btRigidBody(info));
                rigidBody->_rigidBody->setSleepingThresholds(0, 0);
                rigidBody->_rigidBody->setLinearVelocity(linearVelocity);
                rigidBody->_rigidBody->setAngularVelocity(angularVelocity);

                _world->addRigidBody(rigidBody->_rigidBody.get());
            }
            else if (event.type() == ComponentEventType::Remove)
            {
                _world->removeRigidBody(rigidBody->_rigidBody.get());
            }
        }
    }
}

btTriangleMesh* PhysicsSystem::_toBulletMesh(Mesh* mesh)
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
        _bulletMeshes[mesh] = std::shared_ptr<btTriangleMesh>(bulletMesh);
        return bulletMesh;
    }
}