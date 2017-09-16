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
#include <Hect.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Rigid bodies are affected by gravity", "[Scene]")
{
    DefaultScene scene(Engine::instance());

    const double starting_height = 10;

    Entity& entity = scene.create_entity();

    auto& transform = entity.add_component<TransformComponent>();
    transform.local_position = Vector3(0, 0, starting_height);

    Mesh mesh = Mesh::create_box(Vector3::One);

    auto& rigid_body = entity.add_component<RigidBodyComponent>();
    rigid_body.mass = 100;
    rigid_body.mesh = mesh.create_handle();

    entity.activate();

    for (int i = 0; i < 5; ++i)
    {
        scene.tick(Seconds(0.1));
    }

    REQUIRE(transform.local_position.z < starting_height);
}
