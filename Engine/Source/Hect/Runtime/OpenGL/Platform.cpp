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
#include "Hect/Runtime/Platform.h"

#include "Hect/Core/Configuration.h"

#ifdef HECT_PLATFORM_SDL

#include <algorithm>
#include <SDL.h>

#include "Hect/Core/Logging.h"

using namespace hect;

namespace
{

std::unique_ptr<Mouse> _mouse;
std::unique_ptr<Keyboard> _keyboard;
std::vector<Joystick> _joysticks;
std::vector<SDL_Joystick*> _open_joysticks;
MouseMode _mouse_mode { MouseMode::Cursor };

Key convert_key(SDL_Keycode key)
{
    switch (key)
    {
    case SDLK_a:
        return Key::A;
    case SDLK_b:
        return Key::B;
    case SDLK_c:
        return Key::C;
    case SDLK_d:
        return Key::D;
    case SDLK_e:
        return Key::E;
    case SDLK_f:
        return Key::F;
    case SDLK_g:
        return Key::G;
    case SDLK_h:
        return Key::H;
    case SDLK_i:
        return Key::I;
    case SDLK_j:
        return Key::J;
    case SDLK_k:
        return Key::K;
    case SDLK_l:
        return Key::L;
    case SDLK_m:
        return Key::M;
    case SDLK_n:
        return Key::N;
    case SDLK_o:
        return Key::O;
    case SDLK_p:
        return Key::P;
    case SDLK_q:
        return Key::Q;
    case SDLK_r:
        return Key::R;
    case SDLK_s:
        return Key::S;
    case SDLK_t:
        return Key::T;
    case SDLK_u:
        return Key::U;
    case SDLK_v:
        return Key::V;
    case SDLK_w:
        return Key::W;
    case SDLK_x:
        return Key::X;
    case SDLK_y:
        return Key::Y;
    case SDLK_z:
        return Key::Z;
    case SDLK_ESCAPE:
        return Key::Esc;
    case SDLK_TAB:
        return Key::Tab;

    case SDLK_F1:
        return Key::F1;
    case SDLK_F2:
        return Key::F2;
    case SDLK_F3:
        return Key::F3;
    case SDLK_F4:
        return Key::F4;
    case SDLK_F5:
        return Key::F5;
    case SDLK_F6:
        return Key::F6;
    case SDLK_F7:
        return Key::F7;
    case SDLK_F8:
        return Key::F8;
    case SDLK_F9:
        return Key::F9;
    case SDLK_F10:
        return Key::F10;
    case SDLK_F11:
        return Key::F11;
    case SDLK_F12:
        return Key::F12;

    case SDLK_LCTRL:
        return Key::LeftCtrl;
    case SDLK_LSHIFT:
        return Key::LeftShift;
    case SDLK_LALT:
        return Key::LeftAlt;

    default:
        return Key::Unknown;
    }
}

}

Platform::~Platform()
{
    // Close all joysticks
    for (SDL_Joystick* joystick : _open_joysticks)
    {
        SDL_JoystickClose(joystick);
    }
    _open_joysticks.clear();

    SDL_Quit();
}

