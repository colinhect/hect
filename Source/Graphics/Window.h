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

class InputSystem;

///
/// The window that the engine is rendering to.
///
/// \remarks In most cases the window represents the native OS window.
class Window :
    public RenderTarget,
    public Uncopyable
{
public:

    ///
    /// Shows a fatal error.
    ///
    /// \param message The error message to show.
    static void showFatalError(const std::string& message);

    ///
    /// Constructs a window given a title and the settings.
    ///
    /// \param title The window title.
    /// \param videoMode The video mode to use.
    Window(const std::string& title, const VideoMode& videoMode);

    ///
    /// Closes the window.
    ~Window();

    ///
    /// Polls events from the window and redirects them to an input system.
    ///
    /// \param inputSystem The input system to redirect the window input events
    /// to.
    ///
    /// \returns True if the window has not received a close event; false
    /// otherwise.
    bool pollEvents(InputSystem& inputSystem);

    ///
    /// Swaps the back buffer.
    void swapBuffers();

    ///
    /// Sets whether the cursor is locked at the center of the window.
    ///
    /// \param locked Whether the cursor is locked.
    void setCursorLocked(bool locked);

    ///
    /// Returns whether the cursor is locked.
    bool isCursorLocked() const;

    ///
    /// \copydoc RenderTarget::bind()
    void bind(Renderer* renderer);

private:
    Vector2<int> _cursorPosition();

    void* _sfmlWindow;

    bool _cursorLocked;
    Vector2<int> _lastCursorPosition;
};

}