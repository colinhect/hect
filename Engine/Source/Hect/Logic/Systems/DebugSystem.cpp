///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
#include "DebugSystem.h"

#include "Hect/Logic/Systems/RenderSystem.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

DebugSystem::DebugSystem(Engine& engine, Scene& scene) :
    System(engine, scene, SystemTickStage::Precedent),
    _renderer(engine.renderer()),
    _enabled(false)
{
}

void DebugSystem::renderBox(DebugColor color, const Box& box, const Vector3& position, const Quaternion& rotation)
{
    _boxes.emplace_back(box, position, rotation, color);
}

void DebugSystem::addRenderCalls(RenderSystem& renderSystem)
{
    for (DebugBox& box : _boxes)
    {
        renderSystem.addRenderCall(box.transform, *boxMesh, _coloredMaterials[static_cast<size_t>(box.color)]);
    }
}

bool DebugSystem::isEnabled() const
{
    return _enabled;
}

void DebugSystem::setEnabled(bool enabled)
{
    _enabled = enabled;
}

void DebugSystem::initialize()
{
    addColoredMaterial(Color(100, 0, 0)); // Primary
    addColoredMaterial(Color(0, 100, 0)); // Secondary
    addColoredMaterial(Color(0, 0, 100)); // Tertiary

    _renderer.uploadShader(*coloredLineShader);
    _renderer.uploadMesh(*boxMesh);
}

void DebugSystem::tick(double timeStep)
{
    (void)timeStep;
    _boxes.clear();
}

DebugSystem::DebugBox::DebugBox()
{
}

DebugSystem::DebugBox::DebugBox(const Box& box, const Vector3& position, const Quaternion& rotation, DebugColor color) :
    box(box),
    color(color)
{
    transform.globalPosition = position;
    transform.globalScale = box.scale();
    transform.globalRotation = rotation;
}

void DebugSystem::addColoredMaterial(const Color& color)
{
    // Create a material for this color
    Material material;
    material.setShader(coloredLineShader);
    material.setUniformValue("color", color);

    _coloredMaterials.push_back(material);
}
