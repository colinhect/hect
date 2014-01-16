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

namespace hect
{

///
/// Provides access to input peripherals connected to the system.
class InputSystem :
    public Listener<MouseEvent>
{
    friend class Window;
public:

    ///
    /// Constructs an input system without any axes.
    InputSystem();

    ///
    /// Constructs an input system given axes.
    ///
    /// \param axes The input axes to include in the system.
    ///
    /// \throws Error If there are multiple axes with the same name.
    InputSystem(const InputAxis::Array& axes);

    ///
    /// Returns the axis with the given name.
    ///
    /// \param name The name of the axis.
    ///
    /// \throws Error If no axis with the name exists.
    const InputAxis& axisWithName(const std::string& name) const;

    ///
    /// Updates all input axes in the system.
    ///
    /// \param timeStep The duration of time between each update (in seconds).
    void updateAxes(double timeStep);

    ///
    /// Returns the mouse connected to the system.
    Mouse& mouse();

    ///
    /// Returns the keyboard connected to the system.
    Keyboard& keyboard();

    void receiveEvent(const MouseEvent& event);

private:
    void _enqueueEvent(const MouseEvent& event);
    void _enqueueEvent(const KeyboardEvent& event);
    void _dispatchEvents();

    Mouse _mouse;
    Keyboard _keyboard;

    InputAxis::Array _axes;
    std::map<std::string, InputAxis*> _mappedAxes;
};

}