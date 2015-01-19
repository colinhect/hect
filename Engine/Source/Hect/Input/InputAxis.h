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
#include "Hect/Input/InputAxisBinding.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"
#include "Hect/Runtime/Platform.h"

namespace hect
{

///
/// An axis manipulated by bindings to input devices.
class HECT_EXPORT InputAxis
{
public:

    ///
    /// Constructs an input axis.
    InputAxis();

    ///
    /// Constructs an input axis.
    ///
    /// \param name The name of the axis (must be unique).
    InputAxis(const std::string& name);

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Sets the name.
    ///
    /// \param name The new name.
    void setName(const std::string& name);

    ///
    /// Adds a binding to affect the input axis.
    ///
    /// \param binding The binding to add.
    void addBinding(const InputAxisBinding& binding);

    ///
    /// Updates the input axis based on the current state of its bindings.
    void update(Platform& platform, double timeStepInSeconds);

    ///
    /// Returns the current value of the axis.
    double value() const;

    friend HECT_EXPORT Encoder& operator<<(Encoder& encoder, const InputAxis& inputAxis);
    friend HECT_EXPORT Decoder& operator>>(Decoder& decoder, InputAxis& inputAxis);

private:
    std::string _name;

    std::vector<InputAxisBinding> _bindings;
    double _value { 0 };
};

}