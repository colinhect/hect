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
#include "DebugRenderSystem.h"

using namespace hect;

DebugRenderSystem::DebugRenderSystem(Scene& scene) :
    System(scene)
{
}

void DebugRenderSystem::clear()
{
    _boxes.clear();
}

bool DebugRenderSystem::isEnabled() const
{
    return _enabled;
}

void DebugRenderSystem::setEnabled(bool enabled)
{
    _enabled = enabled;
}

void DebugRenderSystem::renderBox(const Box& box, const Vector3& color, const Vector3& position, const Quaternion& rotation)
{
    if (_enabled)
    {
        DebugBox debugBox;
        debugBox.box = box;
        debugBox.color = color;
        debugBox.position = position;
        debugBox.rotation = rotation;

        _boxes.push_back(debugBox);
    }
}

const DebugRenderSystem::DebugBoxSequence DebugRenderSystem::boxes() const
{
    return _boxes;
}