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

#include <unordered_map>

#include "Hect/Core/Export.h"
#include "Hect/Core/Name.h"
#include "Hect/Input/InputAxis.h"
#include "Hect/Scene/System.h"
#include "Hect/Runtime/Platform.h"

namespace hect
{

///
/// Maps user input to input axes.
///
/// \system
class HECT_EXPORT InputSystem :
    public System<InputSystem>
{
public:
    InputSystem(Scene& scene, Platform& platform, const DataValue& settings);

    ///
    /// Adds an axis.
    ///
    /// \param axis The axis to add.
    ///
    /// \throws InvalidOperation If an axis already exists with the same name.
    void add_axis(const InputAxis& axis);

    ///
    /// Returns the value of the axis with the given name.
    ///
    /// \param name The name of the axis.
    ///
    /// \returns The value of the axis; 0 if the axis does not exist.
    double axis_value(Name name) const;

    ///
    /// Update the values of all axes.
    ///
    /// \param time_step The duration of time in seconds to update for.
    void update_axes(Seconds time_step);

private:
    Platform& _platform;
    std::unordered_map<Name, InputAxis> _axes;
};

}
