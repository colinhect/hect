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

#include "Hect/Graphics/Mesh.h"
#include "Hect/IO/AssetCache.h"
#include "Hect/Logic/Scene.h"

// Forward declare Bullet classes
class btRigidBody;
class btMotionState;
class btCollisionShape;

namespace hect
{

///
/// A simulated physical body.
///
/// \component
class RigidBody :
    public Component<RigidBody>
{
    friend class PhysicsSystem;
public:

    ///
    /// The mass.
    Real mass { 0 };

    ///
    /// The linear velocity.
    Vector3 linearVelocity;

    ///
    /// The angular velocity.
    Vector3 angularVelocity;

    ///
    /// The collision mesh.
    AssetHandle<Mesh> mesh;

    void encode(Encoder& encoder) const;
    void decode(Decoder& decoder);

private:
    std::shared_ptr<btRigidBody> _rigidBody;
    std::shared_ptr<btMotionState> _motionState;
    std::shared_ptr<btCollisionShape> _collisionShape;
};

}