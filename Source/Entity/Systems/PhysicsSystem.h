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
class PhysicsSystem :
    public System,
    public Uncopyable
{
public:
    PhysicsSystem();

    ///
    /// \copydoc System::includesEntity()
    bool includesEntity(const Entity& entity) const;

    ///
    /// Updates all physical bodies.
    ///
    /// \param timeStep The duration of time between each update (in seconds).
    /// \param maxSubStepCount The maximum number of sub-steps.
    void update(double timeStep, unsigned maxSubStepCount);

    ///
    /// Returns the gravity.
    const Vector3<>& gravity() const;

    ///
    /// Sets the gravity.
    ///
    /// \param gravity The new gravity.
    void setGravity(const Vector3<>& gravity);

protected:
    void addEntity(const Entity& entity);
    void removeEntity(const Entity& entity);

private:
    btTriangleMesh* _toBulletMesh(Mesh* mesh);

    std::shared_ptr<btCollisionConfiguration> _configuration;
    std::shared_ptr<btCollisionDispatcher> _dispatcher;
    std::shared_ptr<btBroadphaseInterface> _broadphase;
    std::shared_ptr<btConstraintSolver> _solver;
    std::shared_ptr<btDynamicsWorld> _world;

    std::map<Mesh*, std::shared_ptr<btTriangleMesh>> _bulletMeshes;

    Vector3<> _gravity;
};

}