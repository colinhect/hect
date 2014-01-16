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
#include "Hect.h"

using namespace hect;

#include <SFML/Graphics.hpp>

#ifdef HECT_WINDOWS
#include <Windows.h>
#endif

// Translates an SFML mouse event to an internal mouse event.
MouseEvent _translateMouseEvent(const sf::Event& event, const Vector2<int>& cursorPosition, const Vector2<int>& lastCursorPosition)
{
    MouseEvent mouseEvent;

    switch (event.type)
    {
    case sf::Event::MouseMoved:
    {
        mouseEvent.type = MouseEventType::Movement;
    }
    break;
    case sf::Event::MouseButtonPressed:
    case sf::Event::MouseButtonReleased:
    {

        // Ignore unsupported buttons
        if (event.mouseButton.button >= 3)
        {
            break;
        }

        mouseEvent.type =
            event.type == sf::Event::MouseButtonPressed ?
            MouseEventType::ButtonDown :
            MouseEventType::ButtonUp;
        mouseEvent.button = (MouseButton)event.mouseButton.button;
    }
    break;
    case sf::Event::MouseWheelMoved:
    {
        mouseEvent.type =
            event.mouseWheel.delta < 0 ?
            MouseEventType::ScrollDown :
            MouseEventType::ScrollUp;
    }
    break;
    }

    // Get the position and movement of the cursor
    mouseEvent.cursorPosition = cursorPosition;
    mouseEvent.cursorMovement = mouseEvent.cursorPosition - lastCursorPosition;

    return mouseEvent;
}

// Translates an SFML keyboard event to an internal keyboard event.
KeyboardEvent _translateKeyboardEvent(const sf::Event& event)
{
    KeyboardEvent keyboardEvent;

    switch (event.type)
    {
    case sf::Event::KeyPressed:
    case sf::Event::KeyReleased:
    {
        keyboardEvent.type =
            event.type == sf::Event::KeyPressed ?
            KeyboardEventType::KeyDown :
            KeyboardEventType::KeyUp;
        keyboardEvent.key = (Key)event.key.code;
    }
    break;
    }

    return keyboardEvent;
}

void Window::showFatalError(const std::string& message)
{
#ifdef HECT_WINDOWS
    MessageBoxA(NULL, message.c_str(), "Fatal Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#endif
}

Window::Window(const std::string& title, const VideoMode& videoMode) :
    RenderTarget(videoMode.width(), videoMode.height()),
    _sfmlWindow(nullptr),
    _cursorLocked(false)
{
    sf::VideoMode mode;
    mode.width = videoMode.width();
    mode.height = videoMode.height();
    mode.bitsPerPixel = videoMode.bitsPerPixel();

    unsigned style = sf::Style::Titlebar | sf::Style::Close;
    if (videoMode.isFullscreen())
    {
        style |= sf::Style::Fullscreen;
    }

    // Create the window
    _sfmlWindow = new sf::Window(mode, title, style);
}

Window::~Window()
{
    if (_sfmlWindow)
    {
        ((sf::Window*)_sfmlWindow)->close();
        delete (sf::Window*)_sfmlWindow;
    }
}

void Window::bind(Renderer* renderer)
{
    renderer->bindWindow(*this);
}

bool Window::pollEvents(InputSystem& inputSystem)
{
    static bool cursorVisible = true;

    sf::Vector2u windowSize = ((sf::Window*)_sfmlWindow)->getSize();
    sf::Vector2i windowCenter(windowSize.x / 2, windowSize.y / 2);

    // Occurs the first time pollEvents() is called after the mouse is locked
    if (_cursorLocked && cursorVisible)
    {
        // Hide the cursor
        ((sf::Window*)_sfmlWindow)->setMouseCursorVisible(false);
        cursorVisible = false;

        // Move cursor to center of window
        sf::Mouse::setPosition(windowCenter, *((sf::Window*)_sfmlWindow));
        _lastCursorPosition = _cursorPosition();
    }

    // Occurs the first time pollEvents() is called after the mouse is unlocked
    else if (!_cursorLocked && !cursorVisible)
    {
        // Show the cursor
        ((sf::Window*)_sfmlWindow)->setMouseCursorVisible(true);
        cursorVisible = true;

        // Move cursor to center of window
        sf::Mouse::setPosition(windowCenter, *((sf::Window*)_sfmlWindow));
        _lastCursorPosition = _cursorPosition();
    }

    // Poll all pending events and translate/dispatch to the proper systems
    sf::Event event;
    while (((sf::Window*)_sfmlWindow)->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            ((sf::Window*)_sfmlWindow)->close();
            break;
        case sf::Event::MouseMoved:
        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseButtonReleased:
        case sf::Event::MouseWheelMoved:
            inputSystem._enqueueEvent(_translateMouseEvent(event, _cursorPosition(), _lastCursorPosition));
            break;
        case sf::Event::KeyPressed:
        case sf::Event::KeyReleased:
            inputSystem._enqueueEvent(_translateKeyboardEvent(event));
            break;
        }
    }

    inputSystem._dispatchEvents();

    // If the cursor is locked then move it back to the center of the window
    if (_cursorLocked)
    {
        if (sf::Mouse::getPosition(*((sf::Window*)_sfmlWindow)) != windowCenter)
        {
            sf::Mouse::setPosition(windowCenter, *((sf::Window*)_sfmlWindow));
        }
    }

    // Save the current cursor position as the last current position in order to
    // compute the relative cursor movements
    _lastCursorPosition = _cursorPosition();

    return ((sf::Window*)_sfmlWindow)->isOpen();
}

void Window::swapBuffers()
{
    ((sf::Window*)_sfmlWindow)->display();
}

void Window::setCursorLocked(bool locked)
{
    _cursorLocked = locked;
}

bool Window::isCursorLocked() const
{
    return _cursorLocked;
}

Vector2<int> Window::_cursorPosition()
{
    sf::Vector2i position = sf::Mouse::getPosition(*((sf::Window*)_sfmlWindow));
    return Vector2<int>(position.x, ((sf::Window*)_sfmlWindow)->getSize().y - position.y);
}