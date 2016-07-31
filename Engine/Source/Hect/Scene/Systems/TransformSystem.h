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

#include "Hect/Core/Export.h"
#include "Hect/Scene/System.h"
#include "Hect/Scene/Systems/BoundingBoxSystem.h"
#include "Hect/Scene/Components/TransformComponent.h"

namespace hect
{

///
/// Manages the TransformComponent hierarchies of a Scene.
///
/// \system
class HECT_EXPORT TransformSystem :
    public System<TransformSystem, Components<TransformComponent>>
{
public:
    TransformSystem(Engine& engine, Scene& scene);

    ///
    /// Commits any changes made to a TransformComponent to take effect.
    ///
    /// \param transform The transform to commit.
    ///
    /// \throws InvalidOperation If the transform is not dynamic.
    void commitTransform(TransformComponent& transform);

    ///
    /// Updates the global components of a TransformComponent based on the transform
    /// hierarchy.
    ///
    /// \param transform The transform to update.
    ///
    /// \throws InvalidOperation If the transform is not dynamic.
    void updateTransform(TransformComponent& transform);

    ///
    /// Updates the global components of all committed TransformComponent%s
    /// based on the transform hierarchy.
    void updateCommittedTransforms();

    void onComponentAdded(TransformComponent::Iterator transform) override;

private:
    void updateRecursively(Entity& parent, Entity& child);

    BoundingBoxSystem::Handle _boundingBoxSystem;

    std::vector<ComponentId> _committed;
};

}
