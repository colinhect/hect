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
#include "InputDevices.h"

#include <SDL.h>
#include <algorithm>

#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"

using namespace hect;

InputDevices::InputDevices()
{
    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        SDL_Joystick* joystick = SDL_JoystickOpen(i);
        if (joystick)
        {
            _sdlJoysticks.push_back(joystick);

            std::string name = SDL_JoystickName(joystick);
            size_t buttonCount = SDL_JoystickNumButtons(joystick);
            size_t axisCount = SDL_JoystickNumAxes(joystick);

            SDL_Haptic* haptic = SDL_HapticOpenFromJoystick(joystick);
            if (haptic)
            {
                if (SDL_HapticRumbleInit(haptic) == 0)
                {
                    _sdlHaptics[joystick] = haptic;
                }
                else
                {
                    SDL_HapticClose(haptic);
                }
            }

            HECT_INFO(format("Detected joystick '%s' with %i buttons and %i axes", name.c_str(), buttonCount, axisCount));

            _joysticks.push_back(Joystick(*this, i, name, buttonCount, axisCount));
        }
    }
}

InputDevices::~InputDevices()
{
    // Close all haptic devices
    for (auto& pair : _sdlHaptics)
    {
        SDL_HapticClose(pair.second);
    }
    _sdlHaptics.clear();

    // Close all joysticks
    for (SDL_Joystick* joystick : _sdlJoysticks)
    {
        SDL_JoystickClose(joystick);
    }
    _sdlJoysticks.clear();
}

Mouse& InputDevices::mouse()
{
    return _mouse;
}

Keyboard& InputDevices::keyboard()
{
    return _keyboard;
}

size_t InputDevices::joystickCount() const
{
    return _joysticks.size();
}

Joystick& InputDevices::joystick(size_t joystickIndex)
{
    if (joystickIndex < _joysticks.size())
    {
        return _joysticks[joystickIndex];
    }
    else
    {
        throw Error(format("System does not have joystick of index %i", joystickIndex));
    }
}

void InputDevices::enqueueEvent(const JoystickEvent& event)
{
    joystick(event.joystickIndex).enqueueEvent(event);
}

void InputDevices::enqueueEvent(const MouseEvent& event)
{
    _mouse.enqueueEvent(event);
}

void InputDevices::enqueueEvent(const KeyboardEvent& event)
{
    _keyboard.enqueueEvent(event);
}

void InputDevices::dispatchEvents()
{
    _mouse.dispatchEvents();
    _keyboard.dispatchEvents();
}

void InputDevices::hapticRumble(Joystick& joystick, Real strength, TimeSpan duration)
{
    auto it = _sdlHaptics.find(_sdlJoysticks[joystick._index]);
    if (it != _sdlHaptics.end())
    {
        SDL_HapticRumblePlay(it->second, (float)strength, (uint32_t)duration.milliseconds());
    }
}