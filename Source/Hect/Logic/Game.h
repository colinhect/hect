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

#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/Window.h"
#include "Hect/Input/InputDevices.h"
#include "Hect/IO/FileSystem.h"
#include "Hect/IO/JsonValue.h"

namespace hect
{

class Scene;

class Game :
    public Uncopyable
{
public:
    Game(const std::string& name, const Path& settingsFilePath);
    virtual ~Game();

    virtual void execute() = 0;

    void playScene(Scene& scene);

    FileSystem& fileSystem();
    InputDevices& inputDevices();

    Renderer& renderer();
    Window& window();

    const JsonValue& settings() const;

private:
    std::unique_ptr<FileSystem> _fileSystem;
    std::unique_ptr<InputDevices> _inputDevices;
    std::unique_ptr<Renderer> _renderer;
    std::unique_ptr<Window> _window;
    JsonValue _settings;
};

}