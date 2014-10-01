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

#include <map>

#include "Hect/Input/InputAxis.h"
#include "Hect/Logic/SystemRegistry.h"

namespace hect
{

class InputSystem :
    public System
{
public:
    InputSystem(World& world);

    ///
    /// Adds an axis.
    ///
    /// \param axis The axis to add.
    ///
    /// \throws Error If an axis already exists with the same name.
    void addAxis(const InputAxis& axis);

    ///
    /// Returns the value of the axis with the given name.
    ///
    /// \param name The name of the axis.
    ///
    /// \returns The value of the axis; 0 if the axis does not exist.
    Real axisValue(const std::string& name) const;

    ///
    /// Updates all input axes in the system.
    void tick(Real timeStep) override;

private:
    std::map<std::string, InputAxis> _axes;
};

}