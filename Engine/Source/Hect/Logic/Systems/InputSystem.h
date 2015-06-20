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
#pragma once

#include <vector>

#include "Hect/Core/Export.h"
#include "Hect/Core/StringMap.h"
#include "Hect/Input/InputAxis.h"
#include "Hect/Logic/System.h"

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
    InputSystem(Engine& engine, Scene& scene);

    ///
    /// Adds an axis.
    ///
    /// \param axis The axis to add.
    ///
    /// \throws InvalidOperation If an axis already exists with the same name.
    void addAxis(const InputAxis& axis);

    ///
    /// Returns the value of the axis with the given name.
    ///
    /// \param name The name of the axis.
    ///
    /// \returns The value of the axis; 0 if the axis does not exist.
    double axisValue(const std::string& name) const;

    ///
    /// \copydoc InputSystem::axisValue()
    double axisValue(const char* name) const;

    void tick(double timeStep) override;

private:
    std::vector<InputAxis> _axes;
    StringMap<size_t> _axisIndices;

    Engine& _engine;
};

}