bool Platform::handle_events()
{
    _mouse->clear_movement();

    // Update the mouse mode if needed
    MouseMode current_mouse_mode = _mouse->mode();
    if (current_mouse_mode != _mouse_mode)
    {
        _mouse_mode = current_mouse_mode;

        switch (current_mouse_mode)
        {
        case MouseMode::Cursor:
            SDL_SetRelativeMouseMode(SDL_FALSE);
            break;
        case MouseMode::Relative:
            SDL_SetRelativeMouseMode(SDL_TRUE);
            break;
        }
    }

    bool active = true;

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            active = false;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
            KeyboardEvent event;
            event.type = e.type == SDL_KEYDOWN ? KeyboardEventType::KeyDown : KeyboardEventType::KeyUp;
            event.key = convert_key(e.key.keysym.sym);
            _keyboard->enqueue_event(event);
        }
        break;
        case SDL_MOUSEMOTION:
        {
            // Get relative cursor movement
            int movement_x = 0;
            int movement_y = 0;
            SDL_GetRelativeMouseState(&movement_x, &movement_y);

            // Get absolute cursor position
            int position_x = 0;
            int position_y = 0;
            SDL_GetMouseState(&position_x, &position_y);

            // Enqueue the event
            MouseEvent event;
            event.type = MouseEventType::Movement;
            event.cursor_movement = IntVector2(movement_x, -movement_y);
            event.cursor_position = IntVector2(position_x, position_y);
            _mouse->enqueue_event(event);
        }
        break;
        case SDL_MOUSEWHEEL:
        {
            // Enqueue the event
            MouseEvent event;
            event.type = e.wheel.y > 0 ? MouseEventType::ScrollUp : MouseEventType::ScrollDown;
            _mouse->enqueue_event(event);
        }
        break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        {
            // Get absolute cursor position
            int position_x = 0;
            int position_y = 0;
            SDL_GetMouseState(&position_x, &position_y);

            // Enqueue the event
            MouseEvent event;
            event.type = e.type == SDL_MOUSEBUTTONDOWN ? MouseEventType::ButtonDown : MouseEventType::ButtonUp;
            event.cursor_position = IntVector2(position_x, position_y);
            _mouse->enqueue_event(event);
        }
        break;
        case SDL_JOYAXISMOTION:
        {
            // Enqueue the event
            JoystickEvent event;
            event.type = JoystickEventType::AxisMotion;
            event.index = e.jaxis.which;
            event.axis = static_cast<JoystickAxis>(e.jaxis.axis);
            event.axis_value = std::max(static_cast<double>(e.jaxis.value) / 32767.0, -1.0);
            _joysticks[event.index].enqueue_event(event);
        }
        break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
        {
            // Enqueue the event
            JoystickEvent event;
            event.type = e.type == SDL_JOYBUTTONDOWN ? JoystickEventType::ButtonDown : JoystickEventType::ButtonUp;
            event.index = e.jbutton.which;
            event.button = static_cast<JoystickButton>(e.jbutton.button);
            _joysticks[event.index].enqueue_event(event);
        }
        break;
        default:
        break;
        }
    }

    _mouse->dispatch_events();
    _keyboard->dispatch_events();

    for (Joystick& joystick : _joysticks)
    {
        joystick.dispatch_events();
    }

    return active;
}

bool Platform::has_mouse()
{
    return true;
}

Mouse& Platform::mouse()
{
    return *_mouse;
}

bool Platform::has_keyboard()
{
    return true;
}

Keyboard& Platform::keyboard()
{
    return *_keyboard;
}

bool Platform::has_joystick(JoystickIndex index)
{
    return index < _joysticks.size();
}

Joystick& Platform::joystick(JoystickIndex index)
{
    if (!has_joystick(index))
    {
        throw InvalidOperation(format("No joystick connected at index %i", index));
    }
    return _joysticks[index];
}

Platform::Platform()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) != 0)
    {
        throw FatalError(format("Failed to initialize SDL: %s", SDL_GetError()));
    }

    SDL_version version;
    SDL_GetVersion(&version);
    HECT_INFO(format("SDL version %d.%d.%d", version.major, version.minor, version.patch));

    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        SDL_Joystick* joystick = SDL_JoystickOpen(i);
        if (joystick)
        {
            _open_joysticks.push_back(joystick);

            std::string name = SDL_JoystickName(joystick);
            size_t button_count = SDL_JoystickNumButtons(joystick);
            size_t axis_count = SDL_JoystickNumAxes(joystick);

            HECT_INFO(format("Detected joystick '%s' with %i buttons and %i axes", name.data(), button_count, axis_count));

            _joysticks.push_back(Joystick(name, button_count, axis_count));
            for (size_t axis_index = 0; axis_index < axis_count; ++axis_index)
            {
                JoystickEvent event;
                event.type = JoystickEventType::AxisMotion;
                event.index = _joysticks.size() - 1;
                event.axis = static_cast<JoystickAxis>(axis_index);
                event.axis_value = std::max(static_cast<double>(SDL_JoystickGetAxis(joystick, static_cast<int>(axis_index))) / 32767.0, -1.0);
                _joysticks[event.index].enqueue_event(event);
            }
        }
    }

    _mouse.reset(new Mouse());
    _keyboard.reset(new Keyboard());
}

#endif