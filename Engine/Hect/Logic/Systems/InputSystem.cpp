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
#include "InputSystem.h"

#include "Hect/Logic/World.h"
#include "Hect/Platform/Platform.h"

using namespace hect;

InputSystem::InputSystem(World& world) :
    System(world)
{
}

void InputSystem::addAxis(const InputAxis& axis)
{
    auto it = _axes.find(axis.name());
    if (it != _axes.end())
    {
        throw Error(format("Multiple input axes with name '%s'", axis.name().c_str()));
    }

    _axes[axis.name()] = axis;
}

Real InputSystem::axisValue(const std::string& name) const
{
    auto it = _axes.find(name);
    if (it != _axes.end())
    {
        const InputAxis& axis = it->second;
        return axis.value();
    }
    return 0;
}

void InputSystem::tick(Real timeStep)
{
    // Update each axis
    for (auto& pair : _axes)
    {
        InputAxis& axis = pair.second;
        axis.update(timeStep);
    }
}