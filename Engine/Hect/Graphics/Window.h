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

#include <memory>
#include <string>

#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Graphics/VideoMode.h"

namespace hect
{

///
/// The window that the application is rendering to.
///
/// \note In most cases the window represents the native OS window.
class Window :
    public RenderTarget,
    public Uncopyable
{
public:

    ///
    /// A reference-counted pointer to a window.
    typedef std::shared_ptr<Window> Pointer;

    ///
    /// Constructs a window given a title and the video mode.
    ///
    /// \param title The window title.
    /// \param videoMode The video mode to use.
    Window(const std::string& title, const VideoMode& videoMode);

    ///
    /// Closes the window.
    virtual ~Window();

    void bind(Renderer& renderer);
    virtual void swapBuffers() = 0;
};

}