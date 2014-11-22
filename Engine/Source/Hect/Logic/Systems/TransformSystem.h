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

#include "Hect/Event/Listener.h"
#include "Hect/Logic/Scene.h"
#include "Hect/Logic/Components/Transform.h"

namespace hect
{

///
/// Manages the transform hierarchies of the scene.
///
/// \system
class TransformSystem :
    public System,
    public Listener<ComponentEvent<Transform>>
{
public:
    TransformSystem(Scene& scene);

    ///
    /// Forces the update of a transform and all child transforms.
    ///
    /// \param transform The transform to update.
    void forceUpdate(Transform& transform);

    ///
    /// Marks a transform to be updated on the next TransformSystem::tick().
    ///
    /// \note If the transform is already marked for update then no action
    /// will be performed.
    ///
    /// \param transform The transform to mark for update.
    void markForUpdate(Transform& transform);

    void tick(Real timeStep) override;

    void receiveEvent(const ComponentEvent<Transform>& event) override;

private:
    void updateHeierarchy(Entity& parent, Entity& child);
    void postUpdate(Entity& entity);

    std::vector<ComponentId> _markedForUpdate;
};

}