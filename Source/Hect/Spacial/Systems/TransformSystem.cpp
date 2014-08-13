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
#include "TransformSystem.h"

#include "Hect/Graphics/Components/Model.h"
#include "Hect/Logic/World.h"

using namespace hect;

TransformSystem::TransformSystem(World& world) :
    System(world)
{
}

void TransformSystem::update()
{
    for (Transform& transform : world().components<Transform>())
    {
        Entity& entity = transform.entity();
        if (!entity.parent())
        {
            transform.updateGlobalTransform();

            for (Entity& child : entity.children())
            {
                updateTransform(entity, child);
            }
        }
    }
}

void TransformSystem::updateTransform(Entity& parent, Entity& child)
{
    auto parentTransform = parent.component<Transform>();
    if (parentTransform)
    {
        auto childTransform = child.component<Transform>();
        if (childTransform)
        {
            childTransform->updateGlobalTransform(*parentTransform);

            for (Entity& nextChild : child.children())
            {
                updateTransform(child, nextChild);
            }
        }
    }
}