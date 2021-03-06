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
#include "InputSystem.h"

#include "Hect/IO/DataValueDecoder.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

InputSystem::InputSystem(Scene& scene, Platform& platform, const DataValue& settings) :
    System(scene),
    _platform(platform)
{
    for (const DataValue& axis_value : settings["input_axes"])
    {
        try
        {
            InputAxis axis;

            DataValueDecoder decoder(axis_value);
            decoder >> decode_value(axis);

            add_axis(axis);
        }
        catch (const DecodeError& error)
        {
            HECT_ERROR(format("Invalid input axes: %s", error.what()));
        }
    }
}

void InputSystem::add_axis(const InputAxis& axis)
{
    auto it = _axes.find(axis.name());
    if (it != _axes.end())
    {
        throw InvalidOperation(format("Input axis with name '%s' already exists", axis.name().data()));
    }

    if (axis.name().empty())
    {
        throw InvalidOperation("Input axis name cannot be empty");
    }

    _axes[axis.name()] = axis;

    HECT_INFO(format("Added input axis '%s'", axis.name().data()));
}

double InputSystem::axis_value(Name name) const
{
    auto it = _axes.find(name);
    if (it != _axes.end())
    {
        return it->second.value();
    }

    return 0.0;
}

void InputSystem::update_axes(Seconds time_step)
{
    // Update each axis
    for (auto& pair : _axes)
    {
        InputAxis& axis = pair.second;
        axis.update(_platform, time_step);
    }
}
