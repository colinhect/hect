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

#include "Hect/Event/Listener.h"
#include "Hect/Input/InputAxis.h"
#include "Hect/Input/Joystick.h"
#include "Hect/Input/Mouse.h"
#include "Hect/Input/Keyboard.h"
#include "Hect/Timing/TimeSpan.h"

struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;
struct _SDL_Haptic;
typedef struct _SDL_Haptic SDL_Haptic;

namespace hect
{

///
/// Provides access to input peripherals connected to the system.
class Input :
    public Listener<MouseEvent>,
    public Uncopyable
{
    friend class Joystick;
    friend class Window;
public:

    ///
    /// Constructs an input system without any axes.
    Input();

    ~Input();

    ///
    /// Adds an axis.
    ///
    /// \param axis The axis to add.
    ///
    /// \throws Error If an axis already exists with the same name.
    void addAxis(const InputAxis& axis);

    ///
    /// Returns the axis with the given name.
    ///
    /// \param name The name of the axis.
    ///
    /// \throws Error If no axis with the name exists.
    const InputAxis& axis(const std::string& name) const;

    ///
    /// Returns whether there is an axis with the given name.
    ///
    /// \param name The name of the axis.
    bool axisExists(const std::string& name) const;

    ///
    /// Updates all input axes in the system.
    ///
    /// \param timeStep The duration of time between each update (in seconds).
    void updateAxes(Real timeStep);

    ///
    /// Returns the mouse connected to the system.
    Mouse& mouse();

    ///
    /// Returns the keyboard connected to the system.
    Keyboard& keyboard();

    ///
    /// Returns the number of joysticks connected to the system.
    size_t joystickCount() const;

    ///
    /// Returns the joystick connected to the system of the given index.
    ///
    /// \param joystickIndex The index of the joystick to get.
    ///
    /// \throws Error If no joystick of the given index is connected.
    Joystick& joystick(size_t joystickIndex);

    void receiveEvent(const MouseEvent& event);

private:
    void enqueueEvent(const JoystickEvent& event);
    void enqueueEvent(const MouseEvent& event);
    void enqueueEvent(const KeyboardEvent& event);
    void dispatchEvents();

    void hapticRumble(Joystick& joystick, Real strength, TimeSpan duration);

    Mouse _mouse;
    Keyboard _keyboard;
    std::vector<Joystick> _joysticks;
    std::vector<SDL_Joystick*> _sdlJoysticks;
    std::map<SDL_Joystick*, SDL_Haptic*> _sdlHaptics;

    std::vector<InputAxis> _axes;
};

}